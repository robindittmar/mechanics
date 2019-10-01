#include "Application.h"

std::vector<BulletTracerEntry> CApplication::m_pBulletTracer;
std::vector<HitmarkerEntry> CApplication::m_pHitmarker;

CApplication::CApplication()
{
	m_pNetVarMgr = nullptr;

	m_pClientModeHook = nullptr;
	m_pModelRenderHook = nullptr;
	m_pClientHook = nullptr;
	m_pPanelHook = nullptr;
	m_pSurfaceHook = nullptr;
	m_pGameEventManagerHook = nullptr;
	m_pViewRenderHook = nullptr;
	m_pEngineSoundHook = nullptr;
	m_pMdlHook = nullptr;

	m_bGotSendPackets = false;

	m_bInitialHookDone = false;
	m_bIsHooked = false;
}

CApplication::~CApplication()
{
    delete m_pMdlHook;
    delete m_pEngineSoundHook;
    delete m_pViewRenderHook;
    delete m_pGameEventManagerHook;
    delete m_pSurfaceHook;
    delete m_pPanelHook;
    delete m_pClientHook;
    delete m_pModelRenderHook;
    delete m_pClientModeHook;
    delete m_pNetVarMgr;
}

CApplication* CApplication::Instance()
{
	static CApplication inst;
	return &inst;
}

void CApplication::Run(HMODULE hModule)
{
	m_hModule = hModule;

	this->Setup();
	this->Hook();
}

void CApplication::Detach()
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Detaching ...\n");
#endif

	// Enable mouse again
	this->m_pGui->SetEnableGameInput(true);
	this->m_pGui->Cleanup();

	// Unregister listener
	this->m_pGameEventManager->RemoveListener(&this->m_gameEventListener);

	// Unhook everything
	this->Unhook();

	// Free ResourceManager
	delete g_pResourceManager;
	g_pResourceManager = nullptr;

#ifdef _DEBUG
	// Free console
	delete g_pConsole;
	g_pConsole = nullptr;
#endif

	// TODO: Stimmt, das andere ist ja nicht von uns.. Cleanups *MÜSSEN* immer andersrum passieren als das initialisieren
	// ----------- Our stuff -----------

	// Thirdperson
	if (m_pInput->m_fCameraInThirdPerson)
	{
		m_pInput->m_fCameraInThirdPerson = false;
	}

	// FovChanger
	if (m_visuals.GetFovChangeScoped())
	{
		ConVar* pZoomSensitivity = m_pCVar->FindVar(/*zoom_sensitivity_ratio_mouse*/CXorString("\x6D\x64\xEA\xAF\x48\x78\xE0\xAC\x64\x62\xF1\xAB\x61\x62\xF1\xBB\x48\x79\xE4\xB6\x7E\x64\xDA\xAF\x78\x7E\xF6\xA7").ToCharArray());
		pZoomSensitivity->SetValue(m_visuals.GetZoomSensitivity());
	}

	// NoSmoke
	m_visuals.NoSmoke(false);

	// SkinChanger Modeldelete
	if (m_skinchanger.GetEnabled())
		m_pClientState->ForceFullUpdate();

	// ClanTag
	if (m_misc.GetIsCustomClanTag())
	{
		m_misc.SetClanTag(nullptr);
		m_misc.ApplyClanTag();
	}

	// MaterialVisuals
	m_materialvisuals.Nightmode(100);
	m_materialvisuals.Asuswalls(100);
	m_materialvisuals.Skychanger(0);

	// Free & Exit
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadFreeLibrary, this->m_hModule, 0, nullptr);
}

bool CApplication::CreateFolder(const char* pName)
{
	char pBuffer[MAX_PATH];
	snprintf(pBuffer, MAX_PATH, "%s%s", this->GetWorkingDirectory(), pName);

	if (!CreateDirectory(pBuffer, nullptr))
	{
		DWORD dwErr = GetLastError();
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
#ifdef _DEBUG
			g_pConsole->Write(LOGLEVEL_ERROR, "Error when creating folder ('%s'): %d\n", pBuffer, dwErr);
#endif
			return false;
		}
	}

	return true;
}

void CApplication::Unhook()
{
	// Proxy functions
	m_pNetVarEyeAngles1->UnhookProxy();
	m_pNetVarEyeAngles0->UnhookProxy();
	m_pNetVarLowerBodyYaw->UnhookProxy();
	m_pNetVarSequence->UnhookProxy();

	// Reverse order, in case of any dependencies
	this->m_pMdlHook->Restore();
	this->m_pEngineSoundHook->Restore();
	this->m_pViewRenderHook->Restore();
	this->m_pGameEventManagerHook->Restore();
	this->m_pSurfaceHook->Restore();
	this->m_pPanelHook->Restore();
	this->m_pClientHook->Restore();
	this->m_pModelRenderHook->Restore();
	this->m_pClientModeHook->Restore();
	
	this->m_bIsHooked = false;
}

void CApplication::Rehook()
{
	this->m_pClientModeHook->Replace();
	this->m_pModelRenderHook->Replace();
	this->m_pClientHook->Replace();
	this->m_pPanelHook->Replace();
	this->m_pSurfaceHook->Replace();
	this->m_pGameEventManagerHook->Replace();
	this->m_pViewRenderHook->Replace();
	this->m_pEngineSoundHook->Replace();
	this->m_pMdlHook->Replace();

	g_pSequenceProxy = m_pNetVarSequence->HookProxy(hk_SetViewModelSequence);
	g_pLowerBodyYawProxy = m_pNetVarLowerBodyYaw->HookProxy(hk_SetLowerBodyYawTarget);
	g_pEyeAnglesProxy0 = m_pNetVarEyeAngles0->HookProxy(hk_SetEyeAnglesProxy0);
	g_pEyeAnglesProxy1 = m_pNetVarEyeAngles1->HookProxy(hk_SetEyeAnglesProxy1);

	this->m_bIsHooked = true;
}

IClientEntity* CApplication::GetLocalPlayer(bool bGetTargetIfLocalDead)
{
	IClientEntity* pEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());
	if (bGetTargetIfLocalDead && !pEntity->IsAlive())
		pEntity = pEntity->GetObserverTarget();

	return pEntity;
}

