#include "Application.h"

std::vector<BulletTracerEntry> CApplication::m_pBulletTracer;
std::vector<HitmarkerEntry> CApplication::m_pHitmarker;

CApplication::CApplication()
{
	// TODO: Der konstruktor muss *ALLE* pointer auf NULL setzen, der destruktor
	//		 alle ptr != NULL l�schen
	m_pNetVarMgr = NULL;

	m_pClientModeHook = NULL;
	m_pModelRenderHook = NULL;
	m_pClientHook = NULL;
	m_pPanelHook = NULL;
	m_pSurfaceHook = NULL;
	m_pGameEventManagerHook = NULL;
	m_pViewRenderHook = NULL;
	m_pEngineSoundHook = NULL;
	m_pMdlHook = NULL;

	m_bGotSendPackets = false;

	m_bInitialHookDone = false;
	m_bIsHooked = false;
}

CApplication::~CApplication()
{
	if (m_pMdlHook)
		delete m_pMdlHook;

	if (m_pEngineSoundHook)
		delete m_pEngineSoundHook;

	if (m_pViewRenderHook)
		delete m_pViewRenderHook;

	if (m_pGameEventManagerHook)
		delete m_pGameEventManagerHook;

	if (m_pSurfaceHook)
		delete m_pSurfaceHook;

	if (m_pPanelHook)
		delete m_pPanelHook;

	if (m_pClientHook)
		delete m_pClientHook;

	if (m_pModelRenderHook)
		delete m_pModelRenderHook;

	if (m_pClientModeHook)
		delete m_pClientModeHook;

	if (m_pNetVarMgr)
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
	g_pResourceManager = NULL;

#ifdef _DEBUG
	// Free console
	delete g_pConsole;
	g_pConsole = NULL;
#endif

	// TODO: Stimmt, das andere ist ja nicht von uns.. Cleanups *M�SSEN* immer andersrum passieren als das initialisieren
	// ----------- Our stuff -----------

	// Thirdperson
	if (m_pInput->m_fCameraInThirdPerson)
	{
		m_pInput->m_fCameraInThirdPerson = false;
	}

	// FovChanger
	if (m_visuals.GetFovChangeScoped())
	{
		ConVar* pZoomSensitivity = m_pCVar->FindVar(CXorString("md�Hx�db�ab�Hy�~dگx~��").ToCharArray());
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
		m_misc.SetClanTag(NULL);
		m_misc.ApplyClanTag();
	}

	// MaterialVisuals
	m_materialvisuals.Nightmode(100);
	m_materialvisuals.Asuswalls(100);
	m_materialvisuals.Skychanger(0);

	// Free & Exit
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFreeLibrary, this->m_hModule, NULL, NULL);
}

bool CApplication::CreateFolder(const char* pName)
{
	char pBuffer[MAX_PATH];
	snprintf(pBuffer, MAX_PATH, "%s%s", this->GetWorkingDirectory(), pName);

	if (!CreateDirectory(pBuffer, NULL))
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

	// Resource manager
	g_pResourceManager = new CResourceManager();
	g_pResourceManager->Init(this);
	m_pResourceManager = g_pResourceManager;

	// Create Resources
	m_pResourceManager->CreateTextures();
	m_pResourceManager->CreateFonts();

	// Load Netvars
	this->GetNetVars();

	// CGui initialization
	m_pGui = CGui::Instance();
	m_pGui->Setup();

	// Setups
	this->m_targetSelector.Setup(this);
	this->m_ragebot.Setup();
	this->m_gunAccuracy.Setup();
	this->m_legitbot.Setup();
	this->m_triggerbot.Setup();
	this->m_resolver.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_weaponesp.Setup();
	this->m_soundEsp.Setup();
	this->m_chams.Setup();
	this->m_misc.Setup();
	this->m_fakelag.Setup();
	this->m_fakewalk.Setup();
	this->m_skinchanger.Setup();
	this->m_visuals.Setup();
	this->m_materialvisuals.Setup();
	this->m_gunHud.Setup();
	this->m_mirror.Setup();
	this->m_radar.Setup();
	this->m_lagcompensation.Setup();

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("pj�He�zj�").ToCharArray(), false); // game_newmap
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("txڥvf��sb��xe�t�").ToCharArray(), false); // cs_game_disconnected
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("d|�tcڶrj�").ToCharArray(), false); // switch_team
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg�ryڱgj�ro").ToCharArray(), false); // player_spawned
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg�ryڪby�").ToCharArray(), false); // player_hurt
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg�ryڦrj�").ToCharArray(), false); // player_death
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed�sT��vy�").ToCharArray(), false); // round_start
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed�sT�s").ToCharArray(), false); // round_end
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("`n�xeڤ~y�").ToCharArray(), false); // weapon_fire
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("u~�rګz{�c").ToCharArray(), false); // bullet_impact

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
	m_pMenu = CMenu::Instance();
	m_pMenu->Init(this);
	m_pMenu->CreateMenu();
	m_pMenu->ApplySettings();

	if (m_pEngineClient->IsInGame())
	{
		m_gunAccuracy.SetRecoilCompensation(atof(m_pCVar->FindVar(/*weapon_recoil_scale*/CXorString("`n�xeڰrh�{T��vg�"))->value));
	}
}

