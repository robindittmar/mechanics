#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//
#include "Console.h"
#include "Pattern.h"
#include "XorString.h"
#include "Gui.h"

// Features
#include "Aimbot.h"
#include "Antiaim.h"
#include "Bhop.h"
#include "ESP.h"
#include "Misc.h"
#include "Visuals.h"

// Game Event listener
#include "GameEventListener.h"

// Source Engine
#include "CreateInterface.h"
#include "VEngineClient.h"
#include "BaseClientDLL.h"
#include "ClientEntity.h"
#include "ClientEntityList.h"
#include "ClientFrameStage.h"
#include "UserCmd.h"
#include "IVModelInfo.h"
#include "IEngineTrace.h"
#include "IVModelRender.h"
#include "IMaterialSystem.h"
#include "CInput.h"
#include "CWeapon.h"
#include "IPanel.h"
#include "ISurface.h"
#include "IGameEventManager.h"
#include "IPhysicsSurfaceProps.h"

#include "VTableHook.h"

#define OFFSET_LOCAL 0x2FAC
#define OFFSET_AIMPUNCHANGLE 0x70
#define OFFSET_VIEWPUNCHANGLE 0x64
#define OFFSET_GLOBALS 0x1B

#define RECOIL_COMPENSATION 2
#define RECOIL_TRACKING 0.4499999f

#define PI_F		3.1415f
#define DEG2RAD(x)	((x / 180.0f) * PI_F)
#define RAD2DEG(x)	((x * 180.0f) / PI_F)

struct CViewSetup
{
	char _0x0000[16];
	__int32 x;
	__int32 x_old;
	__int32 y;
	__int32 y_old;
	__int32 width;
	__int32    width_old;
	__int32 height;
	__int32    height_old;
	char _0x0030[128];
	float fov;
	float fovViewmodel;
	Vector origin;
	Vector angles;
	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;
	float m_flAspectRatio;
	float m_flNearBlurDepth;
	float m_flNearFocusDepth;
	float m_flFarFocusDepth;
	float m_flFarBlurDepth;
	float m_flNearBlurRadius;
	float m_flFarBlurRadius;
	float m_nDoFQuality;
	__int32 m_nMotionBlurMode;
	char _0x0104[68];
	__int32 m_EdgeBlur;
};
class IClientMode;
class IMatRenderContext;
class DrawModelState_t;

class CGlobalVars
{
public:
	float    realtime;
	int      framecount;
	float    absoluteframetime;
	float    absoluteframestarttimestddev;
	float    curtime;
	float    frametime;
	int      maxClients;
	int      tickcount;
	float    interval_per_tick;
	float    interpolation_amount;
	int      simTicksThisFrame;
	int      network_protocol;
	void*    pSaveData;
	bool     m_bClient;
	int      nTimestampNetworkingBase;
	int      nTimestampRandomizeWindow;
};

typedef bool(__thiscall* CreateMove_t)(void*, float, CUserCmd*);
typedef void(__thiscall *FrameStageNotify_t)(void*, ClientFrameStage_t);
typedef void(__thiscall *OverrideView_t)(void*, CViewSetup*);
typedef void*(__thiscall *DrawModelExecute_t)(void*, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld);
typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);

typedef float(__thiscall* GetViewModelFov_t)(void*);

void CorrectMovement(CUserCmd* pUserCmd, QAngle& qOrigAngles);
void NormalizeAngles(CUserCmd* pUserCmd);
void ClampMovement(CUserCmd* pUserCmd);

DWORD ThreadFreeLibrary(void* pParam);

// Singleton
class CApplication
{
public:
	static CApplication* Instance();
	void Run(HMODULE hModule);

	// VTable Hooks
	VTableHook* ClientModeHook() { return m_pClientModeHook; }
	VTableHook* EngineModelHook() { return m_pEngineModelHook; }
	VTableHook* ClientHook() { return m_pClientHook; }
	VTableHook* VguiHook() { return m_pVguiHook; }