void CApplication::Setup()
{
#ifdef _DEBUG
	// Setup console
	g_pConsole = new CConsole();
	g_pConsole->WritePlain(".mechanics\n > Build date: %s\n > Build time: %s\n\n", __DATE__, __TIME__);
#endif

	// Grab info about our own dll path etc
	this->GetModuleInfo();

	// Create sub directories
	this->CreateSubFolders();

	// Load DLL addresses and interfaces + some sigs
	this->GetLibrarys();
	this->GetInterfaces();

	// Load Netvars
	this->GetNetVars();

	// Resource manager
	g_pResourceManager = new CResourceManager();
	g_pResourceManager->Init(this);
	m_pResourceManager = g_pResourceManager;

	// Create Resources
	m_pResourceManager->CreateTextures();
	m_pResourceManager->CreateFonts();

	// CGui initialization
	m_pGui = CGui::Instance();
	m_pGui->Setup();

	// Setups
	std::vector<IFeature*> features = { &m_ragebot, &m_gunAccuracy, &m_legitbot, &m_triggerbot, &m_resolver, &m_antiAim,
                                        &m_bhop, &m_esp, &m_weaponesp, &m_soundEsp, &m_chams, &m_misc, &m_fakelag,
                                        &m_fakewalk, &m_fakeduck, &m_skinchanger, &m_visuals, &m_materialvisuals,
                                        &m_gunHud, &m_mirror, &m_radar, &m_lagcompensation };
	for (auto& feature : features)
    {
	    feature->Setup();
    }

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, /*game_newmap*/CXorString("\x70\x6A\xE8\xA7\x48\x65\xE0\xB5\x7A\x6A\xF5").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*cs_game_disconnected*/CXorString("\x74\x78\xDA\xA5\x76\x66\xE0\x9D\x73\x62\xF6\xA1\x78\x65\xEB\xA7\x74\x7F\xE0\xA6").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*switch_team*/CXorString("\x64\x7C\xEC\xB6\x74\x63\xDA\xB6\x72\x6A\xE8").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*player_spawned*/CXorString("\x67\x67\xE4\xBB\x72\x79\xDA\xB1\x67\x6A\xF2\xAC\x72\x6F").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*player_hurt*/CXorString("\x67\x67\xE4\xBB\x72\x79\xDA\xAA\x62\x79\xF1").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*player_death*/CXorString("\x67\x67\xE4\xBB\x72\x79\xDA\xA6\x72\x6A\xF1\xAA").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*round_start*/CXorString("\x65\x64\xF0\xAC\x73\x54\xF6\xB6\x76\x79\xF1").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*round_end*/CXorString("\x65\x64\xF0\xAC\x73\x54\xE0\xAC\x73").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*weapon_fire*/CXorString("\x60\x6E\xE4\xB2\x78\x65\xDA\xA4\x7E\x79\xE0").ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, /*bullet_impact*/CXorString("\x75\x7E\xE9\xAE\x72\x7F\xDA\xAB\x7A\x7B\xE4\xA1\x63").ToCharArray(), false);

	// Load config
	CConfig config;
	config.Init(this);
	if (config.LoadFile("default.cfg"))
	{
		ConfigHelper::ConfigToFeatures(&config);
	}
	// <remove once implemented correctly>
	this->m_skinchanger.SetEnabled(true);
	// </remove once implemented correctly>

	// Initialize menu
#ifdef _DEBUG
    g_pConsole->Write(LOGLEVEL_INFO, "Creating menu... ");
#endif
	m_pMenu = CMenu::Instance();
	m_pMenu->Init(this);
	m_pMenu->CreateMenu();
	m_pMenu->ApplySettings();
#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n");
#endif

	if (m_pEngineClient->IsInGame())
	{
		m_gunAccuracy.SetRecoilCompensation(atof(m_pCVar->FindVar(/*weapon_recoil_scale*/CXorString("\x60\x6E\xE4\xB2\x78\x65\xDA\xB0\x72\x68\xEA\xAB\x7B\x54\xF6\xA1\x76\x67\xE0"))->value));
	}
}

void CApplication::Hook()
{
#ifdef _DEBUG
    g_pConsole->Write(LOGLEVEL_INFO, "Hooking engine functions...\n");
#endif

	m_pClientModeHook = new VMTHook((DWORD*)m_pClientMode);
	g_pOverrideView = (OverrideView_t)m_pClientModeHook->Hook(18, (DWORD*)hk_OverrideView);
	g_pCreateMove = (CreateMove_t)m_pClientModeHook->Hook(24, (DWORD*)hk_CreateMove);
	g_pGetViewModelFov = (GetViewModelFov_t)m_pClientModeHook->Hook(35, (DWORD*)hk_GetViewModelFov);

	m_pModelRenderHook = new VMTHook((DWORD*)this->m_pModelRender);
	g_pDrawModelExecute = (DrawModelExecute_t)m_pModelRenderHook->Hook(21, (DWORD*)hk_DrawModelExecute);

	m_pClientHook = new VMTHook((DWORD*)this->m_pClient);
	g_pFrameStageNotify = (FrameStageNotify_t)m_pClientHook->Hook(37, (DWORD*)hk_FrameStageNotify);

	m_pPanelHook = new VMTHook((DWORD*)this->m_pPanel);
	g_pPaintTraverse = (PaintTraverse_t)m_pPanelHook->Hook(41, (DWORD*)hk_PaintTraverse);

	m_pSurfaceHook = new VMTHook((DWORD*)this->m_pSurface);
	g_pPlaySound = (PlaySound_t)m_pSurfaceHook->Hook(82, (DWORD*)hk_PlaySound);
	g_pLockCursor = (LockCursor_t)m_pSurfaceHook->Hook(67, (DWORD*)hk_LockCursor);

	m_pGameEventManagerHook = new VMTHook((DWORD*)this->m_pGameEventManager);
	g_pFireEventClientSide = (FireEventClientSide_t)m_pGameEventManagerHook->Hook(9, (DWORD*)hk_FireEventClientSide);

	m_pViewRenderHook = new VMTHook((DWORD*)m_pViewRender);
	g_pRenderView = (RenderView_t)m_pViewRenderHook->Hook(6, (DWORD*)hk_RenderView);
	g_pRenderSmokeOverlay = (RenderSmokeOverlay_t)m_pViewRenderHook->Hook(41 /*before 40*/, (DWORD*)hk_RenderSmokeOverlay);

	m_pEngineSoundHook = new VMTHook((DWORD*)m_pEngineSound);
	g_pEmitSound1 = (EmitSound1_t)m_pEngineSoundHook->Hook(5, (DWORD*)hk_EmitSound1);
	//g_pEmitSound2 = (EmitSound2_t)m_pEngineSoundHook->Hook(6, (DWORD*)hk_EmitSound2);

	m_pMdlHook = new VMTHook((DWORD*)m_pMdlCache);
	g_pFindMdl = (FindMdl_t)m_pMdlHook->Hook(10, (DWORD*)hk_FindMDL);



	// Proxy functions
	g_pSequenceProxy = m_pNetVarSequence->HookProxy(hk_SetViewModelSequence);
	g_pLowerBodyYawProxy = m_pNetVarLowerBodyYaw->HookProxy(hk_SetLowerBodyYawTarget);
	g_pEyeAnglesProxy0 = m_pNetVarEyeAngles0->HookProxy(hk_SetEyeAnglesProxy0);
	g_pEyeAnglesProxy1 = m_pNetVarEyeAngles1->HookProxy(hk_SetEyeAnglesProxy1);

	//this->m_misc.SetClanTag(".mechanics"); //todo: dynamic!!
	
	this->m_bIsHooked = true;
	this->m_bInitialHookDone = true;

#ifdef _DEBUG
    g_pConsole->Write(LOGLEVEL_INFO, "Done hooking\n");
#endif
}

void CApplication::CreateSubFolders()
{
	this->CreateFolder(CONFIG_FOLDER);
	this->CreateFolder(SKINCHANGER_CONFIG_FOLDER_FULL_RELATIVE);
}

void CApplication::GetModuleInfo()
{
	int iLenFullpath;
	char pFullpath[MAX_PATH];

	iLenFullpath = GetModuleFileName(m_hModule, pFullpath, MAX_PATH);

	for (int i = iLenFullpath - 1; i >= 0; i--)
	{
		if (pFullpath[i] == '\\')
		{
			m_iLenFilepath = i + 1;
			m_pFilepath = new char[m_iLenFilepath + 1];
			strncpy(m_pFilepath, pFullpath, m_iLenFilepath + 1);
			m_pFilepath[m_iLenFilepath] = '\0';

			m_iLenFilename = iLenFullpath - i - 1;
			m_pFilename = new char[m_iLenFilename + 1];
			strcpy(m_pFilename, pFullpath + i + 1);

			break;
		}
	}

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Module path: %s\n", m_pFilepath);
	g_pConsole->Write(LOGLEVEL_INFO, "Module name: %s\n", m_pFilename);
	g_pConsole->WritePlain("\n");
#endif
}

