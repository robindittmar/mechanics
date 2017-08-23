#include "Application.h"
#include "Vector.h"
#include <math.h>

CreateMove_t CApplication::m_pCreateMove;
FrameStageNotify_t CApplication::m_pFrameStageNotify;
OverrideView_t CApplication::m_pOverrideView;
DrawModelExecute_t CApplication::m_pDrawModelExecute;
PaintTraverse_t CApplication::m_pPaintTraverse;
GetViewModelFov_t CApplication::m_pGetViewModelFov;

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
	// Unregister listener
	this->m_pGameEventManager->RemoveListener(&this->m_gameEventListener);

	// Reverse order, in case of any dependencies
	this->m_pVguiHook->Restore();
	this->m_pClientHook->Restore();
	this->m_pEngineModelHook->Restore();
	this->m_pClientModeHook->Restore();

	// Free ResourceManager
	delete g_pResourceManager;
	g_pResourceManager = NULL;

	// Free console
	delete g_pConsole;
	g_pConsole = NULL;

	// Free & Exit
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFreeLibrary, this->m_hModule, NULL, NULL);
}

bool __fastcall CApplication::hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = m_pCreateMove(ecx, fInputSampleTime, pUserCmd);

	CApplication* pApp = CApplication::Instance();

	// Update timer for hitmarker
	pApp->Visuals()->UpdateHitmarker(fInputSampleTime);

	// Instantly return
	if (!pUserCmd || !pUserCmd->command_number)
		return false;

	if (!pApp->m_bGotSendPackets)
	{
		uintptr_t* fp;
		__asm mov fp, ebp;
		pApp->m_bSendPackets = (bool*)(*fp - 0x1C);
		pApp->m_bGotSendPackets = true;
	}

	if (pApp->EngineClient()->IsInGame())
	{
		IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
		if (pLocalEntity->IsAlive())
		{
			// Save Viewangles before doing stuff
			pApp->EngineClient()->GetViewAngles(pApp->ClientViewAngles());
			QAngle qOldAngles = pApp->ClientViewAngles();

			pApp->Misc()->AutoRevolver(pUserCmd);

			// Update Aimbot
			AimbotUpdateParam aimBotParam = { fInputSampleTime, pUserCmd };
			pApp->Aimbot()->Update((void*)&aimBotParam);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update NoRecoil & AutoPistol
			pApp->Misc()->AutoPistol(pUserCmd);
			pApp->Misc()->NoRecoil(pUserCmd);
			// Update Fakelag
			pApp->Misc()->Fakelag(pUserCmd);
			// Update AutoStrafe
			pApp->Misc()->AutoStrafe(pUserCmd);

			// Update AntiAim
			pApp->AntiAim()->Update(pUserCmd);

			// Correct movement & angles
			CorrectMovement(pUserCmd, qOldAngles);
			NormalizeAngles(pUserCmd);

			// Set ViewAngles we prepared for display
			pApp->EngineClient()->SetViewAngles(pApp->ClientViewAngles());

			//todo: check if fake and if sendpackets false
			/*if (flip)
			{*/
			pApp->m_qLastTickAngles.x = pUserCmd->viewangles[0];
			pApp->m_qLastTickAngles.y = pUserCmd->viewangles[1];
			pApp->m_qLastTickAngles.z = pUserCmd->viewangles[2];
			//}
		}
	}

	return false;
}

