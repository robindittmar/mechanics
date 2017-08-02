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

double DEG2RAD(double degrees) {
	return degrees * 4.0 * atan(1.0) / 180.0;
}
void FixMovement(CUserCmd* pUserCmd)
{
	CApplication* pApp = CApplication::Instance();

	QAngle oldAngles;
	pApp->EngineClient()->GetViewAngles(oldAngles);

	float oldForwardmove = pUserCmd->forwardmove;
	float oldSidemove = pUserCmd->sidemove;
	float deltaView = pUserCmd->viewangles[1] - oldAngles.y;

	float f1;
	float f2;

	if (oldAngles.y < 0.f)
		f1 = 360.0f + oldAngles.y;
	else
		f1 = oldAngles.y;

	if (pUserCmd->viewangles[1] < 0.0f)
		f2 = 360.0f + pUserCmd->viewangles[1];
	else
		f2 = pUserCmd->viewangles[1];

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pUserCmd->forwardmove = cos(DEG2RAD(deltaView)) * oldForwardmove + cos(DEG2RAD(deltaView + 90.f)) * oldSidemove;
	pUserCmd->sidemove = sin(DEG2RAD(deltaView)) * oldForwardmove + sin(DEG2RAD(deltaView + 90.f)) * oldSidemove;
	if (pUserCmd->viewangles[0] >= 180 && pUserCmd->viewangles[0] <= 270) pUserCmd->forwardmove = -pUserCmd->forwardmove;
}

bool __fastcall CApplication::hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = m_pCreateMove(ecx, fInputSampleTime, pUserCmd);

	float view_forward = 0;
	float view_right = 10;

	CApplication* pApp = CApplication::Instance();
	pApp->m_bSetClientViewAngles = false;
	pApp->m_bAimbotNoRecoil = false;

	pApp->Aimbot()->Update(pUserCmd);
	pApp->m_bhop.Update(pUserCmd);


	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
	DWORD moveType = *(DWORD*)((DWORD)pLocalEntity + 0x258);

	if (pUserCmd->buttons & IN_ATTACK)
	{
		pApp->m_misc.NoRecoil(pUserCmd);
	}
	else if (!(pUserCmd->buttons & IN_USE) &&
		!(moveType & MOVETYPE_LADDER))
	{
		Antiaim antiaim = { DOWN, BACKWARDS, pUserCmd };
		pApp->m_antiaim.Update(&antiaim);
	}

	FixMovement(pUserCmd);


	return pApp->m_bSetClientViewAngles;
}

HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	if (pEngineClient->IsInGame())
	{
		// this needs to go into paintTraverse hook because of flickering because of multirendering
		pApp->m_esp.Update(device);

		pApp->m_misc.NoFlash(20);
	}

	return m_pEndScene(device);
}

HRESULT __stdcall CApplication::hk_DrawIndexPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount)
{
	// TODO: Maybe Use IVModelRender::DrawModelExecute(...)

	return m_pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, PrimitiveCount);
}

void __fastcall CApplication::hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	m_pFrameStageNotify(ecx, curStage);
}

void __fastcall CApplication::hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup) {
	CApplication* pApp = CApplication::Instance();

	//todo: FOV changer ;)
	pViewSetup->fov = 105;

	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
	if (pApp->EngineClient()->IsInGame())
	{
		if (ENABLE_NOVISRECOIL)
		{
			QAngle punchAngles = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));
			QAngle viewPunch = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + VIEWPUNCHANGLE_OFFSET));

			pViewSetup->angles.x -= (viewPunch.x + punchAngles.x * RECOIL_COMPENSATION * RECOIL_TRACKING);
			pViewSetup->angles.y -= (viewPunch.y + punchAngles.y * RECOIL_COMPENSATION * RECOIL_TRACKING);
		}
	}
	return m_pOverrideView(ecx, pViewSetup);
}

void __fastcall CApplication::hk_DrawModelExecute(void* ecx, void* edx,IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);
		if (strstr(pszModelName, "arms") != NULL &&
			true) //todo: check if nohands
		{
			
		}
	}

	m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
}

void CApplication::Setup()
{
	CXorString clientDll("tgì§y«¦{g");
	CXorString engineDll("reâ«yn«¦{g");
	CXorString createInterface("Tyà£cnÌ¬cn÷¤vhà");
	CXorString VEngineClient("ANë¥~eà{bà¬c;´ö");
	CXorString VClient("AHé«reñò&3");
	CXorString VClientEntityList("AHé«reñ‡yì¶nGì±c;µñ");
	CXorString VModelInfo("ü˜…‘Ï¹£›Ìº©™Ã°„šåÞ", 0x1235AFAA);
	CXorString VModelRender("ANë¥~eàxoà®':³");
	CXorString EngineTraceClient("_cajthRq{nc@vdcmn=67", 0x1A);

	clientDll.Xor();
	engineDll.Xor();
	createInterface.Xor();
	VEngineClient.Xor();
	VClient.Xor();
	VClientEntityList.Xor();
	VModelInfo.Xor();
	VModelRender.Xor();
	EngineTraceClient.Xor();

	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClientDll = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)CreateEngineInterface(VModelInfo.ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)CreateEngineInterface(VModelRender.ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)CreateEngineInterface(EngineTraceClient.ToCharArray(), NULL);

	this->m_aimbot.Setup();
	this->m_antiaim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_misc.Setup();

	this->m_aimbot.IsEnabled(true);
	this->m_bhop.IsEnabled(true);
	this->m_esp.IsEnabled(true);
	this->m_misc.IsEnabled(true);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) {}
}

void CApplication::Hook()
{
	IDirect3DDevice9* dwDevice = (IDirect3DDevice9*)**(DWORD**)((
		(DWORD)CPattern::FindPattern(
		(BYTE*)(GetModuleHandle("shaderapidx9.dll")),
			0xC1000,
			(BYTE*)"\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44",
			"f----fbcdefghasdfta"
		)
		) + 1);

	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClientDll))[10] + 0x5));

	VFTableHook clientModeHook((DWORD*)dwClientMode, true);
	m_pCreateMove = (CreateMove_t)clientModeHook.Hook(24, (DWORD*)hk_CreateMove);
	m_pOverrideView = (OverrideView_t)clientModeHook.Hook(18, (DWORD*)hk_OverrideView);

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (DWORD*)hk_EndScene);
	m_pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)d3dHook.Hook(82, (DWORD*)hk_DrawIndexPrimitive);

	VFTableHook engineModelHook((DWORD*)this->ModelRender(), true);
	m_pDrawModelExecute = (DrawModelExecute_t)engineModelHook.Hook(21, (DWORD*)hk_DrawModelExecute);

	/*VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
	m_pFrameStageNotify = (FrameStageNotify_t)clientHook.Hook(36, (DWORD*)hk_FrameStageNotify);*/
}

// Singleton
CApplication::CApplication()
{
}

CApplication::CApplication(CApplication const&)
{
}

CApplication::~CApplication()
{
}