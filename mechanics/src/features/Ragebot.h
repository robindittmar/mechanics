#ifndef __AIMBOT_H__
#define __AIMBOT_H__

// Source SDK
#include "../source_sdk/Vector.h"
#include "../source_sdk/UserCmd.h"
#include "../source_sdk/ClientEntityList.h"
#include "../source_sdk/IVEngineClient.h"
#include "../source_sdk/IVModelInfo.h"
#include "../source_sdk/IEngineTrace.h"
#include "../source_sdk/CGameTrace.h"
#include "../source_sdk/ray.h"
#include "../source_sdk/CWeapon.h"

// Custom
#include "Autowall.h"
#include "IFeature.h"
#include "../hooks/CreateMove.h"
#include "../core/Target.h"

#define HITGROUP_GENERIC		0
#define HITGROUP_HEAD			1
#define HITGROUP_CHEST			2
#define HITGROUP_STOMACH		3
#define HITGROUP_LEFTARM		4
#define HITGROUP_RIGHTARM		5
#define HITGROUP_LEFTLEG		6
#define HITGROUP_RIGHTLEG		7
#define HITGROUP_GEAR			10

enum Hitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

#define BONE_USED_BY_HITBOX		0x00000100

// Defines how to choose the next target for the aimbot
// -> Origin: Chooses the target that's standing closest to you
// -> Viewangle: Choose the target that's closest to your crosshair
#define TARGETCRITERIA_UNSPECIFIED				0
#define TARGETCRITERIA_ORIGIN					1
#define TARGETCRITERIA_VIEWANGLES				2

// Take LAST 'TARGETCRITERIA_*' value and add 1
#define TARGETCRITERIA_COUNT					3

// Defines how to check for visibility
// -> ignore: no visibility check at all
// -> canhit: check if can do dmg
// -> fullvisible: fully visibile
#define VISIBLEMODE_IGNORE						0
#define VISIBLEMODE_CANHIT						1
#define VISIBLEMODE_FULLVISIBLE					2

#define TARGET_HITBOX_HEAD						0
#define TARGET_HITBOX_CHEST						1
#define TARGET_HITBOX_PELVIS					2
#define TARGET_HITBOX_RIGHT_FOREARM				3
#define TARGET_HITBOX_LEFT_FOREARM				4
#define TARGET_HITBOX_RIGHT_CALF				5
#define TARGET_HITBOX_LEFT_CALF					6

#define TARGET_HITBOX_COUNT						7

class CApplication;

class CRagebot : public IFeature
{
public:
	CRagebot();
	virtual ~CRagebot();

	// Returns pointer to currently selected target (check idx for ==-1 to validate)
	CTarget* GetSelectedTarget() { return m_iSelectedTarget != -1 ? &m_target : nullptr; }
	// Gets the selected target (if none == -1)
	int GetSelectedTargetIdx() { return m_iSelectedTarget; }
	// Is ragebot doing autorevolver?
	bool IsDoingAutoRevolver() { return m_bDoingAutoRevolver; }
	// Returns the Aim angles for the current tick
	QAngle* GetAimAngles() { return &m_qAimAngles; }
	// Returns wether or not the Aimbot will shoot in this tick
	bool IsShooting() { return m_bIsShooting; }

	void SetAutoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; }
	bool GetAutoshoot() { return m_bAutoshoot; }

	void SetAutoscope(bool bAutoscope) { m_bAutoscope = bAutoscope; }
	bool GetAutoscope() { return m_bAutoscope; }

	void SetSilentAim(bool bSilentAim) { m_bSilentAim = bSilentAim; }
	bool GetSilentAim() { return m_bSilentAim; }
	
	void SetCalculateHitchance(bool bCalculateHitchance) { m_bCalculateHitchance = bCalculateHitchance; }
	bool GetCalculateHitchance() { return m_bCalculateHitchance; }

	void SetHitchance(float fHitchance) { m_fHitchance = fHitchance / 100.0f; }
	float GetHitchance() { return m_fHitchance * 100.0f; }

	void SetMinDamage(float fMinDamage) { m_fMinDamage = fMinDamage; }
	float GetMinDamage() { return m_fMinDamage; }

	void SetAutoReload(bool bAutoReload) { m_bAutoReload = bAutoReload; }
	bool GetAutoReload() { return m_bAutoReload; }

	void SetAutoZeus(bool bAutoZeus) { m_bAutoZeus = bAutoZeus; }
	bool GetAutoZeus() { return m_bAutoZeus; }

	void SetTargetCriteria(int iTargetCriteria) { m_iTargetCriteria = iTargetCriteria; }
	int GetTargetCriteria() { return m_iTargetCriteria; }

	void SetAutoRevolver(bool bAutoRevolver) { m_bAutoRevolver = bAutoRevolver; }
	bool GetAutoRevolver() { return m_bAutoRevolver; }

	void SetVisibleMode(int iVisibleMode) { m_iVisibleMode = iVisibleMode; }
	int GetVisibleMode() { return m_iVisibleMode; }

	void SetCheckHitbox(int iHitbox, bool bCheck) { m_bCheckHitbox[iHitbox] = bCheck; }
	bool GetCheckHitbox(int iHitbox) { return m_bCheckHitbox[iHitbox]; }

	void SetMultipoint(bool bMultipoint) { m_bMultipoint = bMultipoint; }
	bool GetMultipoint() { return m_bMultipoint; }

	void SetMultipointScale(float fMultipointScale) { m_fMultipointScale = fMultipointScale; }
	float GetMultipointScale() { return m_fMultipointScale; }

	bool IsAbleToApplyNoSpread();
	bool IsNoSpread();

	void AutoRevolver(CUserCmd* pUserCmd);

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;
private:
	//bool CanHit(Vector &point, float *damage_given);

	// Called each 'Update' (resets m_bIsShooting, m_bDidNoRecoil, etc)
	void inline ResetTickVariables();
	void SelectTarget(float fInputSampleTime);
	// fInputSampleTime for predictions
	void ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity, bool bAbleToHit);
	void inline Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime);
	void inline Aim(CUserCmd* pUserCmd);

	bool CalculateHitchance(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, IClientEntity* pTarget);
	
	float m_fDamage;
	QAngle m_qAimAngles;

	bool m_bIsShooting;

	bool m_bAutoshoot;
	bool m_bAutoscope;
	bool m_bSilentAim;
	bool m_bCalculateHitchance;
	float m_fHitchance;
	float m_fMinDamage;
	bool m_bAutoReload;
	bool m_bAutoZeus;
	bool m_bAutoRevolver;
	bool m_bDoingAutoRevolver;

	int m_iTargetCriteria;
	int m_iVisibleMode;
	bool m_bMultipoint;
	float m_fMultipointScale;

	int m_iHitboxes[TARGET_HITBOX_COUNT];
	bool m_bCheckHitbox[TARGET_HITBOX_COUNT];

	int m_iSelectedTarget;
	CTarget m_target;

	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
};

#endif // __AIMBOT_H__