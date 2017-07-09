#include "Application.h"

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

HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	IClientEntityList* pEntityList = pApp->EntityList();

	static CConsole console;

	if (pEngineClient->IsInGame())
	{
		int iCountClients = pEntityList->GetMaxEntities();
		IClientEntity* pLocalEntity = pEntityList->GetClientEntity(pEngineClient->GetLocalPlayer());
		IClientEntity* pEntity;
		for (int i = 1; i < iCountClients; i++)
		{
			pEntity = pEntityList->GetClientEntity(i);

			if (!pEntity)
				continue;

			if (i == pEngineClient->GetLocalPlayer())
				continue;

			bool isDormant = *(bool*)((DWORD)pEntity + 0xE9);
			if (!isDormant)
				continue;

			int iHealth = *(int*)((DWORD)pEntity + 0xFC);
			if (iHealth == 0)
				continue;

			// ...
			//console.Write("%d\n", iHealth);
		}
	}

	return m_pEndScene(device);
}

void CApplication::Setup()
{
	CreateInterfaceFn CreateClientInterface = GetCreateInterfaceFn("client.dll");
	CreateInterfaceFn CreateEngineInterface = GetCreateInterfaceFn("engine.dll");

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface("VEngineClient014", NULL);
	m_pClientDll = (IBaseClientDLL*)CreateClientInterface("VClient018", NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface("VClientEntityList003", NULL);
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