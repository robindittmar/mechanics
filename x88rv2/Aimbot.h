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

	void Autoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; };
	bool Autoshoot() { return m_bAutoshoot; };

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	TargetCriteria_t m_tTargetCriteria;
	float m_fSpeed;

	bool m_bAutoshoot;
};

#endif // __AIMBOT_H__