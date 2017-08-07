#include "Application.h"
#include "Vector.h"
#include <math.h>

CreateMove_t CApplication::m_pCreateMove;
EndScene_t CApplication::m_pEndScene;
DrawIndexedPrimitive_t CApplication::m_pDrawIndexedPrimitive;
FrameStageNotify_t CApplication::m_pFrameStageNotify;
OverrideView_t CApplication::m_pOverrideView;
DrawModelExecute_t CApplication::m_pDrawModelExecute;

CApplication* CApplication::Instance()
{
	static CApplication inst;
	return &inst;
}

void CApplication::Run()
{
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

			//pApp->m_bSetClientViewAngles = false;
			pApp->m_bAimbotNoRecoil = false;

			// Update Aimbot
			pApp->Aimbot()->Update(pUserCmd);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update NoRecoil
			pApp->Misc()->NoRecoil(pUserCmd);
			// Update Fakelag
			pApp->Misc()->Fakelag(pUserCmd);

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

HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	if (pEngineClient->IsInGame())
	{
		// this needs to go into paintTraverse hook because of flickering because of multirendering
		pApp->Esp()->Update(device);

		pApp->Visuals()->NoFlash();
	}

	//CButton btn(100, 100);
	//btn.Draw(device, 10, 10);
	pApp->m_pWindow->Draw(device);
	return m_pEndScene(device);
}

HRESULT __stdcall CApplication::hk_DrawIndexPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount)
{
	// TODO: Maybe Use IVModelRender::DrawModelExecute(...)

	return m_pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);
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
		}
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

	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(materialSystemDll.ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateMaterialSystemInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClientDll = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)CreateEngineInterface(VModelInfo.ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)CreateEngineInterface(VModelRender.ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)CreateEngineInterface(EngineTraceClient.ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)CreateMaterialSystemInterface(VMaterialSystem.ToCharArray(), NULL);

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

	// AA, Bhop, ESP
	this->m_antiAim.IsEnabled(true);
	this->m_bhop.IsEnabled(true);
	this->m_esp.IsEnabled(true);

	// Misc
	this->m_misc.IsEnabled(true);
	this->m_misc.IsNoRecoil(true);
	this->m_misc.IsFakelag(false);

	// Visuals
	this->m_visuals.IsEnabled(true);
	this->m_visuals.IsNoSmoke(false);
	this->m_visuals.HandsDrawStyle(HandsDrawStyleWireframe);
	this->m_visuals.IsNoVisualRecoil(true);

	this->m_visuals.IsNoFlash(true);
	this->m_visuals.NoFlashPercentage(0.2f);

	this->m_visuals.IsThirdperson(false);
	this->m_visuals.ThirdpersonValue(120);

	this->m_visuals.IsFovChange(true);
	this->m_visuals.FovValue(105);
	this->m_visuals.IsFovChangeScoped(false);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) {}
}

void CApplication::Hook()
{
	IDirect3DDevice9* dwDevice = (IDirect3DDevice9*)**(DWORD**)(
		(DWORD)CPattern::FindPattern(
		(BYTE*)(GetModuleHandle("shaderapidx9.dll")),
			0xC1000,
			(BYTE*)"\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44",
			"f----fbcdefghasdfta"
		) + 1
		);

	this->m_bSendPackets = (bool*)(this->EngineDll() + 0xE5E3A) + 1;


	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClientDll))[10] + 0x5));
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClientDll))[15] + 0x1);

	VFTableHook clientModeHook((DWORD*)dwClientMode, true);
	m_pCreateMove = (CreateMove_t)clientModeHook.Hook(24, (DWORD*)hk_CreateMove);
	m_pOverrideView = (OverrideView_t)clientModeHook.Hook(18, (DWORD*)hk_OverrideView);

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (DWORD*)hk_EndScene);
	//m_pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)d3dHook.Hook(82, (DWORD*)hk_DrawIndexPrimitive);

	VFTableHook engineModelHook((DWORD*)this->ModelRender(), true);
	m_pDrawModelExecute = (DrawModelExecute_t)engineModelHook.Hook(21, (DWORD*)hk_DrawModelExecute);

	VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
	m_pFrameStageNotify = (FrameStageNotify_t)clientHook.Hook(36, (DWORD*)hk_FrameStageNotify);
}

// Singleton
CApplication::CApplication()
{
	m_pWindow = NULL;
}

CApplication::CApplication(CApplication const&)
{
}

CApplication::~CApplication()
{
	if (m_pWindow)
		delete m_pWindow;
}


void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
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

	Normalize(vecViewForward);
	Normalize(vecViewRight);
	Normalize(vecViewUp);

	Vector vecForwardNorm = vecViewForward * flForward;
	Vector vecRightNorm = vecViewRight * flRight;
	Vector vecUpNorm = vecViewUp * flUp;

	pCmd->forwardmove = vecForwardNorm.Dot(vecAimForward) + vecRightNorm.Dot(vecAimForward) + vecUpNorm.Dot(vecAimForward);
	pCmd->sidemove = vecForwardNorm.Dot(vecAimRight) + vecRightNorm.Dot(vecAimRight) + vecUpNorm.Dot(vecAimRight);
	pCmd->upmove = vecForwardNorm.Dot(vecAimUp) + vecRightNorm.Dot(vecAimUp) + vecUpNorm.Dot(vecAimUp);

	ClampMovement(pCmd);
}
void Normalize(Vector angle)
{
	// Normalize pitch
	while (angle.x > 89.0f)
	{
		angle.x -= 178.0f;
	}
	while (angle.x < -89.0f)
	{
		angle.x += 178.0f;
	}

	// Normalize yaw
	while (angle.y > 179.9999f)
	{
		angle.y -= 360.0f;
	}
	while (angle.y < -179.9999f)
	{
		angle.y += 360.0f;
	}

	angle.z = 0.0f;
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