void CApplication::GetLibrarys()
{
	// Grab engine addresses
	this->m_dwTier0Dll = (DWORD)GetModuleHandle(/*tier0.dll*/CXorString("\x63\x62\xE0\xB0\x27\x25\xE1\xAE\x7B"));
	//this->m_dwClientDll = (DWORD)GetModuleHandle(/*client.dll*/CXorString("tgì§y«¦{g"));
	//todo: xorn
	this->m_dwClientDll = (DWORD)GetModuleHandle(/*client_panorama.dll*/CXorString("\x74\x67\xEC\xA7\x79\x7F\xDA\xB2\x76\x65\xEA\xB0\x76\x66\xE4\xEC\x73\x67\xE9"));
	this->m_dwEngineDll = (DWORD)GetModuleHandle(/*engine.dll*/CXorString("\x72\x65\xE2\xAB\x79\x6E\xAB\xA6\x7B\x67"));
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(/*materialsystem.dll*/CXorString("\x7A\x6A\xF1\xA7\x65\x62\xE4\xAE\x64\x72\xF6\xB6\x72\x66\xAB\xA6\x7B\x67"));
	this->m_dwVGui2Dll = (DWORD)GetModuleHandle(/*vgui2.dll*/CXorString("\x61\x6C\xF0\xAB\x25\x25\xE1\xAE\x7B"));
	this->m_dwVGuiSurfaceDll = (DWORD)GetModuleHandle(/*vguimatsurface.dll*/CXorString("\x61\x6C\xF0\xAB\x7A\x6A\xF1\xB1\x62\x79\xE3\xA3\x74\x6E\xAB\xA6\x7B\x67"));
	this->m_dwVPhysicsDll = (DWORD)GetModuleHandle(/*vphysics.dll*/CXorString("\x61\x7B\xED\xBB\x64\x62\xE6\xB1\x39\x6F\xE9\xAE"));
	this->m_dwVStdLibDll = (DWORD)GetModuleHandle(/*vstdlib.dll*/CXorString("\x61\x78\xF1\xA6\x7B\x62\xE7\xEC\x73\x67\xE9"));
	this->m_dwDatacacheDll = (DWORD)GetModuleHandle(/*datacache.dll*/CXorString("\x73\x6A\xF1\xA3\x74\x6A\xE6\xAA\x72\x25\xE1\xAE\x7B"));
	this->m_dwLocalizeDll = (DWORD)GetModuleHandle(/*localize.dll*/CXorString("\x7B\x64\xE6\xA3\x7B\x62\xFF\xA7\x39\x6F\xE9\xAE"));
	this->m_dwInputSystemDll = (DWORD)GetModuleHandle(/*inputsystem.dll*/CXorString("\x7E\x65\xF5\xB7\x63\x78\xFC\xB1\x63\x6E\xE8\xEC\x73\x67\xE9"));

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "tier0.dll\t\t=>\t0x%08X\n", m_dwTier0Dll);
	g_pConsole->Write(LOGLEVEL_INFO, "client.dll\t\t=>\t0x%08X\n", m_dwClientDll);
	g_pConsole->Write(LOGLEVEL_INFO, "engine.dll\t\t=>\t0x%08X\n", m_dwEngineDll);
	g_pConsole->Write(LOGLEVEL_INFO, "materialsystem.dll\t=>\t0x%08X\n", m_dwMaterialSystemDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vgui2.dll\t\t=>\t0x%08X\n", m_dwVGui2Dll);
	g_pConsole->Write(LOGLEVEL_INFO, "vguimatsurface.dll\t=>\t0x%08X\n", m_dwVGuiSurfaceDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vphysics.dll\t\t=>\t0x%08X\n", m_dwVPhysicsDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vstdlib.dll\t\t=>\t0x%08X\n", m_dwVStdLibDll);
	g_pConsole->Write(LOGLEVEL_INFO, "datacache.dll\t\t=>\t0x%08X\n", m_dwDatacacheDll);
	g_pConsole->Write(LOGLEVEL_INFO, "localize.dll\t\t=>\t0x%08X\n", m_dwLocalizeDll);
	g_pConsole->Write(LOGLEVEL_INFO, "inputsystem.dll\t\t=>\t0x%08X\n", m_dwInputSystemDll);
	g_pConsole->WritePlain("\n");
#endif // _DEBUG
}

