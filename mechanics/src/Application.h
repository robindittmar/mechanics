#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Std lib
#include <cmath>

// Custom
#include "Console.h"
#include "Config.h"
#include "ConfigHelper.h"
#include "Pattern.h"
#include "XorString.h"
#include "VMTHook.h"
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
#include "Benchmark.h"

// Hooks
#include "CreateMove.h"
#include "FrameStageNotify.h"
#include "OverrideView.h"
#include "DrawModelExecute.h"
#include "PaintTraverse.h"
#include "PlaySound.h"
#include "GetViewModelFov.h"
#include "FireEventClientSide.h"
#include "RenderView.h"
#include "RenderSmokeOverlay.h"
#include "EmitSound.h"
#include "FindMdl.h"

// Proxies
#include "SequenceProxy.h"
#include "LowerBodyYawProxy.h"
#include "EyeAnglesProxy.h"

// Features
#include "Ragebot.h"
#include "Legitbot.h"
#include "Triggerbot.h"
#include "Antiaim.h"
#include "Bhop.h"
#include "Esp.h"
#include "WeaponEsp.h"
#include "SoundEsp.h"
#include "Chams.h"
#include "Misc.h"
#include "Resolver.h"
#include "Skinchanger.h"
#include "Visuals.h"
#include "MaterialVisuals.h"
#include "GunHud.h"
#include "Mirror.h"
#include "LagCompensation.h"
#include "Fakelag.h"
#include "Fakewalk.h"
#include "Radar.h"
#include "GunAccuracy.h"

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
#include "IViewRenderBeams.h"
#include "IVModelRender.h"
#include "IVRenderView.h"
#include "KeyValues.h"
#include "Vector.h"
#include "IMatRenderContext.h"
#include "IViewRender.h"
#include "IEngineSound.h"
#include "checksum_md5.h"
#include "IClientMode.h"
#include "IMDLCache.h"
#include "ILocalize.h"

#define OFFSET_GLOBALS					0x1B
#define OFFSET_LASTOCCLUSIONCHECK		0xA30
#define OFFSET_OCCLUSIONFLAGS			0xA28

#define RECOIL_TRACKING					0.4499999f

#define CLIENTDLL_SIZE					0x50E5000
#define ENGINEDLL_SIZE					0x8C7000
#define MATERIALSYSTEMDLL_SIZE			0x10C000

typedef void(_cdecl* RandomSeed_t)(int);
typedef int(_cdecl* RandomInt_t)(int, int);
typedef float(_cdecl* RandomFloat_t)(float, float);

typedef void(__thiscall *InitKeyValues_t)(KeyValues*, const char*);
typedef void(__thiscall *LoadFromBuffer_t)(KeyValues*, const char*, const char*, void*, const char*, void*);

void CorrectMovement(CUserCmd* pUserCmd, QAngle& qOrigAngles);
void NormalizeAngles(CUserCmd* pUserCmd);
void ClampMovement(CUserCmd* pUserCmd);

DWORD ThreadFreeLibrary(void* pParam);

// TODO
class BulletTracerEntry
{
public:
	QAngle qAngles;
	Vector vStart;
	Vector vEnd;
};

class HitmarkerEntry
{
public:
	Vector vHit;
	float fStarttime;
};

// Singleton
class CApplication
{
public:
	static CApplication* Instance();

	void Run(HMODULE hModule);
	void Detach();

	/// <summary>
	/// Returns working directory (including trailing slash)
	/// </summary>
	/// <returns>Directory where our .dll is stored</returns>
	const char* GetWorkingDirectory()	{ return m_pFilepath; }

	/// <summary>
	/// Returns the filename of our .dll
	/// </summary>
	/// <returns>Filename, including extension (.dll usually)</returns>
	const char* GetFilename()			{ return m_pFilename; }

	// Hook helper
	bool GetInitialHookDone()	{ return m_bInitialHookDone; }
	bool GetHooked()			{ return m_bIsHooked; }

	void Unhook();
	void Rehook();

	void SetRenderFieldOfView(float fRenderFieldOfView)		{ m_fRenderFieldOfView = fRenderFieldOfView; }
	float GetRenderFieldOfView()							{ return m_fRenderFieldOfView; }

	IClientEntity* GetLocalPlayer(bool bGetTargetIfLocalDead = false);

	// VTable Hooks
	VMTHook* ClientModeHook()						{ return m_pClientModeHook; }
	VMTHook* ModelRenderHook()						{ return m_pModelRenderHook; }
	VMTHook* ClientHook()							{ return m_pClientHook; }
	VMTHook* PanelHook()							{ return m_pPanelHook; }
	VMTHook* GameEventManagerHook()					{ return m_pGameEventManagerHook; }
	VMTHook* ViewRenderHook()						{ return m_pViewRenderHook; }
	VMTHook* EngineSoundHook()						{ return m_pEngineSoundHook; }