void CApplication::Hook()
{
	m_pClientModeHook = new VMTHook((DWORD*)m_pClientMode);
	g_pOverrideView = (OverrideView_t)m_pClientModeHook->Hook(18, (DWORD*)hk_OverrideView);
	g_pCreateMove = (CreateMove_t)m_pClientModeHook->Hook(24, (DWORD*)hk_CreateMove);
	g_pGetViewModelFov = (GetViewModelFov_t)m_pClientModeHook->Hook(35, (DWORD*)hk_GetViewModelFov);

	m_pModelRenderHook = new VMTHook((DWORD*)this->m_pModelRender);
	g_pDrawModelExecute = (DrawModelExecute_t)m_pModelRenderHook->Hook(21, (DWORD*)hk_DrawModelExecute);

	m_pClientHook = new VMTHook((DWORD*)this->m_pClient);
	g_pFrameStageNotify = (FrameStageNotify_t)m_pClientHook->Hook(36, (DWORD*)hk_FrameStageNotify);

	m_pPanelHook = new VMTHook((DWORD*)this->m_pPanel);
	g_pPaintTraverse = (PaintTraverse_t)m_pPanelHook->Hook(41, (DWORD*)hk_PaintTraverse);

	m_pSurfaceHook = new VMTHook((DWORD*)this->m_pSurface);
	g_pPlaySound = (PlaySound_t)m_pSurfaceHook->Hook(82, (DWORD*)hk_PlaySound);

	m_pGameEventManagerHook = new VMTHook((DWORD*)this->m_pGameEventManager);
	g_pFireEventClientSide = (FireEventClientSide_t)m_pGameEventManagerHook->Hook(9, (DWORD*)hk_FireEventClientSide);

	m_pViewRenderHook = new VMTHook((DWORD*)m_pViewRender);
	g_pRenderView = (RenderView_t)m_pViewRenderHook->Hook(6, (DWORD*)hk_RenderView);
	g_pRenderSmokeOverlay = (RenderSmokeOverlay_t)m_pViewRenderHook->Hook(40, (DWORD*)hk_RenderSmokeOverlay);

	m_pEngineSoundHook = new VMTHook((DWORD*)m_pEngineSound);
	g_pEmitSound1 = (EmitSound1_t)m_pEngineSoundHook->Hook(5, (DWORD*)hk_EmitSound1);
	g_pEmitSound2 = (EmitSound2_t)m_pEngineSoundHook->Hook(6, (DWORD*)hk_EmitSound2);

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
	this->m_dwClientDll = (DWORD)GetModuleHandle(/*client.dll*/CXorString("tg�y��{g").ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(/*engine.dll*/CXorString("re�yn��{g").ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(/*materialsystem.dll*/CXorString("zj�eb�dr��rf��{g").ToCharArray());
	this->m_dwVGui2Dll = (DWORD)GetModuleHandle(/*vgui2.dll*/CXorString("al�%%�{").ToCharArray());
	this->m_dwVGuiSurfaceDll = (DWORD)GetModuleHandle(/*vguimatsurface.dll*/CXorString("al�zj�by�tn��{g").ToCharArray());
	this->m_dwVPhysicsDll = (DWORD)GetModuleHandle(/*vphysics.dll*/CXorString("a{�db�9o�").ToCharArray());
	this->m_dwVStdLibDll = (DWORD)GetModuleHandle(/*vstdlib.dll*/CXorString("ax�{b��sg�").ToCharArray());
	this->m_dwDatacacheDll = (DWORD)GetModuleHandle(/*datacache.dll*/CXorString("sj�tj�r%�{").ToCharArray());
	this->m_dwLocalizeDll = (DWORD)GetModuleHandle(/*localize.dll*/CXorString("{d�{b��9o�").ToCharArray());
	//this->m_dwInputSystemDll = (DWORD)GetModuleHandle(/*inputsystem.dll*/"inputsystem.dll");

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "client.dll\t\t=>\t0x%08X\n", m_dwClientDll);
	g_pConsole->Write(LOGLEVEL_INFO, "engine.dll\t\t=>\t0x%08X\n", m_dwEngineDll);
	g_pConsole->Write(LOGLEVEL_INFO, "materialsystem.dll\t=>\t0x%08X\n", m_dwMaterialSystemDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vgui2.dll\t\t=>\t0x%08X\n", m_dwVGui2Dll);
	g_pConsole->Write(LOGLEVEL_INFO, "vguimatsurface.dll\t=>\t0x%08X\n", m_dwVGuiSurfaceDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vphysics.dll\t\t=>\t0x%08X\n", m_dwVPhysicsDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vstdlib.dll\t\t=>\t0x%08X\n", m_dwVStdLibDll);
	g_pConsole->Write(LOGLEVEL_INFO, "datacache.dll\t\t=>\t0x%08X\n", m_dwDatacacheDll);
	g_pConsole->Write(LOGLEVEL_INFO, "localize.dll\t\t=>\t0x%08X\n", m_dwLocalizeDll);
	g_pConsole->WritePlain("\n");
#endif // _DEBUG
}

void CApplication::GetInterfaces()
{
	// Setup strings
	CXorString xorCreateInterface("Ty�cn̬cn��vh�");

	CreateInterfaceFn ClientFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn EngineFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn MaterialSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VGui2Factory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGui2Dll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VGuiSurfaceFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGuiSurfaceDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VPhysicsFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVPhysicsDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VStdLibFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVStdLibDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn DatacacheFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwDatacacheDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn LocalizeFacory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwLocalizeDll, xorCreateInterface.ToCharArray());
	//CreateInterfaceFn InputSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwInputSystemDll, xorCreateInterface.ToCharArray());

	m_pRandomSeed = (RandomSeed_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomSeed*/CXorString("Ej�xf֧ro").ToCharArray());
	m_pRandomInt = (RandomInt_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomInt*/CXorString("Ej�xf̬c").ToCharArray());
	m_pRandomFloat = (RandomFloat_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomFloat*/CXorString("Ej�xfîxj�").ToCharArray());

	m_pEngineClient = (IVEngineClient*)EngineFactory(/*VEngineClient014*/CXorString("AN�~e��{b�c;��").ToCharArray(), NULL);
	m_pClient = (IBaseClientDLL*)ClientFactory(/*VClient018*/CXorString("AH�re��&3").ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)ClientFactory(/*VClientEntityList003*/CXorString("AH�re�y�nG�c;��").ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)EngineFactory(/*VModelInfoClient004*/CXorString("AF�rg̬qdƮ~n�';�").ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)EngineFactory(/*VEngineModel016*/CXorString("AN�~e��xo�':�").ToCharArray(), NULL);
	m_pRenderView = (IVRenderView*)EngineFactory(/*VEngineRenderView014*/CXorString("AN�~e��re�e]�`;��").ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)EngineFactory(/*EngineTraceClient004*/CXorString("Re�ynѰvh��{b�c;��").ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)MaterialSystemFactory(/*VMaterialSystem080*/CXorString("AF�ry�{X��cn��/;").ToCharArray(), NULL);
	m_pCVar = (ICVar*)VStdLibFactory(/*VEngineCvar007*/CXorString("AN�~e��aj��'<").ToCharArray(), NULL);
	m_pPanel = (IPanel*)VGui2Factory(/*VGUI_Panel009*/CXorString("ALЋH[�rg��.").ToCharArray(), NULL);
	m_pSurface = (ISurface*)VGuiSurfaceFactory(/*VGUI_Surface031*/CXorString("ALЋHX�qj�'8�").ToCharArray(), NULL);
	m_pGameEventManager = (IGameEventManager2*)EngineFactory(/*GAMEEVENTSMANAGER002*/CXorString("PJȇR]��CXȃYJE;��").ToCharArray(), NULL);
	m_pPhysicsSurfaceProps = (IPhysicsSurfaceProps*)VPhysicsFactory(/*VPhysicsSurfaceProps001*/CXorString("A[�db�D~��vh��ed��';�").ToCharArray(), NULL);
	m_pEngineSound = (IEngineSound*)EngineFactory(/*IEngineSoundClient003*/CXorString("^N�~e��x~�Tg�y��$").ToCharArray(), NULL);
	m_pMdlCache = (IMDLCache*)DatacacheFactory(/*MDLCache004*/CXorString("ZOɁvh�';�").ToCharArray(), NULL);
	m_pLocalize = (ILocalize*)LocalizeFacory(/*Localize_001*/CXorString("[d�{b��H;��").ToCharArray(), NULL);
	//m_pInputSystem = (IInputSystem*)InputSystemFactory(/*InputSystemVersion001*/"InputSystemVersion001", NULL);

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS); // GlobalVar

	// CInput
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[15] + 0x01);

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
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "CGlobalVars\t\t=>\t0x%08X\n", m_pGlobalVars);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "CInput\t\t\t=>\t0x%08X\n", m_pInput);
	g_pConsole->Write(LOGLEVEL_INFO, "IClientMode\t\t=>\t0x%08X\n", m_pClientMode);
	g_pConsole->Write(LOGLEVEL_INFO, "IClientState\t\t=>\t0x%08X\n", m_pClientState);
	g_pConsole->Write(LOGLEVEL_INFO, "IViewRender\t\t=>\t0x%08X\n", m_pViewRender);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::Init\t\t=>\t0x%08X\n", m_pInitKeyValues);
	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::LoadFromBuffer\=>\t0x%08X\n", m_pLoadFromBuffer);
	g_pConsole->WritePlain("\n");
