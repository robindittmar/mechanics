#ifndef __AIMBOT_H__
#define __AIMBOT_H__

#include "Feature.h"

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

	void IsAutoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; };
	bool IsAutoshoot() { return m_bAutoshoot; };

	void IsAutoscope(bool bAutoscope) { m_bAutoscope = bAutoscope; };
	bool IsAutoscope() { return m_bAutoscope; };

	void IsSilentAim(bool bSilentAim) { m_bSilentAim = bSilentAim; };
	bool IsSilentAim() { return m_bSilentAim; };

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	TargetCriteria_t m_tTargetCriteria;
	float m_fSpeed;

	bool m_bAutoshoot;
	bool m_bAutoscope;
	bool m_bSilentAim;
};

#endif // __AIMBOT_H__