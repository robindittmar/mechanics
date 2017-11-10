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
	QAngle QuadraticBezier(QAngle& p0, QAngle& p1, QAngle& p2, float t);

	float m_fTimeToAim;

	bool m_bHasTarget;
	int m_iTarget;
	
	QAngle m_qStart;
	QAngle m_qEnd;
};

#endif // __LEGITBOT_H__