void CApplication::GetInterfaces()
{
	// Setup strings
	CXorString xorCreateInterface(/*CreateInterface*/"\x54\x79\xE0\xA3\x63\x6E\xCC\xAC\x63\x6E\xF7\xA4\x76\x68\xE0");

	CreateInterfaceFn ClientFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, xorCreateInterface);
	CreateInterfaceFn EngineFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, xorCreateInterface);
	CreateInterfaceFn MaterialSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, xorCreateInterface);
	CreateInterfaceFn VGui2Factory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGui2Dll, xorCreateInterface);
	CreateInterfaceFn VGuiSurfaceFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGuiSurfaceDll, xorCreateInterface);
	CreateInterfaceFn VPhysicsFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVPhysicsDll, xorCreateInterface);
	CreateInterfaceFn VStdLibFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVStdLibDll, xorCreateInterface);
	CreateInterfaceFn DatacacheFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwDatacacheDll, xorCreateInterface);
	CreateInterfaceFn LocalizeFacory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwLocalizeDll, xorCreateInterface);
	CreateInterfaceFn InputSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwInputSystemDll, xorCreateInterface);

	m_pConColorMsg = (ConColorMsg_t)GetProcAddress((HMODULE)this->m_dwTier0Dll, /*?ConColorMsg@@YAXABVColor@@PBDZZ*/CXorString("\x28\x48\xEA\xAC\x54\x64\xE9\xAD\x65\x46\xF6\xA5\x57\x4B\xDC\x83\x4F\x4A\xC7\x94\x54\x64\xE9\xAD\x65\x4B\xC5\x92\x55\x4F\xDF\x98"));

	m_pRandomSeed = (RandomSeed_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomSeed*/CXorString("\x45\x6A\xEB\xA6\x78\x66\xD6\xA7\x72\x6F"));
	m_pRandomInt = (RandomInt_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomInt*/CXorString("\x45\x6A\xEB\xA6\x78\x66\xCC\xAC\x63"));
	m_pRandomFloat = (RandomFloat_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomFloat*/CXorString("\x45\x6A\xEB\xA6\x78\x66\xC3\xAE\x78\x6A\xF1"));

	m_pEngineClient = (IVEngineClient*)EngineFactory(/*VEngineClient014*/CXorString("\x41\x4E\xEB\xA5\x7E\x65\xE0\x81\x7B\x62\xE0\xAC\x63\x3B\xB4\xF6"), nullptr);
	m_pClient = (IBaseClientDLL*)ClientFactory(/*VClient018*/CXorString("\x41\x48\xE9\xAB\x72\x65\xF1\xF2\x26\x33"), nullptr);
	m_pEntityList = (IClientEntityList*)ClientFactory(/*VClientEntityList003*/CXorString("\x41\x48\xE9\xAB\x72\x65\xF1\x87\x79\x7F\xEC\xB6\x6E\x47\xEC\xB1\x63\x3B\xB5\xF1"), nullptr);
	m_pModelInfo = (IVModelInfo*)EngineFactory(/*VModelInfoClient004*/CXorString("\x41\x46\xEA\xA6\x72\x67\xCC\xAC\x71\x64\xC6\xAE\x7E\x6E\xEB\xB6\x27\x3B\xB1"), nullptr);
	m_pModelRender = (IVModelRender*)EngineFactory(/*VEngineModel016*/CXorString("\x41\x4E\xEB\xA5\x7E\x65\xE0\x8F\x78\x6F\xE0\xAE\x27\x3A\xB3"), nullptr);
	m_pRenderView = (IVRenderView*)EngineFactory(/*VEngineRenderView014*/CXorString("\x41\x4E\xEB\xA5\x7E\x65\xE0\x90\x72\x65\xE1\xA7\x65\x5D\xEC\xA7\x60\x3B\xB4\xF6"), nullptr);
	m_pEngineTrace = (IEngineTrace*)EngineFactory(/*EngineTraceClient004*/CXorString("\x52\x65\xE2\xAB\x79\x6E\xD1\xB0\x76\x68\xE0\x81\x7B\x62\xE0\xAC\x63\x3B\xB5\xF6"), nullptr);
	m_pMaterialSystem = (IMaterialSystem*)MaterialSystemFactory(/*VMaterialSystem080*/CXorString("\x41\x46\xE4\xB6\x72\x79\xEC\xA3\x7B\x58\xFC\xB1\x63\x6E\xE8\xF2\x2F\x3B"), nullptr);
	m_pCVar = (ICVar*)VStdLibFactory(/*VEngineCvar007*/CXorString("\x41\x4E\xEB\xA5\x7E\x65\xE0\x81\x61\x6A\xF7\xF2\x27\x3C"), nullptr);
	m_pPanel = (IPanel*)VGui2Factory(/*VGUI_Panel009*/CXorString("\x41\x4C\xD0\x8B\x48\x5B\xE4\xAC\x72\x67\xB5\xF2\x2E"), nullptr);
	m_pSurface = (ISurface*)VGuiSurfaceFactory(/*VGUI_Surface031*/CXorString("\x41\x4C\xD0\x8B\x48\x58\xF0\xB0\x71\x6A\xE6\xA7\x27\x38\xB4"), nullptr);
	m_pGameEventManager = (IGameEventManager2*)EngineFactory(/*GAMEEVENTSMANAGER002*/CXorString("\x50\x4A\xC8\x87\x52\x5D\xC0\x8C\x43\x58\xC8\x83\x59\x4A\xC2\x87\x45\x3B\xB5\xF0"), nullptr);
	m_pPhysicsSurfaceProps = (IPhysicsSurfaceProps*)VPhysicsFactory(/*VPhysicsSurfaceProps001*/CXorString("\x41\x5B\xED\xBB\x64\x62\xE6\xB1\x44\x7E\xF7\xA4\x76\x68\xE0\x92\x65\x64\xF5\xB1\x27\x3B\xB4"), nullptr);
	m_pEngineSound = (IEngineSound*)EngineFactory(/*IEngineSoundClient003*/CXorString("\x5E\x4E\xEB\xA5\x7E\x65\xE0\x91\x78\x7E\xEB\xA6\x54\x67\xEC\xA7\x79\x7F\xB5\xF2\x24"), nullptr);
	m_pMdlCache = (IMDLCache*)DatacacheFactory(/*MDLCache004*/CXorString("\x5A\x4F\xC9\x81\x76\x68\xED\xA7\x27\x3B\xB1"), nullptr);
	m_pLocalize = (ILocalize*)LocalizeFacory(/*Localize_001*/CXorString("\x5B\x64\xE6\xA3\x7B\x62\xFF\xA7\x48\x3B\xB5\xF3"), nullptr);
	m_pInputSystem = (IInputSystem*)InputSystemFactory(/*InputSystemVersion001*/CXorString("\x5E\x65\xF5\xB7\x63\x58\xFC\xB1\x63\x6E\xE8\x94\x72\x79\xF6\xAB\x78\x65\xB5\xF2\x26"), nullptr);

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS); // GlobalVar

	// CInput
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[18] + 0x01);

	// IClientMode
	m_pClientMode = (IClientMode*)(**(DWORD***)((*(DWORD**)(m_pClient))[10] + 0x05));

	// IClientState
	this->m_pClientState = **(IClientState***)(CPattern::FindPattern(
		(BYTE*)this->EngineDll(),
		ENGINEDLL_SIZE,
		(BYTE*)"\xA1\x00\x00\x00\x00\x8B\x80\x00\x00\x00\x00\xC3",
		"a----gh----e"
	) + 0x01);

	// IViewRender
	m_pViewRender = **(IViewRender***)(CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\xFF\x75\x10\x8B\x0D\x00\x00\x00\x00\xFF\x75\x0C",
		"abcdefgh----ijk"
	) + 0x08);

	// KeyValues::Init
	DWORD dwInitKeyValuesTemp = (CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56",
		"a----bbb----ccccccc-----ddd"
	) + 0x07);
	m_pInitKeyValues = (InitKeyValues_t)(dwInitKeyValuesTemp + (*(DWORD_PTR*)(dwInitKeyValuesTemp + 1)) + 0x05);

	// KeyValues::LoadFromBuffer
	DWORD dwLoadFromBufferTemp = CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89\x4C\x24\x04",
		"abcdefghijjjjjjjq"
	);
	m_pLoadFromBuffer = (LoadFromBuffer_t)dwLoadFromBufferTemp;

	// IViewRenderBeams
	m_pViewRenderBeams = *(IViewRenderBeams**)(CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\xB9\x00\x00\x00\x00\xA1\x00\x00\x00\x00\xFF\x10\xA1\x00\x00\x00\x00\xB9",
		"a----l----htg----f"
	) + 1);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "ConColorMsg\t\t=>\t0x%08X\n", m_pConColorMsg);
	g_pConsole->WritePlain("\n");
	
	g_pConsole->Write(LOGLEVEL_INFO, "RandomSeed\t\t=>\t0x%08X\n", m_pRandomSeed);
	g_pConsole->Write(LOGLEVEL_INFO, "RandomInt\t\t=>\t0x%08X\n", m_pRandomInt);
	g_pConsole->Write(LOGLEVEL_INFO, "RandomFloat\t\t=>\t0x%08X\n", m_pRandomFloat);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "VEngineClient014\t\t=>\t0x%08X\n", m_pEngineClient);
	g_pConsole->Write(LOGLEVEL_INFO, "VClient018\t\t=>\t0x%08X\n", m_pClient);
	g_pConsole->Write(LOGLEVEL_INFO, "VClientEntityList003\t=>\t0x%08X\n", m_pEntityList);
	g_pConsole->Write(LOGLEVEL_INFO, "VModelInfoClient004\t=>\t0x%08X\n", m_pModelInfo);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineModel016\t\t=>\t0x%08X\n", m_pModelRender);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineRenderView014\t=>\t0x%08X\n", m_pRenderView);
	g_pConsole->Write(LOGLEVEL_INFO, "EngineTraceClient004\t=>\t0x%08X\n", m_pEngineTrace);
	g_pConsole->Write(LOGLEVEL_INFO, "VMaterialSystem080\t=>\t0x%08X\n", m_pMaterialSystem);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineCvar007\t\t=>\t0x%08X\n", m_pCVar);
	g_pConsole->Write(LOGLEVEL_INFO, "VGUI_Panel009\t\t=>\t0x%08X\n", m_pPanel);
	g_pConsole->Write(LOGLEVEL_INFO, "VGUI_Surface031\t\t=>\t0x%08X\n", m_pSurface);
	g_pConsole->Write(LOGLEVEL_INFO, "GAMEEVENTSMANAGER002\t=>\t0x%08X\n", m_pGameEventManager);
	g_pConsole->Write(LOGLEVEL_INFO, "VPhysicsSurfaceProps001\t=>\t0x%08X\n", m_pPhysicsSurfaceProps);
	g_pConsole->Write(LOGLEVEL_INFO, "IEngineSoundClient003\t=>\t0x%08X\n", m_pEngineSound);
	g_pConsole->Write(LOGLEVEL_INFO, "MDLCache004\t\t=>\t0x%08X\n", m_pMdlCache);
	g_pConsole->Write(LOGLEVEL_INFO, "Localize_001\t\t=>\t0x%08X\n", m_pLocalize);
	g_pConsole->Write(LOGLEVEL_INFO, "InputSystemVersion001\t=>\t0x%08X\n", m_pInputSystem);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "CGlobalVars\t\t=>\t0x%08X\n", m_pGlobalVars);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "CInput\t\t\t=>\t0x%08X\n", m_pInput);
	g_pConsole->Write(LOGLEVEL_INFO, "IClientMode\t\t=>\t0x%08X\n", m_pClientMode);
	g_pConsole->Write(LOGLEVEL_INFO, "IClientState\t\t=>\t0x%08X\n", m_pClientState);
	g_pConsole->Write(LOGLEVEL_INFO, "IViewRender\t\t=>\t0x%08X\n", m_pViewRender);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::Init\t\t=>\t0x%08X\n", m_pInitKeyValues);
	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::LoadFromBuffer=>\t0x%08X\n", m_pLoadFromBuffer);
	g_pConsole->WritePlain("\n");
