#ifndef __AIMBOT_H__
#define __AIMBOT_H__

#include "Feature.h"
#include "Vector.h"

// Defines how to choose the next target for the aimbot
// -> Origin: Chooses the target that's standing closest to you
// -> Viewangle: Choose the target that's closest to your crosshair
enum TargetCriteria_t
{
	TargetCriteriaUnspecified		= 0,
	TargetCriteriaOrigin			= 1,
	TargetCriteriaViewangle			= 2
};

class CAimbot : public IFeature
{
public:
	CAimbot();
	~CAimbot();

	bool IsShooting() { return m_bIsShooting; }
	bool DidNoRecoil() { return m_bDidNoRecoil; }

	void IsAutoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; }
	bool IsAutoshoot() { return m_bAutoshoot; }

	void IsAutoscope(bool bAutoscope) { m_bAutoscope = bAutoscope; }
	bool IsAutoscope() { return m_bAutoscope; }

	void IsSilentAim(bool bSilentAim) { m_bSilentAim = bSilentAim; }
	bool IsSilentAim() { return m_bSilentAim; }

	void DoNoRecoil(bool bDoNoRecoil) { m_bDoNoRecoil = bDoNoRecoil; }
	bool DoNoRecoil() { return m_bDoNoRecoil; }

	void TargetCriteria(TargetCriteria_t tTargetCriteria) { m_tTargetCriteria = tTargetCriteria; }
	TargetCriteria_t TargetCriteria() { return m_tTargetCriteria; }

	void Speed(float fSpeed) { m_fSpeed = fSpeed; }
	float Speed() { return m_fSpeed; }
	
	void Fov(float fFov) { m_fFov = fFov; }
	float Fov() { return m_fFov; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	QAngle CalcAngle(Vector& relativeDist);

	float GetOriginDist(Vector& a, Vector& b);
	float GetViewangleDist(QAngle& a, QAngle& b, float fOriginDistance);

	bool m_bIsShooting;
	bool m_bDidNoRecoil;

	bool m_bAutoshoot;
	bool m_bAutoscope;
	bool m_bSilentAim;
	bool m_bDoNoRecoil;

	TargetCriteria_t m_tTargetCriteria;
	float m_fSpeed;
	float m_fFov;
};

#endif // __AIMBOT_H__