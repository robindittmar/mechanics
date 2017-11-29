#ifndef __AIMBOT_H__
#define __AIMBOT_H__

// Source SDK
#include "Vector.h"
#include "UserCmd.h"
#include "ClientEntityList.h"
#include "IVEngineClient.h"
#include "IVModelInfo.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"
#include "ray.h"
#include "CWeapon.h"

// Custom
#include "IFeature.h"
#include "TargetSelector.h"
#include "CreateMove.h"

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

class CApplication;

class CRagebot : public IFeature
{
public:
	CRagebot();
	virtual ~CRagebot();

	/*// Returns wether or not the Aimbot has got a target
	bool HasTarget() { return m_bHasTarget; }
	// Gets the selected target (if none == -1)
	int SelectedTarget() { return m_iSelectedTarget; }
	// Returns the Aim angles for the current tick
	QAngle* GetAimAngles() { return &m_qAimAngles; }*/
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

	void SetAutoRevolver(bool bAutoRevolver) { m_bAutoRevolver = bAutoRevolver; };
	bool GetAutoRevolver() { return m_bAutoRevolver; };
	bool DoingAutoRevolver() { return m_bDoingAutoRevolver; };

	bool IsAbleToApplyNoSpread();
	bool IsNoSpread();

	void AutoRevolver(CUserCmd* pUserCmd);

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;
private:
	//bool CanHit(Vector &point, float *damage_given);

	// Called each 'Update' (resets m_bIsShooting, m_bDidNoRecoil, etc)
	void inline ResetTickVariables();
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
	CTarget* m_pTarget;

	CTargetSelector* m_pTargetSelector;
	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
};

#endif // __AIMBOT_H__