#endif // _DEBUG
}

void CApplication::GetNetVars()
{
	CBenchmark benchmark(true);

	CXorString xorBaseEntity(/*DT_BaseEntity*/"\x53\x5F\xDA\x80\x76\x78\xE0\x87\x79\x7F\xEC\xB6\x6E");
	CXorString xorBasePlayer(/*DT_BasePlayer*/"\x53\x5F\xDA\x80\x76\x78\xE0\x92\x7B\x6A\xFC\xA7\x65");
	CXorString xorCollisionProperty(/*DT_CollisionProperty*/"\x53\x5F\xDA\x81\x78\x67\xE9\xAB\x64\x62\xEA\xAC\x47\x79\xEA\xB2\x72\x79\xF1\xBB");
	CXorString xorCSPlayer(/*DT_CSPlayer*/"\x53\x5F\xDA\x81\x44\x5B\xE9\xA3\x6E\x6E\xF7");
	CXorString xorLocalPlayerExclusive(/*DT_LocalPlayerExclusive*/"\x53\x5F\xDA\x8E\x78\x68\xE4\xAE\x47\x67\xE4\xBB\x72\x79\xC0\xBA\x74\x67\xF0\xB1\x7E\x7D\xE0");
	CXorString xorBaseCombatWeapon(/*DT_BaseCombatWeapon*/"\x53\x5F\xDA\x80\x76\x78\xE0\x81\x78\x66\xE7\xA3\x63\x5C\xE0\xA3\x67\x64\xEB");
	CXorString xorWeaponCSBase(/*DT_WeaponCSBase*/"\x53\x5F\xDA\x95\x72\x6A\xF5\xAD\x79\x48\xD6\x80\x76\x78\xE0");
	CXorString xorBaseCSGrenade(/*DT_BaseCSGrenade*/"\x53\x5F\xDA\x80\x76\x78\xE0\x81\x44\x4C\xF7\xA7\x79\x6A\xE1\xA7");
	CXorString xorBaseCombatCharacter(/*DT_BaseCombatCharacter*/"\x53\x5F\xDA\x80\x76\x78\xE0\x81\x78\x66\xE7\xA3\x63\x48\xED\xA3\x65\x6A\xE6\xB6\x72\x79");
	CXorString xorBaseViewModel(/*DT_BaseViewModel*/"\x53\x5F\xDA\x80\x76\x78\xE0\x94\x7E\x6E\xF2\x8F\x78\x6F\xE0\xAE");
	CXorString xorPlantedC4(/*DT_PlantedC4*/"\x53\x5F\xDA\x92\x7B\x6A\xEB\xB6\x72\x6F\xC6\xF6");
	CXorString xorBaseAttributableItem(/*DT_BaseAttributableItem*/"\x53\x5F\xDA\x80\x76\x78\xE0\x83\x63\x7F\xF7\xAB\x75\x7E\xF1\xA3\x75\x67\xE0\x8B\x63\x6E\xE8");
	CXorString xorBaseAnimating(/*DT_BaseAnimating*/"\x53\x5F\xDA\x80\x76\x78\xE0\x83\x79\x62\xE8\xA3\x63\x62\xEB\xA5");

	m_pNetVarMgr = new CNetVarManager();
	m_pNetVarMgr->AddTable(xorBaseEntity);
	m_pNetVarMgr->AddTable(xorBasePlayer);
	m_pNetVarMgr->AddTable(xorCSPlayer);
	m_pNetVarMgr->AddTable(xorBaseCombatWeapon);
	m_pNetVarMgr->AddTable(xorWeaponCSBase);
	m_pNetVarMgr->AddTable(xorBaseCSGrenade);
	m_pNetVarMgr->AddTable(xorBaseCombatCharacter);
	m_pNetVarMgr->AddTable(xorBaseViewModel);
	m_pNetVarMgr->AddTable(xorPlantedC4);
	m_pNetVarMgr->AddTable(xorBaseAttributableItem);
	m_pNetVarMgr->AddTable(xorBaseAnimating);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Loading NetVars... ");
#endif

	m_pNetVarMgr->LoadTables(m_pClient->GetAllClasses(), true);

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n");
	g_pConsole->Write(LOGLEVEL_INFO, "Loading Offsets... ");
#endif

	Offsets::m_angRotation = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_angRotation*/CXorString("\x7A\x54\xE4\xAC\x70\x59\xEA\xB6\x76\x7F\xEC\xAD\x79"));
	Offsets::m_nModelIndex = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_nModelIndex*/CXorString("\x7A\x54\xEB\x8F\x78\x6F\xE0\xAE\x5E\x65\xE1\xA7\x6F"));
	Offsets::m_hMyWeapons = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter, /*m_hMyWeapons*/CXorString("\x7A\x54\xED\x8F\x6E\x5C\xE0\xA3\x67\x64\xEB\xB1"));
	Offsets::m_hViewModel = m_pNetVarMgr->GetOffset(xorBasePlayer, /*m_hViewModel[0]*/CXorString(""));
	Offsets::m_flSimulationTime = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_flSimulationTime*/CXorString("\x7A\x54\xE3\xAE\x44\x62\xE8\xB7\x7B\x6A\xF1\xAB\x78\x65\xD1\xAB\x7A\x6E"));
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_vecMins = m_pNetVarMgr->GetOffset(2, xorBaseEntity, xorCollisionProperty.ToCharArray(), /*m_vecMins*/CXorString("\x7A\x54\xF3\xA7\x74\x46\xEC\xAC\x64").ToCharArray());
	Offsets::m_vecMaxs = m_pNetVarMgr->GetOffset(2, xorBaseEntity, xorCollisionProperty.ToCharArray(), /*m_vecMaxs*/CXorString("\x7A\x54\xF3\xA7\x74\x46\xE4\xBA\x64").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_vecOrigin = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_vecOrigin*/CXorString("\x7A\x54\xF3\xA7\x74\x44\xF7\xAB\x70\x62\xEB"));
	Offsets::m_vecViewOffset = m_pNetVarMgr->GetOffset(2, xorBasePlayer, xorLocalPlayerExclusive.ToCharArray(), /*m_vecViewOffset[0]*/CXorString("\x7A\x54\xF3\xA7\x74\x5D\xEC\xA7\x60\x44\xE3\xA4\x64\x6E\xF1\x99\x27\x56").ToCharArray());
	Offsets::m_angEyeAngles = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_angEyeAngles*/CXorString("\x7A\x54\xE4\xAC\x70\x4E\xFC\xA7\x56\x65\xE2\xAE\x72\x78"));

	m_pNetVarEyeAngles0 = m_pNetVarMgr->GetNetVar(xorCSPlayer, /*m_angEyeAngles[0]*/CXorString("\x7A\x54\xE4\xAC\x70\x4E\xFC\xA7\x56\x65\xE2\xAE\x72\x78\xDE\xF2\x4A"));
	m_pNetVarEyeAngles1 = m_pNetVarMgr->GetNetVar(xorCSPlayer, /*m_angEyeAngles[1]*/CXorString("\x7A\x54\xE4\xAC\x70\x4E\xFC\xA7\x56\x65\xE2\xAE\x72\x78\xDE\xF3\x4A"));

	Offsets::m_bHasHeavyArmor = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_bHasHeavyArmor*/CXorString("\x7A\x54\xE7\x8A\x76\x78\xCD\xA7\x76\x7D\xFC\x83\x65\x66\xEA\xB0"));
	m_pNetVarLowerBodyYaw = m_pNetVarMgr->GetNetVar(xorCSPlayer, /*m_flLowerBodyYawTarget*/CXorString("\x7A\x54\xE3\xAE\x5B\x64\xF2\xA7\x65\x49\xEA\xA6\x6E\x52\xE4\xB5\x43\x6A\xF7\xA5\x72\x7F"));
	Offsets::m_flLowerBodyYawTarget = m_pNetVarLowerBodyYaw->GetOffset();
	Offsets::m_vecVelocity = m_pNetVarMgr->GetOffset(2, xorBasePlayer, xorLocalPlayerExclusive.ToCharArray(), /*m_vecVelocity[0]*/CXorString("\x7A\x54\xF3\xA7\x74\x5D\xE0\xAE\x78\x68\xEC\xB6\x6E\x50\xB5\x9F").ToCharArray());
	Offsets::m_iTeamNum = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_iTeamNum*/CXorString("\x7A\x54\xEC\x96\x72\x6A\xE8\x8C\x62\x66"));
	Offsets::m_lifeState = m_pNetVarMgr->GetOffset(xorBasePlayer, /*m_lifeState*/CXorString("\x7A\x54\xE9\xAB\x71\x6E\xD6\xB6\x76\x7F\xE0"));
	Offsets::m_iHealth = m_pNetVarMgr->GetOffset(xorBasePlayer, /*m_iHealth*/CXorString("\x7A\x54\xEC\x8A\x72\x6A\xE9\xB6\x7F"));
	Offsets::m_ArmorValue = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_ArmorValue*/CXorString("\x7A\x54\xC4\xB0\x7A\x64\xF7\x94\x76\x67\xF0\xA7"));
	Offsets::m_bHasHelmet = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_bHasHelmet*/CXorString("\x7A\x54\xE7\x8A\x76\x78\xCD\xA7\x7B\x66\xE0\xB6"));
	Offsets::m_fFlags = m_pNetVarMgr->GetOffset(xorBasePlayer, /*m_fFlags*/CXorString("\x7A\x54\xE3\x84\x7B\x6A\xE2\xB1"));
	Offsets::m_bSpotted = m_pNetVarMgr->GetOffset(xorBaseEntity, /*m_bSpotted*/CXorString("\x7A\x54\xE7\x91\x67\x64\xF1\xB6\x72\x6F"));
	Offsets::m_bGunGameImmunity = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_bGunGameImmunity*/CXorString("\x7A\x54\xE7\x85\x62\x65\xC2\xA3\x7A\x6E\xCC\xAF\x7A\x7E\xEB\xAB\x63\x72"));
	Offsets::m_flFlashDuration = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_flFlashDuration*/CXorString("\x7A\x54\xE3\xAE\x51\x67\xE4\xB1\x7F\x4F\xF0\xB0\x76\x7F\xEC\xAD\x79"));
	Offsets::m_flFlashMaxAlpha = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_flFlashMaxAlpha*/CXorString("\x7A\x54\xE3\xAE\x51\x67\xE4\xB1\x7F\x46\xE4\xBA\x56\x67\xF5\xAA\x76"));
	//Offsets::m_bMoveType = m_pNetVarMgr->GetOffset("DT_BaseEntity", "m_bMoveType"); <= Not existant :/
	Offsets::m_bIsScoped = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_bIsScoped*/CXorString("\x7A\x54\xE7\x8B\x64\x58\xE6\xAD\x67\x6E\xE1"));
	Offsets::m_iShotsFired = m_pNetVarMgr->GetOffset(2, xorCSPlayer,
		/*DT_CSLocalPlayerExclusive*/CXorString("\x53\x5F\xDA\x81\x44\x47\xEA\xA1\x76\x67\xD5\xAE\x76\x72\xE0\xB0\x52\x73\xE6\xAE\x62\x78\xEC\xB4\x72").ToCharArray(),
		/*m_iShotsFired*/CXorString("\x7A\x54\xEC\x91\x7F\x64\xF1\xB1\x51\x62\xF7\xA7\x73").ToCharArray());
	Offsets::m_hActiveWeapon = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter, /*m_hActiveWeapon*/CXorString("\x7A\x54\xED\x83\x74\x7F\xEC\xB4\x72\x5C\xE0\xA3\x67\x64\xEB"));
	Offsets::m_hObserverTarget = m_pNetVarMgr->GetOffset(xorBasePlayer, /*m_hObserverTarget*/CXorString("\x7A\x54\xED\x8D\x75\x78\xE0\xB0\x61\x6E\xF7\x96\x76\x79\xE2\xA7\x63"));
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::deadflag = m_pNetVarMgr->GetOffset(2, xorBasePlayer,
		/*DT_PlayerState*/CXorString("\x53\x5F\xDA\x92\x7B\x6A\xFC\xA7\x65\x58\xF1\xA3\x63\x6E").ToCharArray(),
		/*deadflag*/CXorString("\x73\x6E\xE4\xA6\x71\x67\xE4\xA5").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_nTickBase = m_pNetVarMgr->GetOffset(2, xorBasePlayer, xorLocalPlayerExclusive.ToCharArray(), /*m_nTickBase*/CXorString("\x7A\x54\xEB\x96\x7E\x68\xEE\x80\x76\x78\xE0").ToCharArray());
	Offsets::m_bIsDefusing = m_pNetVarMgr->GetOffset(xorCSPlayer, /*m_bIsDefusing*/CXorString("\x7A\x54\xE7\x8B\x64\x4F\xE0\xA4\x62\x78\xEC\xAC\x70"));
	Offsets::m_flC4Blow = m_pNetVarMgr->GetOffset(xorPlantedC4, /*m_flC4Blow*/CXorString("\x7A\x54\xE3\xAE\x54\x3F\xC7\xAE\x78\x7C"));
	Offsets::m_flDefuseCountDown = m_pNetVarMgr->GetOffset(xorPlantedC4, /*m_flDefuseCountDown*/CXorString("\x7A\x54\xE3\xAE\x53\x6E\xE3\xB7\x64\x6E\xC6\xAD\x62\x65\xF1\x86\x78\x7C\xEB"));
	Offsets::m_hBombDefuser = m_pNetVarMgr->GetOffset(xorPlantedC4, /*m_hBombDefuser*/CXorString("\x7A\x54\xED\x80\x78\x66\xE7\x86\x72\x6D\xF0\xB1\x72\x79"));
	Offsets::m_flDuckSpeed = m_pNetVarMgr->GetOffset(xorBasePlayer, CXorString(/*m_flDuckSpeed*/"\x7A\x54\xE3\xAE\x53\x7E\xE6\xA9\x44\x7B\xE0\xA7\x73"));
    Offsets::m_flDuckAmount = m_pNetVarMgr->GetOffset(xorBasePlayer, CXorString(/*m_flDuckAmount*/"\x7A\x54\xE3\xAE\x53\x7E\xE6\xA9\x56\x66\xEA\xB7\x79\x7F"));

	CNetVar* pDtLocal = m_pNetVarMgr->GetNetVar(2, xorBasePlayer, xorLocalPlayerExclusive.ToCharArray(), /*DT_Local*/CXorString("\x53\x5F\xDA\x8E\x78\x68\xE4\xAE").ToCharArray());
	Offsets::m_nJumpTimeMsecs = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_nJumpTimeMsecs*/CXorString("\x7A\x54\xEB\x88\x62\x66\xF5\x96\x7E\x66\xE0\x8F\x64\x6E\xE6\xB1"))->GetOffset();
	Offsets::m_flFallVelocity = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_flFallVelocity*/CXorString("\x7A\x54\xE3\xAE\x51\x6A\xE9\xAE\x41\x6E\xE9\xAD\x74\x62\xF1\xBB"))->GetOffset();
	Offsets::m_viewPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_viewPunchAngle*/CXorString("\x7A\x54\xF3\xAB\x72\x7C\xD5\xB7\x79\x68\xED\x83\x79\x6C\xE9\xA7"))->GetOffset();
	Offsets::m_aimPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngle*/CXorString("\x7A\x54\xE4\xAB\x7A\x5B\xF0\xAC\x74\x63\xC4\xAC\x70\x67\xE0"))->GetOffset();
	Offsets::m_aimPunchAngleVel = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngleVel*/CXorString("\x7A\x54\xE4\xAB\x7A\x5B\xF0\xAC\x74\x63\xC4\xAC\x70\x67\xE0\x94\x72\x67"))->GetOffset();

	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_iItemDefinitionIndex = m_pNetVarMgr->GetOffset(4, xorBaseCombatWeapon,
		/*DT_EconEntity*/CXorString("\x53\x5F\xDA\x87\x74\x64\xEB\x87\x79\x7F\xEC\xB6\x6E").ToCharArray(),
		/*DT_AttributeContainer*/CXorString("\x53\x5F\xDA\x83\x63\x7F\xF7\xAB\x75\x7E\xF1\xA7\x54\x64\xEB\xB6\x76\x62\xEB\xA7\x65").ToCharArray(),
		/*DT_ScriptCreatedItem*/CXorString("\x53\x5F\xDA\x91\x74\x79\xEC\xB2\x63\x48\xF7\xA7\x76\x7F\xE0\xA6\x5E\x7F\xE0\xAF").ToCharArray(),
		/*m_iItemDefinitionIndex*/CXorString("\x7A\x54\xEC\x8B\x63\x6E\xE8\x86\x72\x6D\xEC\xAC\x7E\x7F\xEC\xAD\x79\x42\xEB\xA6\x72\x73").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon, /*m_hOwner*/CXorString("\x7A\x54\xED\x8D\x60\x65\xE0\xB0"));
	Offsets::m_iClip1 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon, /*m_iClip1*/CXorString("\x7A\x54\xEC\x81\x7B\x62\xF5\xF3"));
	Offsets::m_iClip2 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon, /*m_iClip2*/CXorString("\x7A\x54\xEC\x81\x7B\x62\xF5\xF0"));
	Offsets::m_flNextPrimaryAttack = m_pNetVarMgr->GetOffset(2, xorBaseCombatWeapon,
		/*DT_LocalActiveWeaponData*/CXorString("\x53\x5F\xDA\x8E\x78\x68\xE4\xAE\x56\x68\xF1\xAB\x61\x6E\xD2\xA7\x76\x7B\xEA\xAC\x53\x6A\xF1\xA3").ToCharArray(),
		/*m_flNextPrimaryAttack*/CXorString("\x7A\x54\xE3\xAE\x59\x6E\xFD\xB6\x47\x79\xEC\xAF\x76\x79\xFC\x83\x63\x7F\xE4\xA1\x7C").ToCharArray());
	Offsets::m_hWeaponWorldModel = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon, /*m_hWeaponWorldModel*/CXorString("\x7A\x54\xED\x95\x72\x6A\xF5\xAD\x79\x5C\xEA\xB0\x7B\x6F\xC8\xAD\x73\x6E\xE9"));

	Offsets::m_fAccuracyPenalty = m_pNetVarMgr->GetOffset(xorWeaponCSBase, /*m_fAccuracyPenalty*/CXorString("\x7A\x54\xE3\x83\x74\x68\xF0\xB0\x76\x68\xFC\x92\x72\x65\xE4\xAE\x63\x72"));
	Offsets::m_flPostponeFireReadyTime = m_pNetVarMgr->GetOffset(xorWeaponCSBase, /*m_flPostponeFireReadyTime*/CXorString("\x7A\x54\xE3\xAE\x47\x64\xF6\xB6\x67\x64\xEB\xA7\x51\x62\xF7\xA7\x45\x6E\xE4\xA6\x6E\x5F\xEC\xAF\x72"));

	Offsets::m_fThrowTime = m_pNetVarMgr->GetOffset(xorBaseCSGrenade, /*m_fThrowTime*/CXorString("\x7A\x54\xE3\x96\x7F\x79\xEA\xB5\x43\x62\xE8\xA7"));

	CNetVar* pDtAttributeManager = m_pNetVarMgr->GetNetVar(xorBaseAttributableItem, /*DT_AttributeContainer*/CXorString("\x53\x5F\xDA\x83\x63\x7F\xF7\xAB\x75\x7E\xF1\xA7\x54\x64\xEB\xB6\x76\x62\xEB\xA7\x65"));
	CNetVar* pDtItem = pDtAttributeManager->GetChild(/*DT_ScriptCreatedItem*/CXorString("\x53\x5F\xDA\x91\x74\x79\xEC\xB2\x63\x48\xF7\xA7\x76\x7F\xE0\xA6\x5E\x7F\xE0\xAF"));
	int iOffset = pDtAttributeManager->GetOffset() + pDtItem->GetOffset();

	Offsets::m_iItemIDHigh = iOffset + pDtItem->GetChild(/*m_iItemIDHigh*/CXorString("\x7A\x54\xEC\x8B\x63\x6E\xE8\x8B\x53\x43\xEC\xA5\x7F"))->GetOffset();
	Offsets::m_iItemIDLow = iOffset + pDtItem->GetChild(/*m_iItemIDLow*/CXorString("\x7A\x54\xEC\x8B\x63\x6E\xE8\x8B\x53\x47\xEA\xB5"))->GetOffset();
	Offsets::m_iAccountID = iOffset + pDtItem->GetChild(/*m_iAccountID*/CXorString("\x7A\x54\xEC\x83\x74\x68\xEA\xB7\x79\x7F\xCC\x86"))->GetOffset();
	Offsets::m_iEntityQuality = iOffset + pDtItem->GetChild(/*m_iEntityQuality*/CXorString("\x7A\x54\xEC\x87\x79\x7F\xEC\xB6\x6E\x5A\xF0\xA3\x7B\x62\xF1\xBB"))->GetOffset();
	Offsets::m_szCustomName = iOffset + pDtItem->GetChild(/*m_szCustomName*/CXorString("\x7A\x54\xF6\xB8\x54\x7E\xF6\xB6\x78\x66\xCB\xA3\x7A\x6E"))->GetOffset();

	Offsets::m_OriginalOwnerXuidLow = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_OriginalOwnerXuidLow*/CXorString("\x7A\x54\xCA\xB0\x7E\x6C\xEC\xAC\x76\x67\xCA\xB5\x79\x6E\xF7\x9A\x62\x62\xE1\x8E\x78\x7C"));
	Offsets::m_OriginalOwnerXuidHigh = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_OriginalOwnerXuidHigh*/CXorString("\x7A\x54\xCA\xB0\x7E\x6C\xEC\xAC\x76\x67\xCA\xB5\x79\x6E\xF7\x9A\x62\x62\xE1\x8A\x7E\x6C\xED"));
	Offsets::m_nFallbackPaintKit = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_nFallbackPaintKit*/CXorString("\x7A\x54\xEB\x84\x76\x67\xE9\xA0\x76\x68\xEE\x92\x76\x62\xEB\xB6\x5C\x62\xF1"));
	Offsets::m_nFallbackSeed = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_nFallbackSeed*/CXorString("\x7A\x54\xEB\x84\x76\x67\xE9\xA0\x76\x68\xEE\x91\x72\x6E\xE1"));
	Offsets::m_flFallbackWear = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_flFallbackWear*/CXorString("\x7A\x54\xE3\xAE\x51\x6A\xE9\xAE\x75\x6A\xE6\xA9\x40\x6E\xE4\xB0"));
	Offsets::m_nFallbackStatTrak = m_pNetVarMgr->GetOffset(xorBaseAttributableItem, /*m_nFallbackStatTrak*/CXorString("\x7A\x54\xEB\x84\x76\x67\xE9\xA0\x76\x68\xEE\x91\x63\x6A\xF1\x96\x65\x6A\xEE"));

	Offsets::DT_BaseViewModel::m_hWeapon = m_pNetVarMgr->GetOffset(xorBaseViewModel, /*m_hWeapon*/CXorString("\x7A\x54\xED\x95\x72\x6A\xF5\xAD\x79"));
	Offsets::DT_BaseViewModel::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseViewModel, /*m_hOwner*/CXorString("\x7A\x54\xED\x8D\x60\x65\xE0\xB0"));

	Offsets::m_nSequence = m_pNetVarMgr->GetOffset(xorBaseAnimating, /*m_nSequence*/CXorString("\x7A\x54\xEB\x91\x72\x7A\xF0\xA7\x79\x68\xE0"));
	Offsets::m_flPoseParameter = m_pNetVarMgr->GetOffset(xorBaseAnimating, /*m_flPoseParameter*/CXorString("\x7A\x54\xE3\xAE\x47\x64\xF6\xA7\x47\x6A\xF7\xA3\x7A\x6E\xF1\xA7\x65"));
	Offsets::m_flCycle = m_pNetVarMgr->GetOffset(2, xorBaseAnimating,
            /*DT_ServerAnimationData*/CXorString("\x53\x5F\xDA\x91\x72\x79\xF3\xA7\x65\x4A\xEB\xAB\x7A\x6A\xF1\xAB\x78\x65\xC1\xA3\x63\x6A").ToCharArray(),
            /*m_flCycle*/CXorString("\x7A\x54\xE3\xAE\x54\x72\xE6\xAE\x72").ToCharArray());

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n\n");
#endif

	// Grab NetVars later required for hooking
	m_pNetVarSequence = m_pNetVarMgr->GetNetVar(xorBaseViewModel, /*m_nSequence*/CXorString("\x7A\x54\xEB\x91\x72\x7A\xF0\xA7\x79\x68\xE0"));

	m_misc.SetReadyCallback((IsReadyCallback_t)(CPattern::FindPattern(
		(BYTE*)this->ClientDll(),
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE",
		"ghdrtzuigkog----trg"
	)));

	benchmark.FinishBenchmark();
	benchmark.PrintBenchmark("Get Networked Variables");
}