	RandomSeed_t	RandomSeed()					{ return m_pRandomSeed; }
	RandomInt_t		RandomInt()						{ return m_pRandomInt; }
	RandomFloat_t	RandomFloat()					{ return m_pRandomFloat; }

	InitKeyValues_t		InitKeyValues()				{ return m_pInitKeyValues; }
	LoadFromBuffer_t	LoadFromBuffer()			{ return m_pLoadFromBuffer; }

	// Engine Pointer
	IVEngineClient*			EngineClient()			{ return m_pEngineClient; }
	IBaseClientDLL*			BaseClient()			{ return m_pClient; }
	IClientEntityList*		EntityList()			{ return m_pEntityList; }
	IVModelInfo*			ModelInfo()				{ return m_pModelInfo; }
	IVModelRender*			ModelRender()			{ return m_pModelRender; }
	IVRenderView*			OnRenderView()			{ return m_pRenderView; }
	IEngineTrace*			EngineTrace()			{ return m_pEngineTrace; }
	IMaterialSystem*		MaterialSystem()		{ return m_pMaterialSystem; }
	CInput*					Input()					{ return m_pInput; }
	IPanel*					Panel()					{ return m_pPanel; }
	ISurface*				Surface()				{ return m_pSurface; }
	CGlobalVars*			GlobalVars()			{ return m_pGlobalVars; }
	IGameEventManager2*		GameEventManager()		{ return m_pGameEventManager; }
	IPhysicsSurfaceProps*	PhysicsSurfaceProps()	{ return m_pPhysicsSurfaceProps; }
	IClientMode*			ClientMode()			{ return m_pClientMode; }
	IClientState*			ClientState()			{ return m_pClientState; }
	ICVar*					CVar()					{ return m_pCVar; }
	IViewRender*			ViewRender()			{ return m_pViewRender; }
	IViewRenderBeams*		ViewRenderBeams()		{ return m_pViewRenderBeams; }
	IMDLCache*				MDLCache()				{ return m_pMdlCache; }
	ILocalize*				Localize()				{ return m_pLocalize; }

	// DLL Addresses
	DWORD ClientDll()								{ return m_dwClientDll; }
	DWORD EngineDll()								{ return m_dwEngineDll; }
	DWORD MaterialSystemDll()						{ return m_dwMaterialSystemDll; }
	DWORD VGui2Dll()								{ return m_dwVGui2Dll; }
	DWORD VGuiSurfaceDll()							{ return m_dwVGuiSurfaceDll; }
	DWORD VPhysicsDll()								{ return m_dwVPhysicsDll; }
	DWORD DataCacheDll()							{ return m_dwDatacacheDll; }
	DWORD LocalizeDll()								{ return m_dwLocalizeDll; }

	// Target selector (Feature?)
	CPlayerList* PlayerList()						{ return &m_playerList; }
	CTargetSelector* TargetSelector()				{ return &m_targetSelector; }

	// Features
	CRagebot*			Ragebot()					{ return &m_ragebot; }
	CGunAccuracy*		GunAccuracy()				{ return &m_gunAccuracy; }
	CLegitbot*			Legitbot()					{ return &m_legitbot; }
	CTriggerbot*		Triggerbot()				{ return &m_triggerbot; }
	CAntiAim*			AntiAim()					{ return &m_antiAim; }
	CBhop*				Bhop()						{ return &m_bhop; }
	CEsp*				Esp()						{ return &m_esp; }
	CWeaponEsp*			WeaponEsp()					{ return &m_weaponesp; }
	CSoundEsp*			SoundEsp()					{ return &m_soundEsp; }
	CChams*				Chams()						{ return &m_chams; }
	CMisc*				Misc()						{ return &m_misc; }
	CFakelag*			Fakelag()					{ return &m_fakelag; }
	CFakewalk*			Fakewalk()					{ return &m_fakewalk; }
	CResolver*			Resolver()					{ return &m_resolver; }
	CSkinChanger*		SkinChanger()				{ return &m_skinchanger; }
	CVisuals*			Visuals()					{ return &m_visuals; }
	CMaterialVisuals*	MaterialVisuals()			{ return &m_materialvisuals; }
	CGunHud*			GunHud()					{ return &m_gunHud; }
	CMirror*			Mirror()					{ return &m_mirror; }
	CRadar*				Radar()						{ return &m_radar; }
	CLagCompensation*	LagCompensation()			{ return &m_lagcompensation; }

