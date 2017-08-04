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

#define RECOIL_COMPENSATION 2
#define RECOIL_TRACKING 0.4499999f

#define ENABLE_SILENTAIM true
#define ENABLE_NOVISRECOIL true
#define ENABLE_THIRDPERSON true

#define PI_F		3.1415f
#define DEG2RAD(x)	((x / 180.0f) * PI_F)
#define RAD2DEG(x)	((x * 180.0f) / PI_F)

class CVerifiedUserCmd;
class CInput
{
public:
	void*               pvftable;                     //0x00
	bool                m_fTrackIRAvailable;          //0x04
	bool                m_fMouseInitialized;          //0x05
	bool                m_fMouseActive;               //0x06
	bool                m_fJoystickAdvancedInit;      //0x07
	char                pad_0x08[0x2C];               //0x08
	void*               m_pKeys;                      //0x34
	char                pad_0x38[0x64];               //0x38
	int					pad_0x41;
	int					pad_0x42;
	bool                m_fCameraInterceptingMouse;   //0x9C
	bool                m_fCameraInThirdPerson;       //0x9D
	bool                m_fCameraMovingWithMouse;     //0x9E
	Vector				m_vecCameraOffset;            //0xA0
	bool                m_fCameraDistanceMove;        //0xAC
	int                 m_nCameraOldX;                //0xB0
	int                 m_nCameraOldY;                //0xB4
	int                 m_nCameraX;                   //0xB8
	int                 m_nCameraY;                   //0xBC
	bool                m_CameraIsOrthographic;       //0xC0
	Vector              m_angPreviousViewAngles;      //0xC4
	Vector              m_angPreviousViewAnglesTilt;  //0xD0
	float               m_flLastForwardMove;          //0xDC
	int                 m_nClearInputState;           //0xE0
	char                pad_0xE4[0x8];                //0xE4
	CUserCmd*           m_pCommands;                  //0xEC
	CVerifiedUserCmd*   m_pVerifiedCommands;          //0xF0
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