void __fastcall CApplication::hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CApplication* pApp = CApplication::Instance();

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (pApp->EngineClient()->IsInGame())
		{
			IClientEntity* pEntity = pApp->EntityList()->GetClientEntity(pApp->Aimbot()->SelectedTarget());
			if (pEntity)
			{
				pEntity->AngEyePosition()->y = pEntity->LowerBodyYaw();
			}
		}
	}

	if (curStage == FRAME_RENDER_START)
	{
		if (pApp->EngineClient()->IsInGame())
		{
			IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
			if (pLocalEntity->IsAlive())
			{
				pApp->Visuals()->NoSmoke();
				pApp->Visuals()->ThirdpersonAntiAim();
			}

			pApp->Visuals()->NoFlash();
		}

		// Input handling
		pApp->m_inputEvent.Clear();
		pApp->m_inputHandler.CreateInput(&pApp->m_inputEvent, pApp->m_pWindow->IsVisible());
		if (pApp->m_inputEvent.eventType == EVENT_TYPE_KEYBOARD)
		{
			if (pApp->m_inputEvent.buttons & EVENT_BTN_TOGGLEMENU &&
				pApp->m_inputEvent.buttonProperties & EVENT_BTN_TOGGLEMENU)
			{
				pApp->m_pWindow->IsVisible(!pApp->m_pWindow->IsVisible());
				if (pApp->m_pWindow->IsVisible())
				{
					// TODO: Xor
					pApp->m_pGui->DisableIngameMouse();
					pApp->m_pGui->SetDrawMouse(true);
				}
				else
				{
					// TODO: Xor
					pApp->m_pGui->EnableIngameMouse();
					pApp->m_pGui->SetDrawMouse(false);
				}
			}
			else if (pApp->m_inputEvent.buttons & EVENT_BTN_DETACH &&
				pApp->m_inputEvent.buttonProperties & EVENT_BTN_DETACH)
			{
				pApp->Detach();
			}
			else if (pApp->m_inputEvent.buttons & EVENT_BTN_THIRDPERSON &&
				!(pApp->m_inputEvent.buttonProperties & EVENT_BTN_THIRDPERSON))
			{
				bool bNewValue = !pApp->m_visuals.GetThirdperson();

				pApp->m_pGuiThirdpersonCheckbox->IsChecked(bNewValue);
				pApp->m_visuals.SetThirdperson(bNewValue);
			}
		}

		if (pApp->m_pWindow->IsVisible())
		{
			pApp->m_pWindow->ProcessEvent(&pApp->m_inputEvent);
		}
	}

	m_pFrameStageNotify(ecx, curStage);
}

void __fastcall CApplication::hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup)
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity;

	if (pApp->EngineClient()->IsInGame())
	{
		pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

		if (pLocalEntity->IsAlive())
		{
			pApp->Visuals()->FovChange(pViewSetup);
			pApp->Visuals()->Thirdperson();
			pApp->Visuals()->NoVisualRecoil(pViewSetup);
		}
	}
	return m_pOverrideView(ecx, pViewSetup);
}

void __fastcall CApplication::hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);

		pApp->Chams()->Render(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pApp->Visuals()->HandsDrawStyle(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}

	// Call original func
	m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);

	// This is necessary for stuff to work properly
	pApp->ModelRender()->ForcedMaterialOverride(NULL);
}

void __fastcall CApplication::hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->EngineClient()->IsInGame())
	{
		if (pApp->Misc()->NoScope(vguiPanel))
			return;

		static unsigned int vguiMatSystemTopPanel;
		if (vguiMatSystemTopPanel == NULL)
		{
			static CXorString matSystemTopPanel("Zjñ‘nxñ§z_ê²Gjë§{");
			const char* szName = pApp->Panel()->GetName(vguiPanel);
			if (stricmp(szName, matSystemTopPanel.ToCharArray()) == 0)
			{
				vguiMatSystemTopPanel = vguiPanel;
			}
		}

		if (vguiMatSystemTopPanel == vguiPanel)
		{
			// Draw NoScope & SpecList
			pApp->Misc()->DrawNoScope();
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Update();

			// Draw Hitmarker
			pApp->Visuals()->DrawHitmarker();

			// Draw Crosshair last (but not least)
			pApp->Visuals()->DrawCrosshair();

			// Draw Menu least ;)
			pApp->m_pWindow->Draw(pApp->Surface());
			pApp->m_pGui->DrawMouse(pApp->Surface());
		}
	}

	m_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}

