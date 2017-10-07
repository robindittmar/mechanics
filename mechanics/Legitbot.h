#ifndef __LEGITBOT_H__
#define __LEGITBOT_H__

// Source SDK
#include "Vector.h"

// Custom
#include "IFeature.h"

class CLegitbot : public IFeature
{
public:
	CLegitbot();
	~CLegitbot();

	void SetTimeToAim(float fTimeToAim) { m_fTimeToAim = fTimeToAim; }
	float GetTimeToAim() { return m_fTimeToAim; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	float m_fTimeToAim;

	bool m_bHasTarget;
	bool m_bDidShoot;
	QAngle m_qTargetAngles;
	int m_iMaxStepsRequired;
	int m_iStepsRequired;
	QAngle m_qAngleStep;

	QAngle m_qPathAngles;
};

#endif // __LEGITBOT_H__