void CorrectMovement(CUserCmd* pCmd, QAngle& qOrigAngles)
{
	CApplication* pApp = CApplication::Instance();

	Vector vecViewForward, vecViewRight, vecViewUp, vecAimForward, vecAimRight, vecAimUp;
	QAngle qViewAngles, qAimAngles;

	pApp->EngineClient()->GetViewAngles(qViewAngles);

	float flForward = pCmd->forwardmove;
	float flRight = pCmd->sidemove;
	float flUp = pCmd->upmove;

	qViewAngles = QAngle(0.0f, qViewAngles.y, 0.0f);
	qAimAngles = QAngle(0.0f, pCmd->viewangles[1], 0.0f);

	AngleVectors(qViewAngles, &vecViewForward, &vecViewRight, &vecViewUp);
	AngleVectors(qAimAngles, &vecAimForward, &vecAimRight, &vecAimUp);

	vecViewForward.NormalizeAngles();
	vecViewRight.NormalizeAngles();
	vecViewUp.NormalizeAngles();

	Vector vecForwardNorm = vecViewForward * flForward;
	Vector vecRightNorm = vecViewRight * flRight;
	Vector vecUpNorm = vecViewUp * flUp;

	pCmd->forwardmove = vecForwardNorm.Dot(vecAimForward) + vecRightNorm.Dot(vecAimForward) + vecUpNorm.Dot(vecAimForward);
	pCmd->sidemove = vecForwardNorm.Dot(vecAimRight) + vecRightNorm.Dot(vecAimRight) + vecUpNorm.Dot(vecAimRight);
	pCmd->upmove = vecForwardNorm.Dot(vecAimUp) + vecRightNorm.Dot(vecAimUp) + vecUpNorm.Dot(vecAimUp);

	ClampMovement(pCmd);
}

