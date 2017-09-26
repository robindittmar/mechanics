#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Std lib
#include <cmath>

// Custom
#include "Console.h"
#include "Pattern.h"
#include "XorString.h"
#include "VTableHook.h"
#include "PlayerList.h"
#include "Gui.h"
#include "InputHandler.h"
#include "Controls.h"
#include "Menu.h"
#include "GameEventListener.h"
#include "ResourceManager.h"
#include "NetVarManager.h"
#include "Offsets.h"
#include "MathDefs.h"
#include "TargetSelector.h"

// Features
#include "Ragebot.h"
#include "Triggerbot.h"
#include "Antiaim.h"
#include "Bhop.h"
#include "Esp.h"
#include "SoundEsp.h"
#include "Chams.h"
#include "Misc.h"
#include "Skinchanger.h"
#include "Visuals.h"
#include "Mirror.h"

// Source SDK
#include "CreateInterface.h"
#include "IVEngineClient.h"
#include "IBaseClientDLL.h"
#include "ClientEntity.h"
#include "ClientEntityList.h"
#include "ClientFrameStage.h"
#include "ICvar.h"
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
#include "Vector.h"
#include "IMatRenderContext.h"
#include "IViewRender.h"
#include "IEngineSound.h"

#define OFFSET_LOCAL 0x2FAC
#define OFFSET_AIMPUNCHANGLE 0x70
#define OFFSET_VIEWPUNCHANGLE 0x64
#define OFFSET_GLOBALS 0x1B

#define RECOIL_TRACKING 0.4499999f

#define CLIENTDLL_SIZE			0x50E5000
#define ENGINEDLL_SIZE			0x8C7000
#define MATERIALSYSTEMDLL_SIZE	0x10C000

typedef void(_cdecl* RandomSeed_t)(int);
typedef int(_cdecl* RandomInt_t)(int, int);
typedef float(_cdecl* RandomFloat_t)(float, float);

