#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Std lib
#include <cmath>

// Custom
#include "utils/Console.h"
#include "utils/Config.h"
#include "utils/ConfigHelper.h"
#include "utils/Pattern.h"
#include "utils/XorString.h"
#include "utils/VMTHook.h"
#include "core/PlayerList.h"
#include "gui/Gui.h"
#include "gui/InputHandler.h"
#include "gui/Controls.h"
#include "gui/Menu.h"
#include "core/GameEventListener.h"
#include "core/ResourceManager.h"
#include "core/NetVarManager.h"
#include "core/Offsets.h"
#include "utils/MathDefs.h"
#include "utils/Benchmark.h"

// Hooks
#include "hooks/CreateMove.h"
#include "hooks/FrameStageNotify.h"
#include "hooks/OverrideView.h"
#include "hooks/DrawModelExecute.h"
#include "hooks/PaintTraverse.h"
#include "hooks/PlaySound.h"
#include "hooks/GetViewModelFov.h"
#include "hooks/FireEventClientSide.h"
#include "hooks/RenderView.h"
#include "hooks/RenderSmokeOverlay.h"
#include "hooks/EmitSound.h"
#include "hooks/FindMdl.h"
#include "hooks/LockCursor.h"

// Proxies
#include "proxies/SequenceProxy.h"
#include "proxies/LowerBodyYawProxy.h"
#include "proxies/EyeAnglesProxy.h"

// Features
#include "features/Ragebot.h"
#include "features/Legitbot.h"
#include "features/Triggerbot.h"
#include "features/AntiAim.h"
#include "features/Bhop.h"
#include "features/Esp.h"
#include "features/WeaponEsp.h"
#include "features/SoundEsp.h"
#include "features/Chams.h"
#include "features/Misc.h"
#include "features/Resolver.h"
#include "features/SkinChanger.h"
#include "features/Visuals.h"
#include "features/MaterialVisuals.h"
#include "features/GunHud.h"
#include "features/Mirror.h"
#include "features/LagCompensation.h"
#include "features/Fakelag.h"
#include "features/Fakewalk.h"
#include "features/Fakeduck.h"
#include "features/Radar.h"
#include "features/GunAccuracy.h"

// Source SDK
#include "source_sdk/CreateInterface.h"
#include "source_sdk/IVEngineClient.h"
#include "source_sdk/IBaseClientDLL.h"
#include "source_sdk/ClientEntity.h"
#include "source_sdk/ClientEntityList.h"
#include "source_sdk/ClientFrameStage.h"
#include "source_sdk/ICVar.h"
#include "source_sdk/UserCmd.h"
#include "source_sdk/IVModelInfo.h"
#include "source_sdk/IEngineTrace.h"
#include "source_sdk/IVModelRender.h"
#include "source_sdk/IMaterialSystem.h"
#include "source_sdk/CInput.h"
#include "source_sdk/CWeapon.h"
#include "source_sdk/IPanel.h"
#include "source_sdk/ISurface.h"
#include "source_sdk/IGameEventManager.h"
#include "source_sdk/IPhysicsSurfaceProps.h"
#include "source_sdk/CViewSetup.h"
#include "source_sdk/CGlobalVars.h"
#include "source_sdk/IViewRenderBeams.h"
#include "source_sdk/IVModelRender.h"
#include "source_sdk/IVRenderView.h"
#include "source_sdk/KeyValues.h"
#include "source_sdk/Vector.h"
#include "source_sdk/IMatRenderContext.h"
#include "source_sdk/IViewRender.h"
#include "source_sdk/IEngineSound.h"
#include "source_sdk/checksum_md5.h"
#include "source_sdk/IClientMode.h"
#include "source_sdk/IMDLCache.h"
#include "source_sdk/ILocalize.h"
#include "source_sdk/IInputSystem.h"

#define OFFSET_GLOBALS					0x1B
#define OFFSET_LASTOCCLUSIONCHECK		0xA30
#define OFFSET_OCCLUSIONFLAGS			0xA28

#define RECOIL_TRACKING					0.4499999f

#define CLIENTDLL_SIZE					0x50E5000
#define ENGINEDLL_SIZE					0x8C7000
#define MATERIALSYSTEMDLL_SIZE			0x10C000

typedef void(__cdecl *ConColorMsg_t)(Color const&, const char*, ...);

typedef void(__cdecl *RandomSeed_t)(int);
typedef int(__cdecl *RandomInt_t)(int, int);
typedef float(__cdecl *RandomFloat_t)(float, float);

typedef void(__thiscall *InitKeyValues_t)(KeyValues*, const char*);
typedef void(__thiscall *LoadFromBuffer_t)(KeyValues*, const char*, const char*, void*, const char*, void*, void*);

void CorrectMovement(CUserCmd* pUserCmd, QAngle& qOrigAngles);
void NormalizeAngles(CUserCmd* pUserCmd);
void ClampMovement(CUserCmd* pUserCmd);

DWORD __stdcall ThreadFreeLibrary(void* pParam);

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

	/// <summary>
	/// Creates single directory (non recursive)
	/// 
	/// Does nothing if directory already exists
	/// </summary>
	/// <param name="pName">Name of folder</param>
	/// <returns>False when error occuurs</returns>
	bool CreateFolder(const char* pName);

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

	ConColorMsg_t	ConColorMsg()					{ return m_pConColorMsg; }

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
	IInputSystem*			InputSystem()			{ return m_pInputSystem; }

	// DLL Addresses
	DWORD Tier0Dll()								{ return m_dwTier0Dll; }
	DWORD ClientDll()								{ return m_dwClientDll; }
	DWORD EngineDll()								{ return m_dwEngineDll; }
	DWORD MaterialSystemDll()						{ return m_dwMaterialSystemDll; }
	DWORD VGui2Dll()								{ return m_dwVGui2Dll; }
	DWORD VGuiSurfaceDll()							{ return m_dwVGuiSurfaceDll; }
	DWORD VPhysicsDll()								{ return m_dwVPhysicsDll; }
	DWORD DataCacheDll()							{ return m_dwDatacacheDll; }
	DWORD LocalizeDll()								{ return m_dwLocalizeDll; }
	DWORD InputSystemDll()							{ return m_dwInputSystemDll; }

	// Target selector (Feature?)
	CPlayerList* PlayerList()						{ return &m_playerList; }

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
	CFakeduck*          Fakeduck()                  { return &m_fakeduck; }
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

	// Creates necessary directories
	void CreateSubFolders();

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

	float m_fRenderFieldOfView;

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

	ConColorMsg_t m_pConColorMsg;

	RandomSeed_t m_pRandomSeed;
	RandomInt_t m_pRandomInt;
	RandomFloat_t m_pRandomFloat;

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
	IInputSystem* m_pInputSystem;

	DWORD m_dwTier0Dll;
	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;
	DWORD m_dwMaterialSystemDll;
	DWORD m_dwVGui2Dll;
	DWORD m_dwVGuiSurfaceDll;
	DWORD m_dwVPhysicsDll;
	DWORD m_dwVStdLibDll;
	DWORD m_dwDatacacheDll;
	DWORD m_dwLocalizeDll;
	DWORD m_dwInputSystemDll;

	QAngle m_qClientViewAngles;
	QAngle m_qLastTickAngles;

	CPlayerList m_playerList;

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
	CFakeduck m_fakeduck;
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