#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//
#include "Console.h"
#include "Pattern.h"
#include "XorString.h"

// Features
#include "Aimbot.h"
#include "Bhop.h"
#include "ESP.h"
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

#define LOCAL_OFFSET 0x2FAC
#define AIMPUNCHANGLE_OFFSET 0x70
#define VIEWPUNCHANGLE_OFFSET 0x64
#define SHOTSFIRED_OFFSET 0x0A2C0

#define RECOIL_COMPENSATION 2

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,            // (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

struct Vector3 {
	float x, y, z;
};


typedef HRESULT(__stdcall* EndScene_t)(IDirect3DDevice9* device);
typedef void(__thiscall *tFrameStageNotify)(void*, ClientFrameStage_t curStage);

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

	tFrameStageNotify fnFrameStageNotify() {
		return m_fnFrameStageNotify;
	}

	QAngle m_OldAimPunchAngle;
	QAngle m_ViewAngle;

	static HRESULT __stdcall hk_EndScene(IDirect3DDevice9* device);
private:
	void Setup();
	void Hook();

	// TODO: CreateMove, UpdateCmd(?)
	static EndScene_t m_pEndScene;
	tFrameStageNotify m_fnFrameStageNotify;

	IVEngineClient* m_pEngineClient;
	IBaseClientDLL* m_pClientDll;
	IClientEntityList* m_pEntityList;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;

	CBhop m_Bhop;
	CMisc m_Misc;
	CEsp m_Esp;

	// Singleton 
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__