typedef bool(__thiscall *CreateMove_t)(void*, float, CUserCmd*);
typedef void(__thiscall *FrameStageNotify_t)(void*, ClientFrameStage_t);
typedef void(__thiscall *OverrideView_t)(void*, CViewSetup*);
typedef void(__thiscall *DrawModelExecute_t)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
typedef void(__thiscall *PlaySound_t)(void*, const char*);
typedef float(__thiscall *GetViewModelFov_t)(void*);
typedef bool(__thiscall *FireEventClientSide_t)(void*, IGameEvent*);
typedef void(__thiscall *RenderView_t)(void*, const CViewSetup&, const CViewSetup&, int, int);
typedef void(__thiscall *RenderSmokeOverlay_t)(void*, bool);
typedef int(__thiscall *EmitSound1_t)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, soundlevel_t, int, int, int, const Vector*, const Vector*, CUtlVector<Vector>*, bool, float, int);
typedef int(__thiscall *EmitSound2_t)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const Vector*, const Vector*, CUtlVector<Vector>*, bool, float, int);

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

	// TODO: REMOVE
	void LoadSkinChangerConfig();
	// TODO: REMOVE

	// Hook helper
	bool GetInitialHookDone() { return m_bInitialHookDone; }
	bool GetHooked() { return m_bIsHooked; }

	void Unhook();
	void Rehook();

	void SetRecoilCompensation(float recoilCompensation) { m_flRecoilCompensation = recoilCompensation; }
	float GetRecoilCompensation() { return m_flRecoilCompensation; }

	IClientEntity* GetLocalPlayer(bool bGetTargetIfLocalDead = false);

	// VTable Hooks
	VTableHook* ClientModeHook() { return m_pClientModeHook; }
	VTableHook* ModelRenderHook() { return m_pModelRenderHook; }
	VTableHook* ClientHook() { return m_pClientHook; }
	VTableHook* PanelHook() { return m_pPanelHook; }
	VTableHook* GameEventManagerHook() { return m_pGameEventManagerHook; }
	VTableHook* ViewRenderHook() { return m_pViewRenderHook; }
	VTableHook* EngineSound() { return m_pEngineSoundHook; }

	RandomSeed_t RandomSeed() { return m_pRandomSeed; }
	RandomInt_t RandomInt() { return m_pRandomInt; }
	RandomFloat_t RandomFloat() { return m_pRandomFloat; }

	// Exposed callable engine functions
	CreateMove_t CreateMove() { return m_pCreateMove; }
	FrameStageNotify_t FrameStageNotify() { return m_pFrameStageNotify; }
	OverrideView_t OverrideView() { return m_pOverrideView; }
	DrawModelExecute_t DrawModelExecute() { return m_pDrawModelExecute; }
	PaintTraverse_t PaintTraverse() { return m_pPaintTraverse; }
	PlaySound_t PlaySound() { return m_pPlaySound; }
	GetViewModelFov_t GetViewModelFov() { return m_pGetViewModelFov; }
	FireEventClientSide_t FireEventClientSide() { return m_pFireEventClientSide; }
	RenderView_t RenderViewFn() { return m_pRenderViewFn; }
	RenderSmokeOverlay_t RenderSmokeOverlay() { return m_pRenderSmokeOverlay; }

	RecvVarProxy_t SequenceProxy() { return m_pSequenceProxy; }

	InitKeyValues_t InitKeyValues() { return m_pInitKeyValues; }
	LoadFromBuffer_t LoadFromBuffer() { return m_pLoadFromBuffer; }

	// Engine Pointer
	IVEngineClient* EngineClient() { return m_pEngineClient; }
	IBaseClientDLL* BaseClient() { return m_pClient; }
	IClientEntityList* EntityList() { return m_pEntityList; }
	IVModelInfo* ModelInfo() { return m_pModelInfo; }
	IVModelRender* ModelRender() { return m_pModelRender; }
	IVRenderView* OnRenderView() { return m_pRenderView; }
	IEngineTrace* EngineTrace() { return m_pEngineTrace; }
	IMaterialSystem* MaterialSystem() { return m_pMaterialSystem; }
	CInput* Input() { return m_pInput; }
	IPanel* Panel() { return m_pPanel; }
	ISurface* Surface() { return m_pSurface; }
	CGlobalVars* GlobalVars() { return m_pGlobalVars; }
	IGameEventManager2* GameEventManager() { return m_pGameEventManager; }
	IPhysicsSurfaceProps* PhysicsSurfaceProps() { return m_pPhysicsSurfaceProps; }
	IClientState* ClientState() { return m_pClientState; }
	ICVar* CVar() { return m_pCVar; }
	IViewRender* ViewRender() { return m_pViewRender; }

	// DLL Addresses
	DWORD ClientDll() { return m_dwClientDll; }
	DWORD EngineDll() { return m_dwEngineDll; }
	DWORD MaterialSystemDll() { return m_dwMaterialSystemDll; }
	DWORD VGui2Dll() { return m_dwVGui2Dll; }
	DWORD VGuiSurfaceDll() { return m_dwVGuiSurfaceDll; }
	DWORD VPhysicsDll() { return m_dwVPhysicsDll; }

	// Target selector (Feature?)
	CPlayerList* PlayerList() { return &m_playerList; }
	CTargetSelector* TargetSelector() { return &m_targetSelector; }

	// Features
	CRagebot* Ragebot() { return &m_ragebot; }
	CTriggerbot* Triggerbot() { return &m_triggerbot; }
	CAntiAim* AntiAim() { return &m_antiAim; }
	CBhop* Bhop() { return &m_bhop; }
	CEsp* Esp() { return &m_esp; }
	CSoundEsp* SoundEsp() { return &m_soundEsp; }
	CChams* Chams() { return &m_chams; }
	CMisc* Misc() { return &m_misc; }
	CSkinChanger* SkinChanger() { return &m_skinchanger; }
	CVisuals* Visuals() { return &m_visuals; }
	CMirror* Mirror() { return &m_mirror; }

	// Resource Manager
	CResourceManager* ResourceManager() { return m_pResourceManager; }

	// Gui
	CGui* Gui() { return m_pGui; }
	CMenu* Menu() { return m_pMenu; }

	// Client ViewAngles
	QAngle& GetClientViewAngles() { return m_qClientViewAngles; }
	void SetClientViewAngles(QAngle& q) { m_qClientViewAngles = q; }

	// Viewangles of last tick
	QAngle LastTickAngles() { return m_qLastTickAngles; }

	QAngle m_oldAimPunchAngle;
	QAngle m_viewAngle;
	bool* m_bSendPackets;
	bool m_bGotSendPackets;

	//todo: change!!!
	bool m_bLBY;
	float m_flOldLby;
	float m_flLbyUpdateTime;
	float m_flRealLbyUpdateTime;
	float m_flPredLbyUpdateTime;
	bool m_bLbyUpdate;

	static bool __fastcall hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd);
	static void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);
	static void __fastcall hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup);
	static void __fastcall hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	static void __fastcall hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	static void __fastcall hk_PlaySound(void* ecx, void* edx, const char* fileName);
	static float __fastcall hk_GetViewModelFov(void* ecx, void* edx);
	static bool __fastcall hk_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent);
	static void __fastcall hk_RenderView(void* ecx, void* edx, const CViewSetup& view, CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);
	static void __fastcall hk_RenderSmokeOverlay(void* ecx, void* edx, bool bUnknown);
	static int __fastcall hk_EmitSound1(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
		float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector<Vector>* pUtlVecOrigins = NULL,
		bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1);
	static int __fastcall hk_EmitSound2(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector<Vector>* pUtlVecOrigins = NULL,
		bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1);
	static void __cdecl hk_SetViewModelSequence(const CRecvProxyData* ecx, void* pStruct, void* pOut);
