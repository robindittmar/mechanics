#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Custom includes
#include "Console.h"
#include "Pattern.h"
#include "XorString.h"
#include "VTableHook.h"
#include "Gui.h"
#include "InputHandler.h"
#include "Controls.h"
#include "GameEventListener.h"
#include "ResourceManager.h"
#include "NetVarManager.h"
#include "Offsets.h"

// Features
#include "Aimbot.h"
#include "Antiaim.h"
#include "Bhop.h"
#include "ESP.h"
#include "Chams.h"
#include "Misc.h"
#include "Skinchanger.h"
#include "Visuals.h"

// Source Engine
#include "CreateInterface.h"
#include "VEngineClient.h"
#include "IBaseClientDLL.h"
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
#include "CViewSetup.h"
#include "CGlobalVars.h"
#include "IVModelRender.h"
#include "IVRenderView.h"
#include "KeyValues.h"

#define OFFSET_LOCAL 0x2FAC
#define OFFSET_AIMPUNCHANGLE 0x70
#define OFFSET_VIEWPUNCHANGLE 0x64
#define OFFSET_GLOBALS 0x1B

#define RECOIL_COMPENSATION 2
#define RECOIL_TRACKING 0.4499999f

#define PI_F		3.1415f
#define DEG2RAD(x)	((x / 180.0f) * PI_F)
#define RAD2DEG(x)	((x * 180.0f) / PI_F)

#define CLIENTDLL_SIZE	0x50E5000

typedef bool(__thiscall *CreateMove_t)(void*, float, CUserCmd*);
typedef void(__thiscall *FrameStageNotify_t)(void*, ClientFrameStage_t);
typedef void(__thiscall *OverrideView_t)(void*, CViewSetup*);
typedef void(__thiscall *DrawModelExecute_t)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
typedef float(__thiscall* GetViewModelFov_t)(void*);

typedef void(__thiscall *InitKeyValues_t)(KeyValues*, const char*);
typedef void(__thiscall *LoadFromBuffer_t)(KeyValues*, const char*, const char*, void*, const char*, void*);


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
	void Detach();

	// VTable Hooks
	VTableHook* ClientModeHook() { return m_pClientModeHook; }
	VTableHook* EngineModelHook() { return m_pEngineModelHook; }
	VTableHook* ClientHook() { return m_pClientHook; }
	VTableHook* VguiHook() { return m_pVguiHook; }

	// Exposed callable engine functions
	CreateMove_t CreateMove() { return m_pCreateMove; }
	FrameStageNotify_t FrameStageNotify() { return m_pFrameStageNotify; }
	OverrideView_t OverrideView() { return m_pOverrideView; }
	DrawModelExecute_t DrawModelExecute() { return m_pDrawModelExecute; }
	PaintTraverse_t PaintTraverse() { return m_pPaintTraverse; }

	InitKeyValues_t InitKeyValues() { return m_pInitKeyValues; }
	LoadFromBuffer_t LoadFromBuffer() { return m_pLoadFromBuffer; }

	// Engine Pointer
	IVEngineClient* EngineClient() { return m_pEngineClient; }
	IBaseClientDLL* BaseClient() { return m_pClient; }
	IClientEntityList* EntityList() { return m_pEntityList; }
	IVModelInfo* ModelInfo() { return m_pModelInfo; }
	IVModelRender* ModelRender() { return m_pModelRender; }
	IVRenderView* RenderView() { return m_pRenderView; }
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
	CChams* Chams() { return (CChams*)&m_chams; }
	CMisc* Misc() { return (CMisc*)&m_misc; }
	CSkinChanger* SkinChanger() { return (CSkinChanger*)&m_skinchanger; }
	CVisuals* Visuals() { return (CVisuals*)&m_visuals; }

	// Resource Manager
	CResourceManager* ResourceManager() { return m_pResourceManager; }

	// Gui
	CGui* Gui() { return m_pGui; }

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
	static float __fastcall hk_GetViewModelFov(void* ecx, void* edx);
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

	InitKeyValues_t m_pInitKeyValues;
	LoadFromBuffer_t m_pLoadFromBuffer;

	IVEngineClient* m_pEngineClient;
	IBaseClientDLL* m_pClient;
	IClientEntityList* m_pEntityList;
	IVModelInfo* m_pModelInfo;
	IVModelRender* m_pModelRender;
	IVRenderView* m_pRenderView;
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
	CChams m_chams;
	CMisc m_misc;
	CSkinChanger m_skinchanger;
	CVisuals m_visuals;

	// Event listener
	CGameEventListener m_gameEventListener;

	// Resource Manager
	CResourceManager* m_pResourceManager;

	// Hack GUI
	CGui* m_pGui;
	CInputEvent m_inputEvent;
	CInputHandler m_inputHandler;
	CWindow* m_pWindow;

	CCheckbox* m_pGuiThirdpersonCheckbox;

	// Singleton
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__