#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//
#include "Console.h"
#include "Pattern.h"

// Features
#include "Aimbot.h"
#include "Bhop.h"
#include "Misc.h"

// Source Engine
#include "CreateInterface.h"
#include "VEngineClient.h"
#include "BaseClientDLL.h"
#include "ClientEntity.h"
#include "ClientEntityList.h"

// DirectX
#include "d3d9.h"
#include "d3dx9.h"

// TODO: Pending to be removed once reworked
#include "VFTableHook.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(__stdcall* EndScene_t)(IDirect3DDevice9* device);

// Singleton
class CApplication
{
public:
	static CApplication* Instance();
	void Run();

	IVEngineClient* EngineClient() {
		return m_pEngineClient;
	}

	IBaseClientDLL* BaseClientDLL() {
		return m_pClientDll;
	}

	IClientEntityList* EntityList() {
		return m_pEntityList;
	}

	DWORD ClientDll() {
		return m_dwClientDll;
	}

	DWORD EngineDll() {
		return m_dwEngineDll;
	}

	static HRESULT __stdcall hk_EndScene(IDirect3DDevice9* device);
private:
	void Setup();
	void Hook();

	// TODO: CreateMove, UpdateCmd(?)
	static EndScene_t m_pEndScene;

	IVEngineClient* m_pEngineClient;
	IBaseClientDLL* m_pClientDll;
	IClientEntityList* m_pEntityList;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;

	CBhop m_Bhop;
	CMisc m_Misc;

	// Singleton 
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__