private:
	void Setup();
	void Hook();

	HMODULE m_hModule;

	float m_flRecoilCompensation;

	RandomSeed_t m_pRandomSeed;
	RandomInt_t m_pRandomInt;
	RandomFloat_t m_pRandomFloat;

	bool m_bInitialHookDone;
	bool m_bIsHooked;
	VTableHook* m_pClientModeHook;
	VTableHook* m_pModelRenderHook;
	VTableHook* m_pClientHook;
	VTableHook* m_pPanelHook;
	VTableHook* m_pSurfaceHook;
	VTableHook* m_pGameEventManagerHook;
	VTableHook* m_pViewRenderHook;
	VTableHook* m_pEngineSoundHook;

	static CreateMove_t m_pCreateMove;
	static FrameStageNotify_t m_pFrameStageNotify;
	static OverrideView_t m_pOverrideView;
	static DrawModelExecute_t m_pDrawModelExecute;
	static PaintTraverse_t m_pPaintTraverse;
	static PlaySound_t m_pPlaySound;
	static GetViewModelFov_t m_pGetViewModelFov;
	static FireEventClientSide_t m_pFireEventClientSide;
	static RenderView_t m_pRenderViewFn;
	static RenderSmokeOverlay_t m_pRenderSmokeOverlay;
	static EmitSound1_t m_pEmitSound1;
	static EmitSound2_t m_pEmitSound2;

	static RecvVarProxy_t m_pSequenceProxy;

	InitKeyValues_t m_pInitKeyValues;
	LoadFromBuffer_t m_pLoadFromBuffer;

	// NetVars
	CNetVarManager* m_pNetVarMgr;
	CNetVar* m_pNetVarSequence;

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
	IClientState* m_pClientState;
	ICVar* m_pCVar;
	IViewRender* m_pViewRender;
	IEngineSound* m_pEngineSound;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;
	DWORD m_dwMaterialSystemDll;
	DWORD m_dwVGui2Dll;
	DWORD m_dwVGuiSurfaceDll;
	DWORD m_dwVPhysicsDll;
	DWORD m_dwVStdLibDll;

	QAngle m_qClientViewAngles;
	QAngle m_qLastTickAngles;

	CPlayerList m_playerList;
	CTargetSelector m_targetSelector;

	// Features
	CRagebot m_ragebot;
	CTriggerbot m_triggerbot;
	CAntiAim m_antiAim;
	CBhop m_bhop;
	CEsp m_esp;
	CSoundEsp m_soundEsp;
	CChams m_chams;
	CMisc m_misc;
	CSkinChanger m_skinchanger;
	CVisuals m_visuals;
	CMirror m_mirror;

	// Event listener
	CGameEventListener m_gameEventListener;

	// Resource Manager
	CResourceManager* m_pResourceManager;

	// Hack GUI
	CGui* m_pGui;
	CMenu* m_pMenu;

	// Singleton
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__