#endif // _DEBUG
}

void CApplication::GetNetVars()
{
	CXorString xorBaseEntity("S_ڀvx��y�n"); // DT_BaseEntity
	CXorString xorBasePlayer("S_ڀvx��{j��e"); // DT_BasePlayer
	CXorString xorCollisionProperty("S_ځxg�db�Gy�ry�");
	CXorString xorCSPlayer("S_ځD[�nn�"); // DT_CSPlayer
	CXorString xorLocalPlayerExclusive("S_ڎxh�Gg�ry��tg�~}�"); // DT_LocalPlayerExclusive
	CXorString xorBaseCombatWeapon("S_ڀvx��xf�c\\�gd�"); // DT_BaseCombatWeapon
	CXorString xorWeaponCSBase("S_ڕrj��yHրvx�"); // DT_WeaponCSBase
	CXorString xorBaseCSGrenade("S_ڀvx��DL��yj�"); // DT_BaseCSGrenade
	CXorString xorBaseCombatCharacter("S_ڀvx��xf�cH�ej�ry"); // DT_BaseCombatCharacter
	CXorString xorBaseViewModel("S_ڀvx��~n�xo�"); // DT_BaseViewModel
	CXorString xorPlantedC4("S_ڒ{j�ro��");
	CXorString xorBaseAttributableItem("S_ڀvx��c��u~�ug��cn�"); // DT_BaseAttributableItem
	CXorString xorBaseAnimating("S_ڀvx��yb�cb�"); // DT_BaseAnimating

	m_pNetVarMgr = new CNetVarManager();
	m_pNetVarMgr->AddTable(xorBaseEntity.ToCharArray());
	m_pNetVarMgr->AddTable(xorBasePlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorCSPlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatWeapon.ToCharArray());
	m_pNetVarMgr->AddTable(xorWeaponCSBase.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCSGrenade.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatCharacter.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseViewModel.ToCharArray());
	m_pNetVarMgr->AddTable(xorPlantedC4.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseAttributableItem.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseAnimating.ToCharArray());

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Loading NetVars... ");
#endif

	m_pNetVarMgr->LoadTables(m_pClient->GetAllClasses(), true);

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n");
	g_pConsole->Write(LOGLEVEL_INFO, "Loading Offsets... ");
#endif

	Offsets::m_angRotation = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_angRotation*/CXorString("zT�pY�v�y").ToCharArray());
	Offsets::m_nModelIndex = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_nModelIndex*/CXorString("zT�xo�^e�o").ToCharArray());
	Offsets::m_hMyWeapons = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter.ToCharArray(), /*m_hMyWeapons*/CXorString("zT�n\\�gd�").ToCharArray());
	Offsets::m_hViewModel = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_hViewModel[0]*/CXorString("zT�~n�xo�L;�").ToCharArray());
	Offsets::m_flSimulationTime = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_flSimulationTime*/CXorString("zT�Db�{j�xeѫzn").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_vecMins = m_pNetVarMgr->GetOffset(2, xorBaseEntity.ToCharArray(), xorCollisionProperty.ToCharArray(), /*m_vecMins*/CXorString("zT�tF�d").ToCharArray());
	Offsets::m_vecMaxs = m_pNetVarMgr->GetOffset(2, xorBaseEntity.ToCharArray(), xorCollisionProperty.ToCharArray(), /*m_vecMaxs*/CXorString("zT�tF�d").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_vecOrigin = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_vecOrigin*/CXorString("zT�tD��pb�").ToCharArray());
	Offsets::m_vecViewOffset = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_vecViewOffset[0]*/CXorString("zT�t]�`D�dn�'V").ToCharArray());
	Offsets::m_angEyeAngles = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_angEyeAngles*/CXorString("zT�pN��Ve�rx").ToCharArray());

	m_pNetVarEyeAngles0 = m_pNetVarMgr->GetNetVar(xorCSPlayer.ToCharArray(), /*m_angEyeAngles[0]*/CXorString("zT�pN��Ve�rx��J").ToCharArray());
	m_pNetVarEyeAngles1 = m_pNetVarMgr->GetNetVar(xorCSPlayer.ToCharArray(), /*m_angEyeAngles[1]*/CXorString("zT�pN��Ve�rx��J").ToCharArray());

	Offsets::m_bHasHeavyArmor = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bHasHeavyArmor*/CXorString("zT�vxͧv}��ef�").ToCharArray());
	m_pNetVarLowerBodyYaw = m_pNetVarMgr->GetNetVar(xorCSPlayer.ToCharArray(), /*m_flLowerBodyYawTarget*/CXorString("zT�[d�eI�nR�Cj��r").ToCharArray());
	Offsets::m_flLowerBodyYawTarget = m_pNetVarLowerBodyYaw->GetOffset();
	Offsets::m_vecVelocity = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_vecVelocity[0]*/CXorString("zT�t]�xh�nP��").ToCharArray());
	Offsets::m_iTeamNum = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_iTeamNum*/CXorString("zT�rj�bf").ToCharArray());
	Offsets::m_lifeState = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_lifeState*/CXorString("zT�qnֶv�").ToCharArray());
	Offsets::m_iHealth = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_iHealth*/CXorString("zT�rj�").ToCharArray());
	Offsets::m_ArmorValue = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_ArmorValue*/CXorString("zTİzd��vg�").ToCharArray());
	Offsets::m_bHasHelmet = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bHasHelmet*/CXorString("zT�vxͧ{f�").ToCharArray());
	Offsets::m_fFlags = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_fFlags*/CXorString("zT�{j�").ToCharArray());
	Offsets::m_bSpotted = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_bSpotted*/CXorString("zT�gd�ro").ToCharArray());
	Offsets::m_bGunGameImmunity = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bGunGameImmunity*/CXorString("zT�be£zn̯z~�cr").ToCharArray());
	Offsets::m_flFlashDuration = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_flFlashDuration*/CXorString("zT�Qg�O�v�y").ToCharArray());
	Offsets::m_flFlashMaxAlpha = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_flFlashMaxAlpha*/CXorString("zT�Qg�F�Vg��v").ToCharArray());
	//Offsets::m_bMoveType = m_pNetVarMgr->GetOffset("DT_BaseEntity", "m_bMoveType"); <= Not existant :/
	Offsets::m_bIsScoped = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bIsScoped*/CXorString("zT�dX�gn�").ToCharArray());
	Offsets::m_iShotsFired = m_pNetVarMgr->GetOffset(2, xorCSPlayer.ToCharArray(),
		/*DT_CSLocalPlayerExclusive*/CXorString("S_ځDG�vgծvr�Rs�bx�r").ToCharArray(),
		/*m_iShotsFired*/CXorString("zT�d�Qb��s").ToCharArray());
	Offsets::m_hActiveWeapon = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter.ToCharArray(), /*m_hActiveWeapon*/CXorString("zT�t�r\\�gd�").ToCharArray());
	Offsets::m_hObserverTarget = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_hObserverTarget*/CXorString("zT�ux�an��vy�c").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::deadflag = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(),
		/*DT_PlayerState*/CXorString("S_ڒ{j��eX�cn").ToCharArray(),
		/*deadflag*/CXorString("sn�qg�").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_nTickBase = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_nTickBase*/CXorString("zT�~h�vx�").ToCharArray());
	Offsets::m_bIsDefusing = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bIsDefusing*/CXorString("zT�dO�bx�p").ToCharArray());
	Offsets::m_flC4Blow = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_flC4Blow*/CXorString("zT�T?Ǯx|").ToCharArray());
	Offsets::m_flDefuseCountDown = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_flDefuseCountDown*/CXorString("zT�Sn�dnƭbe�x|�").ToCharArray());
	Offsets::m_hBombDefuser = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_hBombDefuser*/CXorString("zT�xf�rm�ry").ToCharArray());

	CNetVar* pDtLocal = m_pNetVarMgr->GetNetVar(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*DT_Local*/CXorString("S_ڎxh�").ToCharArray());
	Offsets::m_nJumpTimeMsecs = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_nJumpTimeMsecs*/CXorString("zT�bf��~f��dn�").ToCharArray())->GetOffset();
	Offsets::m_flFallVelocity = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_flFallVelocity*/CXorString("zT�Qj�An�tb�").ToCharArray())->GetOffset();
	Offsets::m_viewPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_viewPunchAngle*/CXorString("zT�r|շyh�yl�").ToCharArray())->GetOffset();
	Offsets::m_aimPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngle*/CXorString("zT�z[�tcĬpg�").ToCharArray())->GetOffset();
	Offsets::m_aimPunchAngleVel = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngleVel*/CXorString("zT�z[�tcĬpg��rg").ToCharArray())->GetOffset();

	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_iItemDefinitionIndex = m_pNetVarMgr->GetOffset(4, xorBaseCombatWeapon.ToCharArray(),
		/*DT_EconEntity*/CXorString("S_ڇtd�y�n").ToCharArray(),
		/*DT_AttributeContainer*/CXorString("S_ڃc��u~�Td�vb�e").ToCharArray(),
		/*DT_ScriptCreatedItem*/CXorString("S_ڑty�cH��v�^�").ToCharArray(),
		/*m_iItemDefinitionIndex*/CXorString("zT�cn�rm�~�yB�rs").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_hOwner*/CXorString("zT�`e�").ToCharArray());
	Offsets::m_iClip1 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip1*/CXorString("zT�{b��").ToCharArray());
	Offsets::m_iClip2 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip2*/CXorString("zT�{b��").ToCharArray());
	Offsets::m_flNextPrimaryAttack = m_pNetVarMgr->GetOffset(2, xorBaseCombatWeapon.ToCharArray(),
		/*DT_LocalActiveWeaponData*/CXorString("S_ڎxh�Vh�anҧv{�Sj�").ToCharArray(),
		/*m_flNextPrimaryAttack*/CXorString("zT�Yn��Gy�vy��c�|").ToCharArray());

	Offsets::m_fAccuracyPenalty = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_fAccuracyPenalty*/CXorString("zT�th�vh��re�cr").ToCharArray());
	Offsets::m_flPostponeFireReadyTime = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_flPostponeFireReadyTime*/CXorString("zT�Gd��gd�Qb��En�n_�r").ToCharArray());

	Offsets::m_fThrowTime = m_pNetVarMgr->GetOffset(xorBaseCSGrenade.ToCharArray(), /*m_fThrowTime*/CXorString("zT�y�Cb�").ToCharArray());

	CNetVar* pDtAttributeManager = m_pNetVarMgr->GetNetVar(xorBaseAttributableItem.ToCharArray(), /*DT_AttributeContainer*/CXorString("S_ڃc��u~�Td�vb�e").ToCharArray());
	CNetVar* pDtItem = pDtAttributeManager->GetChild(/*DT_ScriptCreatedItem*/CXorString("S_ڑty�cH��v�^�").ToCharArray());
	int iOffset = pDtAttributeManager->GetOffset() + pDtItem->GetOffset();

	Offsets::m_iItemIDHigh = iOffset + pDtItem->GetChild(/*m_iItemIDHigh*/CXorString("zT�cn�SC�").ToCharArray())->GetOffset();
	Offsets::m_iItemIDLow = iOffset + pDtItem->GetChild(/*m_iItemIDLow*/CXorString("zT�cn�SG�").ToCharArray())->GetOffset();
	Offsets::m_iAccountID = iOffset + pDtItem->GetChild(/*m_iAccountID*/CXorString("zT�th�y̆").ToCharArray())->GetOffset();
	Offsets::m_iEntityQuality = iOffset + pDtItem->GetChild(/*m_iEntityQuality*/CXorString("zT�y�nZ�{b�").ToCharArray())->GetOffset();
	Offsets::m_szCustomName = iOffset + pDtItem->GetChild(/*m_szCustomName*/CXorString("zT��T~��xfˣzn").ToCharArray())->GetOffset();

	Offsets::m_OriginalOwnerXuidLow = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_OriginalOwnerXuidLow*/CXorString("zTʰ~l�vgʵyn��bb�x|").ToCharArray());
	Offsets::m_OriginalOwnerXuidHigh = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_OriginalOwnerXuidHigh*/CXorString("zTʰ~l�vgʵyn��bb�~l�").ToCharArray());
	Offsets::m_nFallbackPaintKit = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackPaintKit*/CXorString("�������������������", 0xAFFEAFFE).ToCharArray());
	Offsets::m_nFallbackSeed = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackSeed*/CXorString("zT�vg�vh�rn�").ToCharArray());
	Offsets::m_flFallbackWear = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_flFallbackWear*/CXorString("zT�Qj�uj�@n�").ToCharArray());
	Offsets::m_nFallbackStatTrak = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackStatTrak*/CXorString("zT�vg�vh�cj�ej�").ToCharArray());

	Offsets::DT_BaseViewModel::m_hWeapon = m_pNetVarMgr->GetOffset(xorBaseViewModel.ToCharArray(), /*m_hWeapon*/CXorString("zT�rj��y").ToCharArray());
	Offsets::DT_BaseViewModel::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseViewModel.ToCharArray(), /*m_hOwner*/CXorString("zT�`e�").ToCharArray());

	Offsets::m_nSequence = m_pNetVarMgr->GetOffset(xorBaseAnimating.ToCharArray(), /*m_nSequence*/CXorString("zT�rz�yh�").ToCharArray());
	Offsets::m_flPoseParameter = m_pNetVarMgr->GetOffset(xorBaseAnimating.ToCharArray(), /*m_flPoseParameter*/CXorString("zT�Gd��Gj��zn�e").ToCharArray());
	Offsets::m_flCycle = m_pNetVarMgr->GetOffset(2, xorBaseAnimating.ToCharArray(),
		/*DT_ServerAnimationData*/CXorString("S_ڑry�eJ�zj�xe��cj").ToCharArray(),
		/*m_flCycle*/CXorString("zT�Tr�r").ToCharArray());

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n\n");
#endif

	// Grab NetVars later required for hooking
	m_pNetVarSequence = m_pNetVarMgr->GetNetVar(xorBaseViewModel.ToCharArray(), /*m_nSequence*/CXorString("zT�rz�yh�").ToCharArray());

	m_misc.SetReadyCallback((IsReadyCallback_t)(CPattern::FindPattern(
		(BYTE*)this->ClientDll(),
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE",
		"ghdrtzuigkog----trg"
	)));
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

DWORD ThreadFreeLibrary(void* pParam)
{
	// Wait 1 sec for hooks to finish running
	Sleep(1000);
	FreeLibraryAndExitThread((HMODULE)pParam, 0);

	return 0;
}