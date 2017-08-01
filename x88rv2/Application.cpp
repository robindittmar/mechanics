#include "Application.h"
#include "Vector.h"

CreateMove_t CApplication::m_pCreateMove;
EndScene_t CApplication::m_pEndScene;
DrawIndexedPrimitive_t CApplication::m_pDrawIndexedPrimitive;
FrameStageNotify_t CApplication::m_pFrameStageNotify;

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

	pApp->m_Bhop.Update(pUserCmd);

	return rtn;
}

bool enable = false;
int wait = 0;
HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	if (pEngineClient->IsInGame())
	{
		//pApp->m_Bhop.Update();

		// this needs to go into paintTraverse hook because of flickering because of multirendering
		pApp->m_Esp.Update(device);

		pApp->m_Misc.NoFlash(40);
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
	CApplication* pApp = CApplication::Instance();

	if (wait == 0 && GetAsyncKeyState(VK_INSERT))
	{
		enable = !enable;
		wait++;
	}
	
	if (wait > 0)
	{
		wait++;
		if (wait == 600)
		{
			wait = 0;
		}
	}

	if (curStage == FRAME_RENDER_START)
	{
		IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
		if (pApp->EngineClient()->IsInGame())
		{
			if (enable) //todo: NoRecoil active
			{
				if (GetAsyncKeyState(0x01)) {
					int shotsFired = *(int*)((DWORD)pLocalEntity + SHOTSFIRED_OFFSET);
					if (shotsFired > 1) {
						pApp->EngineClient()->GetViewAngles(pApp->m_ViewAngle);
						Vector3 aimPunchAngle = *(Vector3*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));

						pApp->m_ViewAngle.x += (pApp->m_OldAimPunchAngle.x - aimPunchAngle.x * RECOIL_COMPENSATION);
						pApp->m_ViewAngle.y += (pApp->m_OldAimPunchAngle.y - aimPunchAngle.y * RECOIL_COMPENSATION);

						if (true)//todo: NoVisRecoil inactive sons SetViewAngles in CreateMove (cmd)
						{
							pApp->EngineClient()->SetViewAngles(pApp->m_ViewAngle);
						}

						pApp->m_OldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
						pApp->m_OldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;
					}
					else {
						pApp->m_OldAimPunchAngle.x = 0;
						pApp->m_OldAimPunchAngle.y = 0;
					}
				}
				else {
					pApp->m_OldAimPunchAngle.x = 0;
					pApp->m_OldAimPunchAngle.y = 0;
				}
			}

			if (false) //todo: NoVisRecoil active
			{
				Vector3* aimPunch = (Vector3*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));
				*aimPunch = { 0, 0, 0 };

				Vector3* viewPunch = (Vector3*)((DWORD)pLocalEntity + (LOCAL_OFFSET + VIEWPUNCHANGLE_OFFSET));
				*viewPunch = { 0, 0, 0 };
			}
		}
	}
	pApp->FrameStageNotify()(ecx, curStage);
}

void CApplication::Setup()
{
	CXorString clientDll("tgì§y«¦{g");
	CXorString engineDll("reâ«yn«¦{g");
	CXorString createInterface("Tyà£cnÌ¬cn÷¤vhà");
	CXorString VEngineClient("ANë¥~eà{bà¬c;´ö");
	CXorString VClient("AHé«reñò&3");
	CXorString VClientEntityList("AHé«reñ‡yì¶nGì±c;µñ");

	clientDll.Xor();
	engineDll.Xor();
	createInterface.Xor();
	VEngineClient.Xor();
	VClient.Xor();
	VClientEntityList.Xor();

	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClientDll = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);

	this->m_Bhop.Setup();
	this->m_Esp.Setup();
	this->m_Misc.Setup();

	this->m_Bhop.IsEnabled(true);
	this->m_Esp.IsEnabled(true);
	//this->m_Misc.IsEnabled(true);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) { }
}

void CApplication::Hook()
{
	IDirect3DDevice9* dwDevice = (IDirect3DDevice9*)**(DWORD**)((
		(DWORD)CPattern::FindPattern(
		(BYTE*)(GetModuleHandle("shaderapidx9.dll")),
			0xC1000,
			(BYTE*)"\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44",
			"a----abcdefghasdfta"
		)
		) + 1);

	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClientDll))[10] + 0x5));

	VFTableHook clientModeHook((DWORD*)dwClientMode, true);
	m_pCreateMove = (CreateMove_t)clientModeHook.Hook(24, (DWORD*)hk_CreateMove);

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (PDWORD)hk_EndScene);
	m_pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)d3dHook.Hook(82, (DWORD*)hk_DrawIndexPrimitive);

	VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
	m_pFrameStageNotify = (FrameStageNotify_t)clientHook.Hook(36, (DWORD*)hk_FrameStageNotify);
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