float __fastcall CApplication::hk_GetViewModelFov(void* ecx, void* edx)
{
	CApplication* pApp = CApplication::Instance();

	return m_pGetViewModelFov(ecx) + 70.0f;
}

/*void BtnDown(IControl* p)
{
CButton* btn = (CButton*)p;
g_pConsole->Write("Button down (%s)\n", btn->ContentText());
}*/

void BtnUp(IControl* p)
{
	CApplication* pApp = CApplication::Instance();

	pApp->Gui()->EnableIngameMouse();
	pApp->Detach();
}

void CApplication::Setup()
{
	// Setup console
	g_pConsole = new CConsole();

	// Resource manager
	g_pResourceManager = new CResourceManager();
	g_pResourceManager->Init(this);
	m_pResourceManager = g_pResourceManager;

	// Setup strings
	CXorString clientDll("tgì§y«¦{g");
	CXorString engineDll("reâ«yn«¦{g");
	CXorString materialSystemDll("zjñ§ebä®drö¶rf«¦{g");
	CXorString createInterface("Tyà£cnÌ¬cn÷¤vhà");
	CXorString VEngineClient("ANë¥~eà{bà¬c;´ö");
	CXorString VClient("AHé«reñò&3");
	CXorString VClientEntityList("AHé«reñ‡yì¶nGì±c;µñ");
	CXorString VModelInfo("ü˜…‘Ï¹£›Ìº©™Ã°„šåÞ", 0x1235AFAA);
	CXorString VModelRender("ANë¥~eàxoà®':³");
	CXorString EngineTraceClient("_cajthRq{nc@vdcmn=67", 0x1A);
	CXorString VMaterialSystem("AFä¶ryì£{Xü±cnèò/;");
	CXorString vgui2Dll("alð«%%á®{");
	CXorString vguiSurfaceDll("alð«zjñ±byã£tn«¦{g");
	CXorString VguiPanel("ALÐ‹H[ä¬rgµò.");
	CXorString VguiSurface("ALÐ‹HXð°qjæ§'8´");
	CXorString GameEventListener("lÔ\a`nÃ\xfkÆ\adeÔ\r`y¥z\x17", 0xF12B);
	CXorString player_hurt("ggä»ryÚªbyñ");
	CXorString player_death("ggä»ryÚ¦rjñª");
	CXorString round_start("edð¬sTö¶vyñ");
	CXorString round_end("edð¬sTà¬s");
	CXorString vphysicsDll("a{í»dbæ±9oé®");
	CXorString physicsSurfaceProps("A[í»dbæ±D~÷¤vhà’edõ±';´");
	CXorString renderView("ANë¥~eàreá§e]ì§`;´ö");

	// Grab engine addresses
	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(materialSystemDll.ToCharArray());
	this->m_dwVgui2Dll = (DWORD)GetModuleHandle(vgui2Dll.ToCharArray());
	this->m_dwVguiSurfaceDll = (DWORD)GetModuleHandle(vguiSurfaceDll.ToCharArray());
	this->m_dwVPhysicsDll = (DWORD)GetModuleHandle(vphysicsDll.ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateMaterialSystemInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVgui2Interface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVgui2Dll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVguiSurfaceInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVguiSurfaceDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVPhysicsInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVPhysicsDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClient = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)CreateEngineInterface(VModelInfo.ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)CreateEngineInterface(VModelRender.ToCharArray(), NULL);
	m_pRenderView = (IVRenderView*)CreateEngineInterface(renderView.ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)CreateEngineInterface(EngineTraceClient.ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)CreateMaterialSystemInterface(VMaterialSystem.ToCharArray(), NULL);
	m_pPanel = (IPanel*)CreateVgui2Interface(VguiPanel.ToCharArray(), NULL);
	m_pSurface = (ISurface*)CreateVguiSurfaceInterface(VguiSurface.ToCharArray(), NULL);
	m_pGameEventManager = (IGameEventManager2*)CreateEngineInterface(GameEventListener.ToCharArray(), NULL);
	m_pPhysicsSurfaceProps = (IPhysicsSurfaceProps*)CreateVPhysicsInterface(physicsSurfaceProps.ToCharArray(), NULL);

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS);

	// Create Resources
	m_pResourceManager->CreateFonts();

	// Print all classes & their properties
	/*ClientClass* lClass = m_pClient->GetAllClasses();
	while(lClass)
	{
	g_pConsole->Write("%s\n", lClass->m_pNetworkName);

	RecvTable* pTable = lClass->m_pRecvTable;
	for(int i = 0; i < pTable->m_nProps; i++)
	{
	g_pConsole->Write("\t> %s (0x%08X)\n", pTable->m_pProps[i].m_pVarName, pTable->m_pProps[i].m_Offset);
	}

	lClass = lClass->m_pNext;
	}*/

	// Setups
	this->m_aimbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_chams.Setup();
	this->m_misc.Setup();
	this->m_visuals.Setup();

	// Aimbot
	this->m_aimbot.SetEnabled(true);
	this->m_aimbot.SetAutoshoot(true);
	this->m_aimbot.SetAutoscope(true);
	this->m_aimbot.SetSilentAim(true);
	this->m_aimbot.SetTargetCriteria(TARGETCRITERIA_VIEWANGLES);
	this->m_aimbot.SetSpeed(1.0f);
	this->m_aimbot.SetFov(360.0f);

	// Antiaim
	this->m_antiAim.SetEnabled(true);
	this->m_antiAim.SetPitchSetting(PITCHANTIAIM_DOWN);
	this->m_antiAim.SetYawSetting(YAWANTIAIM_BACKWARDS);

	// Bhop
	this->m_bhop.SetEnabled(true);

	// ESP
	this->m_esp.SetEnabled(true);
	this->m_esp.SetDrawBoundingBox(true);
	this->m_esp.SetDrawNames(true);
	this->m_esp.SetDrawHealthBar(true);
	this->m_esp.SetDrawHealthNumber(true);
	this->m_esp.SetDrawArmorBar(false);
	this->m_esp.SetDrawOwnTeam(false);
	this->m_esp.SetDrawOwnModel(true);
	this->m_esp.SetDrawOnlySpotted(false);
	this->m_esp.SetDrawOutline(true);

	// Chams
	this->m_chams.SetEnabled(true);
	this->m_chams.SetRenderTeam(true);
	this->m_chams.SetRenderLocalplayer(true);

	// Misc
	this->m_misc.SetEnabled(true);
	this->m_misc.SetNoRecoil(true);
	this->m_misc.SetFakelag(false);
	this->m_misc.SetAutoStrafe(true);
	this->m_misc.SetNoScope(true);
	this->m_misc.SetAutoPistol(true);
	this->m_misc.SetShowSpectators(false);
	this->m_misc.SetShowOnlyMySpectators(false);
	this->m_misc.SetShowOnlyMyTeamSpectators(false);

	// Visuals
	this->m_visuals.SetEnabled(true);

	this->m_visuals.SetCrosshair(true);
	this->m_visuals.SetHitmarker(true);
	this->m_visuals.SetNoSmoke(true);
	this->m_visuals.SetHandsDrawStyle(HANDSDRAWSTYLE_WIREFRAME);
	this->m_visuals.SetNoVisualRecoil(true);

	this->m_visuals.SetNoFlash(true);
	this->m_visuals.SetFlashPercentage(0.f);

	this->m_visuals.SetThirdperson(false);
	this->m_visuals.SetThirdpersonDistance(150);

	this->m_visuals.SetFovChange(true);
	this->m_visuals.SetFovValue(110);
	this->m_visuals.SetFovChangeScoped(false);

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, player_hurt.ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, player_death.ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, round_start.ToCharArray(), false);
	m_pGameEventManager->AddListener(&m_gameEventListener, round_end.ToCharArray(), false);

	// Create all Gui stuff
	m_pGui = CGui::Instance();

	// Initialize InputHandler
	m_inputHandler.RegisterKey(VK_DELETE, EVENT_BTN_DETACH);
	m_inputHandler.RegisterKey(VK_INSERT, EVENT_BTN_TOGGLEMENU);
	m_inputHandler.RegisterKey(VK_NUMPAD0, EVENT_BTN_THIRDPERSON);

	// Create GUI (Window + all controls)
	CButton* pBtn = new CButton(16, 64, 120, 45, "Detach");
	//pBtn->SetButtonDownEventHandler(BtnDown);
	pBtn->SetButtonUpEventHandler(BtnUp);

	CCheckbox* pAimbot = new CCheckbox(16, 16, 120, 32, "Aimbot", m_aimbot.GetEnabled());
	pAimbot->SetEventHandler(std::bind(&CAimbot::SetEnabled, &m_aimbot, std::placeholders::_1));
	
	CCheckbox* pSilentAim = new CCheckbox(16, 64, 120, 32, "Silent Aim", m_aimbot.GetSilentAim());
	pSilentAim->SetEventHandler(std::bind(&CAimbot::SetSilentAim, &m_aimbot, std::placeholders::_1));

	CCheckbox* pCheck = new CCheckbox(16, 112, 120, 32, "Autoshoot", m_aimbot.GetAutoshoot());
	pCheck->SetEventHandler(std::bind(&CAimbot::SetAutoshoot, &m_aimbot, std::placeholders::_1));

	CCheckbox* pAutoScope = new CCheckbox(16, 160, 120, 32, "Autoscope", m_aimbot.GetAutoscope());
	pAutoScope->SetEventHandler(std::bind(&CAimbot::SetAutoscope, &m_aimbot, std::placeholders::_1));

	CCheckbox* pCheck2 = new CCheckbox(160, 112, 120, 32, "Thirdperson", m_visuals.GetThirdperson());
	pCheck2->SetEventHandler(std::bind(&CVisuals::SetThirdperson, &m_visuals, std::placeholders::_1));
	m_pGuiThirdpersonCheckbox = pCheck2;

	CSelectbox* pSelectbox = new CSelectbox(16, 16, 100, 32);
	pSelectbox->AddOption(0, "Unspecified");
	pSelectbox->AddOption(1, "NoHands");
	pSelectbox->AddOption(2, "lol");
	pSelectbox->AddOption(3, "Just testing");
	pSelectbox->AddOption(4, "this is fun");
	pSelectbox->AddOption(5, "nicht");
	pSelectbox->AddOption(6, "SIEG");

	// TODO: Groupbox -> "Antiaim" :D
	CSelectbox* pSelectPitchAntiaim = new CSelectbox(16, 64, 100, 32, "Pitch");
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_NONE, "None");
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_DOWN, "Down");
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_UP, "Up");
	pSelectPitchAntiaim->SetEventHandler(std::bind(&CAntiAim::SetPitchSetting, &m_antiAim, std::placeholders::_1));

	CSelectbox* pSelectYawAntiaim = new CSelectbox(16, 112, 100, 32, "Yaw");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_NONE, "None");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_REALLEFTFAKERIGHT, "REAL LEFT FAKE RIGHT (NAME VIEL ZU LANG LOL)");
	pSelectPitchAntiaim->SetEventHandler(std::bind(&CAntiAim::SetYawSetting, &m_antiAim, std::placeholders::_1));

	CSelectbox* pSelectbox2 = new CSelectbox(260, 16, 100, 32);
	pSelectbox2->AddOption(HANDSDRAWSTYLE_NONE, "None");
	pSelectbox2->AddOption(HANDSDRAWSTYLE_NOHANDS, "NoHands");
	pSelectbox2->AddOption(HANDSDRAWSTYLE_WIREFRAME, "Wireframe");
	pSelectbox2->SetSelection(m_visuals.GetHandsDrawStyle());
	pSelectbox2->SetEventHandler(std::bind(&CVisuals::SetHandsDrawStyle, &m_visuals, std::placeholders::_1));

	CCheckbox* pDrawBoundingBox = new CCheckbox(16, 16, 128, 32, "Bounding Box", m_esp.GetDrawBoundingBox());
	CCheckbox* pDrawHealthbar = new CCheckbox(16, 64, 128, 32, "Health bar", m_esp.GetDrawHealthBar());
	CCheckbox* pDrawHealthnumber = new CCheckbox(16, 112, 128, 32, "Health number", m_esp.GetDrawHealthNumber());
	CCheckbox* pDrawArmorbar = new CCheckbox(16, 160, 128, 32, "Armor bar", m_esp.GetDrawArmorBar());
	CCheckbox* pDrawOwnTeam = new CCheckbox(16, 208, 128, 32, "Own team", m_esp.GetDrawOwnTeam());
	CCheckbox* pDrawOwnModel = new CCheckbox(16, 256, 128, 32, "Own model (3rd person)", m_esp.GetDrawOwnModel());
	CCheckbox* pDrawOnlySpotted = new CCheckbox(16, 304, 128, 32, "Only spotted", m_esp.GetDrawOnlySpotted());
	CCheckbox* pDrawOutline = new CCheckbox(160, 16, 128, 32, "Outlines", m_esp.GetDrawOutline());
	CCheckbox* pDrawNames = new CCheckbox(160, 64, 128, 32, "Names", m_esp.GetDrawNames());

	pDrawBoundingBox->SetEventHandler(std::bind(&CEsp::SetDrawBoundingBox, &m_esp, std::placeholders::_1));
	pDrawHealthbar->SetEventHandler(std::bind(&CEsp::SetDrawHealthBar, &m_esp, std::placeholders::_1));
	pDrawHealthnumber->SetEventHandler(std::bind(&CEsp::SetDrawHealthNumber, &m_esp, std::placeholders::_1));
	pDrawArmorbar->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, &m_esp, std::placeholders::_1));
	pDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, &m_esp, std::placeholders::_1));
	pDrawOwnModel->SetEventHandler(std::bind(&CEsp::SetDrawOwnModel, &m_esp, std::placeholders::_1));
	pDrawOnlySpotted->SetEventHandler(std::bind(&CEsp::SetDrawOnlySpotted, &m_esp, std::placeholders::_1));
	pDrawOutline->SetEventHandler(std::bind(&CEsp::SetDrawOutline, &m_esp, std::placeholders::_1));
	pDrawNames->SetEventHandler(std::bind(&CEsp::SetDrawNames, &m_esp, std::placeholders::_1));

	CTabContainer* pContainer = new CTabContainer();
	CTabPage* pPage1 = new CTabPage("Rage");
	CTabPage* pPage2 = new CTabPage("Legit");
	CTabPage* pPage3 = new CTabPage("Visuals");
	CTabPage* pPage4 = new CTabPage("Misc");
	CTabPage* pPage5 = new CTabPage("Skin Changer");
	CTabPage* pPage6 = new CTabPage("Config");

	pPage1->AddChild(pAimbot);
	pPage1->AddChild(pSilentAim);
	pPage1->AddChild(pCheck);
	pPage1->AddChild(pAutoScope);

	pPage3->AddChild(pDrawBoundingBox);
	pPage3->AddChild(pDrawHealthbar);
	pPage3->AddChild(pDrawHealthnumber);
	pPage3->AddChild(pDrawArmorbar);
	pPage3->AddChild(pDrawOwnTeam);
	pPage3->AddChild(pDrawOwnModel);
	pPage3->AddChild(pDrawOnlySpotted);
	pPage3->AddChild(pDrawOutline);
	pPage3->AddChild(pDrawNames);
	pPage3->AddChild(pCheck2);
	pPage3->AddChild(pSelectbox2);

	pPage4->AddChild(pSelectbox);
	pPage4->AddChild(pSelectPitchAntiaim);
	pPage4->AddChild(pSelectYawAntiaim);

	pPage6->AddChild(pBtn);

	pPage1->IsEnabled(true);

	pContainer->AddChild(pPage1);
	pContainer->AddChild(pPage2);
	pContainer->AddChild(pPage3);
	pContainer->AddChild(pPage4);
	pContainer->AddChild(pPage5);
	pContainer->AddChild(pPage6);

	m_pWindow = new CWindow(30, 30, 600, 400, "x88rv2");
	m_pWindow->AddChild(pContainer);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) {
		Sleep(100);
	}
}

