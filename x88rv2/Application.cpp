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

int iFlash = 0;
HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	if (pEngineClient->IsInGame())
	{
		if (true) //todo: bhop active
		{
			pApp->m_Bhop.Update();
		}

		// this needs to go into paintTraverse hook because of flickering because of multirendering
		// maybe w2s function broken
		if (true) //todo: esp active
		{
			pApp->m_Esp.Update(device);
		}

		if (iFlash == 0) //todo: remove | workaround only 1 time
		{
			pApp->m_Misc.NoFlash(30);
			iFlash++;
		}
	}

	return m_pEndScene(device);
}

void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CApplication* pApp = CApplication::Instance();

	if (curStage == FRAME_RENDER_START)
	{
		IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
		if (pApp->EngineClient()->IsInGame())
		{
			if (true) //todo: NoRecoil active
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

			if (true) //todo: NoVisRecoil active
			{
				Vector3 aimPunch = *(Vector3*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));
				aimPunch = { 0, 0, 0 };

				Vector3* viewPunch = (Vector3*)((DWORD)pLocalEntity + (LOCAL_OFFSET + VIEWPUNCHANGLE_OFFSET));
				*viewPunch = { 0, 0, 0 };
			}
		}
	}
	pApp->fnFrameStageNotify()(ecx, curStage);
}

void CApplication::Setup()
{
	this->m_dwClientDll = (DWORD)GetModuleHandle("client.dll");
	this->m_dwEngineDll = (DWORD)GetModuleHandle("engine.dll");
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, "CreateInterface");
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, "CreateInterface");

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface("VEngineClient014", NULL);
	m_pClientDll = (IBaseClientDLL*)CreateClientInterface("VClient018", NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface("VClientEntityList003", NULL);

	this->m_Bhop.Setup();
	this->m_Esp.Setup();
	this->m_Misc.Setup();
}

void CApplication::Hook()
{
	IDirect3DDevice9* dwDevice = (IDirect3DDevice9*)**(DWORD**)((
		(DWORD)CPattern::FindPattern(
		(BYTE*)(GetModuleHandle("shaderapidx9.dll")),
			0xC1000,
			(BYTE*)"\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44",
			"a----abcdefghasdfaa"
		)
		) + 1);

	VFTableHook d3dHook((DWORD*)dwDevice, true);
	m_pEndScene = (EndScene_t)d3dHook.Hook(42, (PDWORD)hk_EndScene);

	VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
	m_fnFrameStageNotify = (tFrameStageNotify)clientHook.Hook(36, (PDWORD)hk_FrameStageNotify);
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