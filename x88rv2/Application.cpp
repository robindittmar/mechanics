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
	if (pApp->EngineClient()->IsInGame())
	{
		IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
		int health = *(int*)((DWORD)pLocalEntity + HEALTH_OFFSET);
		if (health > 0) //todo: in IsAlive() funktion umwandeln
		{
			float view_forward = 0;
			float view_right = 10;

			// Save Viewangles before doing stuff
			pApp->EngineClient()->GetViewAngles(pApp->ClientViewAngles());
			QAngle old = pApp->ClientViewAngles();
			float fOldForward = pUserCmd->forwardmove;
			float fOldSidemove = pUserCmd->sidemove;

			//pApp->m_bSetClientViewAngles = false;
			pApp->m_bAimbotNoRecoil = false;

			// Update Aimbot
			pApp->Aimbot()->Update(pUserCmd);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update NoRecoil
			pApp->Misc()->NoRecoil(pUserCmd);

			// Update AntiAim
			pApp->AntiAim()->Update(pUserCmd);

			// Fix movement & angles
			FixMovement(pUserCmd, old);
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
		//pApp->Misc()->NoFlash(80);
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
			int health = *(int*)((DWORD)pLocalEntity + HEALTH_OFFSET);
			if (health > 0) //todo: in IsAlive() funktion umwandeln
			{
				if (true)//todo: check for nosmoke
				{
					static CXorString smoke_materials[] = {
						CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú¤~yà"),
						CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú±zdî§pyà¬voà"),
						CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±"),
						CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±Hbè²vhñ¦bxñ")
					};
					static CXorString pOtherTextures("Xí§e+ñ§oð°rx");

					for (int i = 0; i < sizeof(smoke_materials) / sizeof(*smoke_materials); i++)
					{
						IMaterial* pMat = pApp->MaterialSystem()->FindMaterial(smoke_materials[i].ToCharArray(), pOtherTextures.ToCharArray());
						pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					}
				}

				if (pApp->m_pInput->m_fCameraInThirdPerson)
				{
					*(Vector*)((DWORD)pLocalEntity + DEADFLAG_OFFSET + 0x4) = pApp->m_qLastTickAngles;
				}
			}
		}
	}

	m_pFrameStageNotify(ecx, curStage);
}

void __fastcall CApplication::hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup) {
	CApplication* pApp = CApplication::Instance();

	//todo: FOV changer ;)
	pViewSetup->fov = 105;

	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
	if (pApp->EngineClient()->IsInGame())
	{
		int health = *(int*)((DWORD)pLocalEntity + HEALTH_OFFSET);
		if (health > 0)
		{
			static Vector vecAngles;
			pApp->EngineClient()->GetViewAngles(vecAngles);
			if (ENABLE_THIRDPERSON)
			{
				if (!pApp->m_pInput->m_fCameraInThirdPerson)
				{
					pApp->m_pInput->m_fCameraInThirdPerson = true;
					pApp->m_pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 120); //todo: value
				}
			}
			else
			{
				pApp->m_pInput->m_fCameraInThirdPerson = false;
				pApp->m_pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
			}

			if (ENABLE_NOVISRECOIL)
			{
				QAngle punchAngles = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));
				QAngle viewPunch = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + VIEWPUNCHANGLE_OFFSET));

				pViewSetup->angles.x -= (viewPunch.x + punchAngles.x * RECOIL_COMPENSATION * RECOIL_TRACKING);
				pViewSetup->angles.y -= (viewPunch.y + punchAngles.y * RECOIL_COMPENSATION * RECOIL_TRACKING);
			}
		}
	}
	return m_pOverrideView(ecx, pViewSetup);
}

void __fastcall CApplication::hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	static CXorString pArms("vyè±");
	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");

	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);
		if (strstr(pszModelName, pArms.ToCharArray()) != NULL &&
			true) //todo: check if nohands
		{
			IMaterial* pMat = pApp->MaterialSystem()->FindMaterial(pszModelName, pModelTextures.ToCharArray());
			pMat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			pApp->ModelRender()->ForcedMaterialOverride(pMat);
		}
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

	this->m_aimbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_misc.Setup();

	this->m_aimbot.IsEnabled(true);
	this->m_antiAim.IsEnabled(true);
	this->m_bhop.IsEnabled(true);
	this->m_esp.IsEnabled(true);
	this->m_misc.IsEnabled(true);

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

	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClientDll))[10] + 0x5));
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClientDll))[15] + 0x1);

	VFTableHook clientModeHook((DWORD*)dwClientMode, true);
	m_pCreateMove = (CreateMove_t)clientModeHook.Hook(24, (DWORD*)hk_CreateMove);
	m_pOverrideView = (OverrideView_t)clientModeHook.Hook(18, (DWORD*)hk_OverrideView);

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (DWORD*)hk_EndScene);
	m_pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)d3dHook.Hook(82, (DWORD*)hk_DrawIndexPrimitive);

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

// TODO: This is not working :c
void FixMovement(CUserCmd* pUserCmd, QAngle& qOrigAngles)
{
	CApplication* pApp = CApplication::Instance();

	float oldForwardmove = pUserCmd->forwardmove;
	float oldSidemove = pUserCmd->sidemove;
	float deltaView = pUserCmd->viewangles[1] - qOrigAngles.y;
	float f1;
	float f2;
	if (qOrigAngles.y < 0.f)
		f1 = 360.0f + qOrigAngles.y;
	else
		f1 = qOrigAngles.y;
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
	pUserCmd->sidemove = -(sin(DEG2RAD(deltaView)) * oldForwardmove + sin(DEG2RAD(deltaView + 90.f)) * oldSidemove);

	ClampAngles(pUserCmd);
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
void ClampAngles(CUserCmd* pUserCmd)
{
	// Clamp forward
	if (pUserCmd->forwardmove > 450)
	{
		pUserCmd->forwardmove = 450;
	}
	else if (pUserCmd->forwardmove < -450)
	{
		pUserCmd->forwardmove = -450;
	}

	// Clamp sidemove
	if (pUserCmd->sidemove > 450)
	{
		pUserCmd->sidemove = 450;
	}
	else if (pUserCmd->sidemove < -450)
	{
		pUserCmd->sidemove = -450;
	}
}