void CApplication::Hook()
{
	// Grab Screensize
	m_pGui->GetScreenSize();

	// Create Resources
	m_pResourceManager->CreateFonts();

	// Get ClientMode and CInput
	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClient))[10] + 0x5));
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[15] + 0x1);

	DWORD dwInitKeyValuesTemp = (CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56",
		"a----bbb----ccccccc-----ddd"
	) + 7);
	m_pInitKeyValues = (InitKeyValues_t)(dwInitKeyValuesTemp + (*(DWORD_PTR*)(dwInitKeyValuesTemp + 1)) + 5);

	/*DWORD dwLoadFromBufferTemp = CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\xE8\x00\x00\x00\x00\x80\x7D\xF8\x00\x00\x00\x85\xDB",
		"a----cccc--ff"
	);
	m_pLoadFromBuffer = (LoadFromBuffer_t)(dwLoadFromBufferTemp + (*(DWORD_PTR*)(dwLoadFromBufferTemp + 1)) + 5);*/
	DWORD dwLoadFromBufferTemp = CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89\x4C\x24\x04",
		"abcdefghijjjjjjjq"
	);
	m_pLoadFromBuffer = (LoadFromBuffer_t)dwLoadFromBufferTemp;

	m_pClientModeHook = new VTableHook((DWORD*)dwClientMode, true);
	m_pOverrideView = (OverrideView_t)m_pClientModeHook->Hook(18, (DWORD*)hk_OverrideView);
	m_pCreateMove = (CreateMove_t)m_pClientModeHook->Hook(24, (DWORD*)hk_CreateMove);
	m_pGetViewModelFov = (GetViewModelFov_t)m_pClientModeHook->Hook(35, (DWORD*)hk_GetViewModelFov);

	m_pEngineModelHook = new VTableHook((DWORD*)this->m_pModelRender, true);
	m_pDrawModelExecute = (DrawModelExecute_t)m_pEngineModelHook->Hook(21, (DWORD*)hk_DrawModelExecute);

	m_pClientHook = new VTableHook((DWORD*) this->m_pClient, true);
	m_pFrameStageNotify = (FrameStageNotify_t)m_pClientHook->Hook(36, (DWORD*)hk_FrameStageNotify);

	m_pVguiHook = new VTableHook((DWORD*)this->m_pPanel, true);
	m_pPaintTraverse = (PaintTraverse_t)m_pVguiHook->Hook(41, (DWORD*)hk_PaintTraverse);
}

// Singleton
CApplication::CApplication()
{
	// TODO: Der konstruktor muss *ALLE* pointer auf NULL setzen, der destruktor
	//		 alle ptr != NULL löschen
	m_pWindow = NULL;

	m_pClientModeHook = NULL;
	m_pEngineModelHook = NULL;
	m_pClientHook = NULL;
	m_pVguiHook = NULL;

	m_bGotSendPackets = false;
}

CApplication::CApplication(CApplication const&)
{
}

CApplication::~CApplication()
{
	if (m_pVguiHook)
		delete m_pVguiHook;

	if (m_pClientHook)
		delete m_pClientHook;

	if (m_pEngineModelHook)
		delete m_pEngineModelHook;

	if (m_pClientModeHook)
		delete m_pClientModeHook;

	if (m_pWindow)
		delete m_pWindow;
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

	vecViewForward.Normalize();
	vecViewRight.Normalize();
	vecViewUp.Normalize();

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