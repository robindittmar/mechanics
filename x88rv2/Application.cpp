#include "Application.h"
#include "Vector.h"

EndScene_t CApplication::m_pEndScene;

CApplication* CApplication::Instance()
{
	static CApplication inst;
	return &inst;
}

void CApplication::Run()
{
	this->Setup();
	this->Hook();
}

bool enable = false;
int wait = 0;
HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	if (pEngineClient->IsInGame())
	{
		pApp->m_Bhop.Update();

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

	// TODO: TEMPORARY, NOT FINISHED CODE, UNTESTED, NOT COMPILED
	DWORD createMove = (DWORD)(CPattern::FindPattern(
		(BYTE*)GetModuleHandle("client.dll"),
		0xFFFF, // TODO: Keine Ahnung wie groß client.dll ist, kannste @ollydbg z.B. in der modulübersicht nachgucken afaik
		(BYTE*)"\xA3\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x59\xC3\x6A\x00",
		"g----gf--------e----abc-"
	) + 1); // +1 weil wegen \xA3
	// TODO: TEMPORARY, NOT FINISHED CODE, UNTESTED, NOT COMPILED

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (PDWORD)hk_EndScene);
	m_pDrawIndexedPrimitive = (DrawIndexedPrimitive_t)d3dHook.Hook(82, (PDWORD)hk_);

	VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
	m_pFrameStageNotify = (FrameStageNotify_t)clientHook.Hook(36, (PDWORD)hk_FrameStageNotify);
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