void NormalizeAngles(CUserCmd* pUserCmd)
{
	// Normalize pitch
	while (pUserCmd->viewangles[0] > 89.0f)
	{
		pUserCmd->viewangles[0] -= 178.0f;
	}
	while (pUserCmd->viewangles[0] < -89.0f)
	{
		pUserCmd->viewangles[0] += 178.0f;
	}

	// Normalize yaw
	while (pUserCmd->viewangles[1] > 179.9999f)
	{
		pUserCmd->viewangles[1] -= 360.0f;
	}
	while (pUserCmd->viewangles[1] < -179.9999f)
	{
		pUserCmd->viewangles[1] += 360.0f;
	}

	pUserCmd->viewangles[2] = 0.0f;
}

void ClampMovement(CUserCmd* pUserCmd)
{
	// Clamp forward
	if (pUserCmd->forwardmove > 450.0f)
	{
		pUserCmd->forwardmove = 450.0f;
	}
	else if (pUserCmd->forwardmove < -450.0f)
	{
		pUserCmd->forwardmove = -450.0f;
	}

	// Clamp sidemove
	if (pUserCmd->sidemove > 450.0f)
	{
		pUserCmd->sidemove = 450.0f;
	}
	else if (pUserCmd->sidemove < -450.0f)
	{
		pUserCmd->sidemove = -450.0f;
	}

	// Clamp upmove
	if (pUserCmd->sidemove > 320.0f)
	{
		pUserCmd->sidemove = 320.0f;
	}
	else if (pUserCmd->sidemove < -320.0f)
	{
		pUserCmd->sidemove = -320.0f;
	}
}

DWORD __stdcall ThreadFreeLibrary(void* pParam)
{
	// Wait 1 sec for hooks to finish running
	Sleep(1000);
	FreeLibraryAndExitThread((HMODULE)pParam, 0);

	return 0;
}