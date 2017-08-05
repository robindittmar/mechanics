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
#define DEADFLAG_OFFSET 0x31C4
#define ISSCOPED_OFFSET 0x389C
#define WEAPONID_OFFSET 0x2EE8

#define RECOIL_COMPENSATION 2
#define RECOIL_TRACKING 0.4499999f

#define ENABLE_AUTOSCOPE true
#define ENABLE_AUTOSHOOT false
#define ENABLE_SILENTAIM true
#define ENABLE_NOVISRECOIL true
#define ENABLE_THIRDPERSON true

#define PI_F		3.1415f
#define DEG2RAD(x)	((x / 180.0f) * PI_F)
#define RAD2DEG(x)	((x * 180.0f) / PI_F)

enum WeaponIndex : int {
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

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

typedef bool(__thiscall* CreateMove_t)(void*, float, CUserCmd*);
typedef HRESULT(__stdcall* EndScene_t)(IDirect3DDevice9*);
typedef HRESULT(__stdcall* DrawIndexedPrimitive_t)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
typedef void(__thiscall *FrameStageNotify_t)(void*, ClientFrameStage_t);
typedef void(__thiscall *OverrideView_t)(void*, CViewSetup*);
typedef void* (__thiscall *DrawModelExecute_t)(void*, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld);

void FixMovement(CUserCmd* pUserCmd, QAngle& qOrigAngles);
void NormalizeAngles(CUserCmd* pUserCmd);
void ClampAngles(CUserCmd* pUserCmd);

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

	IVModelInfo* ModelInfo() {
		return m_pModelInfo;
	}

	IVModelRender* ModelRender() {
		return m_pModelRender;
	}

	IEngineTrace* EngineTrace() {
		return m_pEngineTrace;
	}

	IMaterialSystem* MaterialSystem() {
		return m_pMaterialSystem;
	}

	DWORD ClientDll() {
		return m_dwClientDll;
	}

	DWORD EngineDll() {
		return m_dwEngineDll;
	}

	DWORD MaterialSystemDll() {
		return m_dwMaterialSystemDll;
	}

	FrameStageNotify_t FrameStageNotify() {
		return m_pFrameStageNotify;
	}

	CAimbot* Aimbot() {
		return (CAimbot*)&m_aimbot;
	}

	CAntiAim* AntiAim() {
		return (CAntiAim*)&m_antiAim;
	}

	CBhop* Bhop() {
		return (CBhop*)&m_bhop;
	}

	CEsp* Esp() {
		return (CEsp*)&m_esp;
	}

	CMisc* Misc() {
		return (CMisc*)&m_misc;
	}

	QAngle& ClientViewAngles() {
		return m_qClientViewAngles;
	}

	void ClientViewAngles(QAngle& q) {
		m_qClientViewAngles = q;
	}

	QAngle m_oldAimPunchAngle;
	QAngle m_viewAngle;
	//bool m_bSetClientViewAngles;
	bool m_bAimbotNoRecoil;

	static bool __fastcall hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd);
	static HRESULT __stdcall hk_EndScene(IDirect3DDevice9* device);
	static HRESULT __stdcall hk_DrawIndexPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount);
	static void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);
	static void __fastcall hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup);
	static void __fastcall hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
private:
	void Setup();
	void Hook();

	static CreateMove_t m_pCreateMove;
	static EndScene_t m_pEndScene;
	static DrawIndexedPrimitive_t m_pDrawIndexedPrimitive;
	static FrameStageNotify_t m_pFrameStageNotify;
	static OverrideView_t m_pOverrideView;
	static DrawModelExecute_t m_pDrawModelExecute;

	IVEngineClient* m_pEngineClient;
	IBaseClientDLL* m_pClientDll;
	IClientEntityList* m_pEntityList;
	IVModelInfo* m_pModelInfo;
	IVModelRender* m_pModelRender;
	IEngineTrace* m_pEngineTrace;
	IMaterialSystem* m_pMaterialSystem;
	CInput* m_pInput;

	DWORD m_dwClientDll;
	DWORD m_dwEngineDll;
	DWORD m_dwMaterialSystemDll;

	QAngle m_qClientViewAngles;
	QAngle m_qLastTickAngles;

	CAimbot m_aimbot;
	CAntiAim m_antiAim;
	CBhop m_bhop;
	CEsp m_esp;
	CMisc m_misc;

	CWindow* m_pWindow;

	// Singleton 
	CApplication();
	CApplication(CApplication const&);
	~CApplication();

	void operator=(CApplication const&) {}
};

#endif // __APPLICATION_H__