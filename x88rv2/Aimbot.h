#ifndef __AIMBOT_H__
#define __AIMBOT_H__

#include "IFeature.h"
#include "Vector.h"
#include "UserCmd.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"
#include "CWeapon.h"

#define HITGROUP_GENERIC		0
#define HITGROUP_HEAD			1
#define HITGROUP_CHEST			2
#define HITGROUP_STOMACH		3
#define HITGROUP_LEFTARM		4
#define HITGROUP_RIGHTARM		5
#define HITGROUP_LEFTLEG		6
#define HITGROUP_RIGHTLEG		7
#define HITGROUP_GEAR			10

struct FireBulletData
{
	FireBulletData(const Vector &eye_pos, IClientEntity* pLocal = NULL)
		: src(eye_pos),
		filter((IHandleEntity*)pLocal)
	{
	}

	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilterSkipEntity    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

// Defines how to choose the next target for the aimbot
// -> Origin: Chooses the target that's standing closest to you
// -> Viewangle: Choose the target that's closest to your crosshair
#define TARGETCRITERIA_UNSPECIFIED				0
#define TARGETCRITERIA_ORIGIN					1
#define TARGETCRITERIA_VIEWANGLES				2

struct AimbotUpdateParam
{
	float fInputSampleTime;
	CUserCmd* pUserCmd;
};

class IVEngineClient;
class IClientEntityList;

class CAimbot : public IFeature
{
public:
	CAimbot();
	~CAimbot();

	// Returns wether or not the Aimbot has got a target
	bool HasTarget() { return m_bHasTarget; }
	// Gets the selected target (if none == -1)
	int SelectedTarget() { return m_iSelectedTarget; }
	// Returns the Aim angles for the current tick
	QAngle* GetAimAngles() { return &m_qAimAngles; }
	// Returns wether or not the Aimbot will shoot in this tick
	bool IsShooting() { return m_bIsShooting; }
	// Returns wether or not the aimbot compensated for recoil
	bool DidNoRecoil() { return m_bDidNoRecoil; }
	
	void SetAutoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; }
	bool GetAutoshoot() { return m_bAutoshoot; }

	void SetAutoscope(bool bAutoscope) { m_bAutoscope = bAutoscope; }
	bool GetAutoscope() { return m_bAutoscope; }

	void SetSilentAim(bool bSilentAim) { m_bSilentAim = bSilentAim; }
	bool GetSilentAim() { return m_bSilentAim; }

	void SetNoRecoil(bool bDoNoRecoil) { m_bDoNoRecoil = bDoNoRecoil; }
	bool GetNoRecoil() { return m_bDoNoRecoil; }

	void SetTargetCriteria(int iTargetCriteria) { m_iTargetCriteria = iTargetCriteria; }
	int GetTargetCriteria() { return m_iTargetCriteria; }

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	float GetSpeed() { return m_fSpeed; }
	
	void SetFov(float fFov) { m_fFov = fFov; }
	float GetFov() { return m_fFov; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	float CAimbot::GetDamage(const Vector& point);
private:
	QAngle CalcAngle(Vector& vStartPos, Vector& vEndPos);
	bool CanHit(Vector &point, float *damage_given);


	float GetOriginDist(Vector& a, Vector& b);
	float GetViewangleDist(QAngle& a, QAngle& b/*, float fOriginDistance*/);

	// Called each 'Update' (resets m_bIsShooting, m_bDidNoRecoil, etc)
	void inline ResetTickVariables();
	// fInputSampleTime for predictions
	bool ChooseTarget(float fInputSampleTime, CUserCmd* pUserCmd);
	void ApplyNoRecoil(IClientEntity* pLocalEntity);
	void ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity);
	void inline Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime);
	void inline Aim(CUserCmd* pUserCmd);

	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;

	bool m_bHasTarget;
	int m_iSelectedTarget;
	int m_iTargetBone;
	float m_fDamage;
	QAngle m_qAimAngles;

	bool m_bIsShooting;
	bool m_bDidNoRecoil;

	bool m_bAutoshoot;
	bool m_bAutoscope;
	bool m_bSilentAim;
	bool m_bDoNoRecoil;

	int m_iTargetCriteria;
	float m_fSpeed;
	float m_fFov;
};

#endif // __AIMBOT_H__