	// Engine Pointer
	IVEngineClient* EngineClient() { return m_pEngineClient; }
	IBaseClientDLL* BaseClient() { return m_pClient; }
	IClientEntityList* EntityList() { return m_pEntityList; }
	IVModelInfo* ModelInfo() { return m_pModelInfo; }
	IVModelRender* ModelRender() { return m_pModelRender; }
	IEngineTrace* EngineTrace() { return m_pEngineTrace; }
	IMaterialSystem* MaterialSystem() { return m_pMaterialSystem; }
	CInput* Input() { return m_pInput; }
	IPanel* Panel() { return m_pPanel; }
	ISurface* Surface() { return m_pSurface; }
	CGlobalVars* GlobalVars() { return m_pGlobalVars; }
	IGameEventManager2* GameEventManager() { return m_pGameEventManager; }
	IPhysicsSurfaceProps* PhysicsSurfaceProps() { return m_pPhysicsSurfaceProps; }

	// DLL Addresses
	DWORD ClientDll() { return m_dwClientDll; }
	DWORD EngineDll() { return m_dwEngineDll; }
	DWORD MaterialSystemDll() { return m_dwMaterialSystemDll; }
	DWORD Vgui2Dll() { return m_dwVgui2Dll; }
	DWORD VguiSurfaceDll() { return m_dwVguiSurfaceDll; }
	DWORD VPhysicsDll() { return m_dwVPhysicsDll; }

	// Features
	CAimbot* Aimbot() { return (CAimbot*)&m_aimbot; }
	CAntiAim* AntiAim() { return (CAntiAim*)&m_antiAim; }
	CBhop* Bhop() { return (CBhop*)&m_bhop; }
	CEsp* Esp() { return (CEsp*)&m_esp; }
	CMisc* Misc() { return (CMisc*)&m_misc; }
	CVisuals* Visuals() { return (CVisuals*)&m_visuals; }

	// Client ViewAngles
	QAngle& ClientViewAngles() { return m_qClientViewAngles; }
	void ClientViewAngles(QAngle& q) { m_qClientViewAngles = q; }

	// Viewangles of last tick
	QAngle LastTickAngles() { return m_qLastTickAngles; }

	QAngle m_oldAimPunchAngle;
	QAngle m_viewAngle;
	bool* m_bSendPackets;
	bool m_bGotSendPackets;

	static bool __fastcall hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd);
	static void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);
	static void __fastcall hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup);
	static void __fastcall hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
	static void __fastcall hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);

	DrawModelExecute_t DrawModelExecute() { return m_pDrawModelExecute; }
private:
	void Setup();
	void Hook();

	HMODULE m_hModule;

	VTableHook* m_pClientModeHook;
	VTableHook* m_pEngineModelHook;
	VTableHook* m_pClientHook;
	VTableHook* m_pVguiHook;

	static CreateMove_t m_pCreateMove;
	static FrameStageNotify_t m_pFrameStageNotify;
	static OverrideView_t m_pOverrideView;
	static DrawModelExecute_t m_pDrawModelExecute;
	static PaintTraverse_t m_pPaintTraverse;

	static GetViewModelFov_t m_pGetViewModelFov;

	IVEngineClient* m_pEngineClient;
	IBaseClientDLL* m_pClient;
	IClientEntityList* m_pEntityList;
	IVModelInfo* m_pModelInfo;
	IVModelRender* m_pModelRender;
	IEngineTrace* m_pEngineTrace;
	IMaterialSystem* m_pMaterialSystem;
	CInput* m_pInput;
	IPanel* m_pPanel;
	ISurface* m_pSurface;
	CGlobalVars* m_pGlobalVars;
	IGameEventManager2* m_pGameEventManager;
	IPhysicsSurfaceProps* m_pPhysicsSurfaceProps;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;
	DWORD m_dwMaterialSystemDll;
	DWORD m_dwVgui2Dll;
	DWORD m_dwVguiSurfaceDll;
	DWORD m_dwVPhysicsDll;

	QAngle m_qClientViewAngles;
	QAngle m_qLastTickAngles;

	// Features
	CAimbot m_aimbot;
	CAntiAim m_antiAim;
	CBhop m_bhop;
	CEsp m_esp;
	CMisc m_misc;
	CVisuals m_visuals;

	// Event listener
	CGameEventListener m_gameEventListener;

	// Hack GUI
	CWindow* m_pWindow;

	// Singleton
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__