	// Resource Manager
	CResourceManager* ResourceManager()				{ return m_pResourceManager; }

	// Gui
	CGui* Gui() { return m_pGui; }
	CMenu* Menu() { return m_pMenu; }

	// Client ViewAngles
	void SetClientViewAngles(QAngle& q) { m_qClientViewAngles = q; }
	QAngle& GetClientViewAngles() { return m_qClientViewAngles; }
	
	// Last Tick Angles
	void SetLastTickViewAngles(QAngle& q) { m_qLastTickAngles = q; }
	QAngle& GetLastTickViewAngles() { return m_qLastTickAngles; }

	// Viewangles of last tick
	QAngle LastTickAngles() { return m_qLastTickAngles; }

	QAngle m_oldAimPunchAngle;
	QAngle m_viewAngle;
	bool* m_bSendPackets;
	bool m_bGotSendPackets;

	//todo: change!!!
	bool m_bLBY;
	bool m_bLbyUpdate;

	// TODO!!
	static std::vector<BulletTracerEntry> m_pBulletTracer;
	static std::vector<HitmarkerEntry> m_pHitmarker;
private:
	void Setup();
	void Hook();

	// Own DLL
	void GetModuleInfo();

	// Engine stuff
	void GetLibrarys();
	void GetInterfaces();
	void GetNetVars();

	HMODULE m_hModule;
	
	int m_iLenFilepath;
	char* m_pFilepath;

	int m_iLenFilename;
	char* m_pFilename;

	float m_flRecoilCompensation;
	float m_fRenderFieldOfView;

	RandomSeed_t m_pRandomSeed;
	RandomInt_t m_pRandomInt;
	RandomFloat_t m_pRandomFloat;

	bool m_bInitialHookDone;
	bool m_bIsHooked;
	VMTHook* m_pClientModeHook;
	VMTHook* m_pModelRenderHook;
	VMTHook* m_pClientHook;
	VMTHook* m_pPanelHook;
	VMTHook* m_pSurfaceHook;
	VMTHook* m_pGameEventManagerHook;
	VMTHook* m_pViewRenderHook;
	VMTHook* m_pEngineSoundHook;
	VMTHook* m_pMdlHook;

	InitKeyValues_t m_pInitKeyValues;
	LoadFromBuffer_t m_pLoadFromBuffer;

	// NetVars
	CNetVarManager* m_pNetVarMgr;
	CNetVar* m_pNetVarSequence;
	CNetVar* m_pNetVarLowerBodyYaw;
	CNetVar* m_pNetVarEyeAngles0;
	CNetVar* m_pNetVarEyeAngles1;

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
	IClientMode* m_pClientMode;
	IClientState* m_pClientState;
	ICVar* m_pCVar;
	IViewRender* m_pViewRender;
	IViewRenderBeams* m_pViewRenderBeams;
	IEngineSound* m_pEngineSound;
	IMDLCache* m_pMdlCache;
	ILocalize* m_pLocalize;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;
	DWORD m_dwMaterialSystemDll;
	DWORD m_dwVGui2Dll;
	DWORD m_dwVGuiSurfaceDll;
	DWORD m_dwVPhysicsDll;
	DWORD m_dwVStdLibDll;
	DWORD m_dwDatacacheDll;
	DWORD m_dwLocalizeDll;

	QAngle m_qClientViewAngles;
	QAngle m_qLastTickAngles;

	CPlayerList m_playerList;
	CTargetSelector m_targetSelector;

	// Features
	CRagebot m_ragebot;
	CGunAccuracy m_gunAccuracy;
	CLegitbot m_legitbot;
	CTriggerbot m_triggerbot;
	CResolver m_resolver;
	CAntiAim m_antiAim;
	CBhop m_bhop;
	CEsp m_esp;
	CWeaponEsp m_weaponesp;
	CSoundEsp m_soundEsp;
	CChams m_chams;
	CMisc m_misc;
	CFakelag m_fakelag;
	CFakewalk m_fakewalk;
	CSkinChanger m_skinchanger;
	CVisuals m_visuals;
	CMaterialVisuals m_materialvisuals;
	CGunHud m_gunHud;
	CMirror m_mirror;
	CRadar m_radar;
	CLagCompensation m_lagcompensation;

	// Event listener
	CGameEventListener m_gameEventListener;

	// Resource Manager
	CResourceManager* m_pResourceManager;

	// Hack GUI
	CGui* m_pGui;
	CMenu* m_pMenu;

	// Singleton
	CApplication();
	CApplication(const CApplication&) = delete;
	~CApplication();
};

#endif // __APPLICATION_H__