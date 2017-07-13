#include "Application.h"

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

HRESULT __stdcall CApplication::hk_EndScene(IDirect3DDevice9* device)
{
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();

	if (pEngineClient->IsInGame())
	{
		pApp->m_Bhop.Update();
	}

	return m_pEndScene(device);
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

	//VFTableHook clientHook((DWORD*) this->m_pClientDll, true);
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