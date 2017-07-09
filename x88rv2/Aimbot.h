#ifndef __AIMBOT_H__
#define __AIMBOT_H__

#include "Feature.h"

// Defines how to choose the next target for the aimbot
// -> Origin: Chooses the target that's standing closest to you
// -> Viewangle: Choose the target that's closest to your crosshair
enum TargetCriteria_t
{
	TargetCriteriaUnspecified = 0,
	TargetCriteriaOrigin = 1,
	TargetCriteriaViewangle = 2
};

class CAimbot : public IFeature
{
public:
	CAimbot();
	~CAimbot();

	virtual void Setup();
	virtual void Update();
private:
	TargetCriteria_t m_tTargetCriteria;
	float m_fSpeed;
};

#endif // __AIMBOT_H__