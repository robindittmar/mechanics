#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//
#include "Console.h"
#include "Pattern.h"

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
//#include "SourceEngine.h"
#include "VFTableHook.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(__stdcall* EndScene_t)(IDirect3DDevice9* device);
extern EndScene_t p_EndScene;

extern IVEngineClient* pEngineClient;
extern IBaseClientDLL* pClientDll;
extern IClientEntityList* pEntityList;

class CApplication
{
public:
	CApplication();
	~CApplication();

	void Run();

	static HRESULT __stdcall hk_EndScene(IDirect3DDevice9* device);
private:
	void Setup();
	void Hook();
};

#endif // __APPLICATION_H__