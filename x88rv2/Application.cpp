#include "Application.h"
#include "Vector.h"
#include <math.h>

CreateMove_t CApplication::m_pCreateMove;
FrameStageNotify_t CApplication::m_pFrameStageNotify;
OverrideView_t CApplication::m_pOverrideView;
DrawModelExecute_t CApplication::m_pDrawModelExecute;
PaintTraverse_t CApplication::m_pPaintTraverse;

CApplication* CApplication::Instance()
{
	static CApplication inst;
	return &inst;
}

void CApplication::Run(HMODULE hModule)
{
	m_hModule = hModule;

	this->Setup();
	//Sleep(5000);
	this->Hook();
}

bool __fastcall CApplication::hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = m_pCreateMove(ecx, fInputSampleTime, pUserCmd);
	CApplication* pApp = CApplication::Instance();

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
			float view_forward = 0;
			float view_right = 10;

			// Save Viewangles before doing stuff
			pApp->EngineClient()->GetViewAngles(pApp->ClientViewAngles());
			QAngle qOldAngles = pApp->ClientViewAngles();


			// Update Aimbot
			pApp->Aimbot()->Update(pUserCmd);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update NoRecoil & AutoPistol
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

			pApp->m_qLastTickAngles.x = pUserCmd->viewangles[0];
			pApp->m_qLastTickAngles.y = pUserCmd->viewangles[1];
			pApp->m_qLastTickAngles.z = pUserCmd->viewangles[2];
		}
	}

	return false;
}

void __fastcall CApplication::hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CApplication* pApp = CApplication::Instance();

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
		
		// TODO: Temporary, I hope
		if(GetAsyncKeyState(VK_DELETE) & 0x8000)
		{
			// Reverse order, in case of any dependencies
			pApp->VguiHook()->Restore();
			pApp->ClientHook()->Restore();
			pApp->EngineModelHook()->Restore();
			pApp->ClientModeHook()->Restore();
				
			// Free & Exit
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFreeLibrary, pApp->m_hModule, NULL, NULL);
		}
		// TODO: Temporary, I hope
	}

	m_pFrameStageNotify(ecx, curStage);
}

void __fastcall CApplication::hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup) {
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

	if (pApp->EngineClient()->IsInGame())
	{
		if (pLocalEntity->IsAlive())
		{
			pApp->Visuals()->FovChange(pViewSetup);
			pApp->Visuals()->Thirdperson();
			pApp->Visuals()->NoVisualRecoil(pViewSetup);
		}
	}
	return m_pOverrideView(ecx, pViewSetup);
}

void __fastcall CApplication::hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);

		pApp->Visuals()->HandsDrawStyle(pszModelName);
	}

	m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
}

void __fastcall CApplication::hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
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
			pApp->Misc()->DrawNoScope();

			pApp->Esp()->Update();
		}
	}

	m_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}

void CApplication::Setup()
{
	// Create GUI (Window + all controls)
	m_pWindow = new CWindow(30, 30, 500, 400, "I'm a title lel");
	m_pWindow->AddChild(
		new CButton(10, 10)
	);
	//m_pWindow->IsVisible(true);

	// Setup engine stuff
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

	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(materialSystemDll.ToCharArray());
	this->m_dwVgui2Dll = (DWORD)GetModuleHandle(vgui2Dll.ToCharArray());
	this->m_dwVguiSurfaceDll = (DWORD)GetModuleHandle(vguiSurfaceDll.ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateMaterialSystemInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVgui2Interface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVgui2Dll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVguiSurfaceInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVguiSurfaceDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClient = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)CreateEngineInterface(VModelInfo.ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)CreateEngineInterface(VModelRender.ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)CreateEngineInterface(EngineTraceClient.ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)CreateMaterialSystemInterface(VMaterialSystem.ToCharArray(), NULL);
	m_pPanel = (IPanel*)CreateVgui2Interface(VguiPanel.ToCharArray(), NULL);
	m_pSurface = (ISurface*)CreateVguiSurfaceInterface(VguiSurface.ToCharArray(), NULL);

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS);

	// Setups
	this->m_aimbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_misc.Setup();
	this->m_visuals.Setup();

	// Aimbot
	this->m_aimbot.IsEnabled(true);
	this->m_aimbot.IsAutoshoot(true);
	this->m_aimbot.IsAutoscope(true);
	this->m_aimbot.IsSilentAim(true);
	this->m_aimbot.TargetCriteria(TargetCriteriaViewangle);
	this->m_aimbot.Speed(1.0f);
	this->m_aimbot.Fov(360.0f);

	// AA, Bhop
	this->m_antiAim.IsEnabled(true);
	this->m_bhop.IsEnabled(true);

	// ESP
	this->m_esp.IsEnabled(true);
	this->m_esp.ShouldDrawBoundingBox(true);
	this->m_esp.ShouldDrawHealthBar(true);
	this->m_esp.ShouldDrawArmorBar(false);
	this->m_esp.ShouldDrawOwnTeam(false);
	this->m_esp.ShouldDrawOwnModel(true);
	this->m_esp.ShouldDrawOnlySpotted(false);

	// Misc
	this->m_misc.IsEnabled(true);
	this->m_misc.IsNoRecoil(true);
	this->m_misc.IsFakelag(false);
	this->m_misc.IsAutoStrafe(true);
	this->m_misc.IsNoScope(true);
	this->m_misc.IsAutoPistol(true);

	// Visuals
	this->m_visuals.IsEnabled(true);

	this->m_visuals.IsNoSmoke(true);
	this->m_visuals.HandsDrawStyle(HandsDrawStyleNoHands);
	this->m_visuals.IsNoVisualRecoil(true);

	this->m_visuals.IsNoFlash(true);
	this->m_visuals.NoFlashPercentage(0.2f);

	this->m_visuals.IsThirdperson(false);
	this->m_visuals.ThirdpersonValue(120);

	this->m_visuals.IsFovChange(true);
	this->m_visuals.FovValue(110);
	this->m_visuals.ShouldFovChangeScoped(true);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) {}
}

void CApplication::Hook()
{
	// Get ClientMode and CInput
	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClient))[10] + 0x5));
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[15] + 0x1);

	m_pClientModeHook = new VTableHook((DWORD*)dwClientMode, true);
	m_pOverrideView = (OverrideView_t)m_pClientModeHook->Hook(18, (DWORD*)hk_OverrideView);
	m_pCreateMove = (CreateMove_t)m_pClientModeHook->Hook(24, (DWORD*)hk_CreateMove);
	
	m_pEngineModelHook = new VTableHook((DWORD*)this->ModelRender(), true);
	m_pDrawModelExecute = (DrawModelExecute_t)m_pEngineModelHook->Hook(21, (DWORD*)hk_DrawModelExecute);

	m_pClientHook = new VTableHook((DWORD*) this->m_pClient, true);
	m_pFrameStageNotify = (FrameStageNotify_t)m_pClientHook->Hook(36, (DWORD*)hk_FrameStageNotify);

	m_pVguiHook = new VTableHook((DWORD*)this->m_pPanel, true);
	m_pPaintTraverse = (PaintTraverse_t)m_pVguiHook->Hook(41, (DWORD*)hk_PaintTraverse);
}

// Singleton
CApplication::CApplication()
{
	m_pWindow = NULL;

	m_pClientModeHook = NULL;
	m_pEngineModelHook = NULL;
	m_pClientHook = NULL;
	m_pVguiHook = NULL;
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
	Sleep(1000);
	FreeLibraryAndExitThread((HMODULE)pParam, 0);
}