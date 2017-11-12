#ifndef __LEGITBOT_H__
#define __LEGITBOT_H__

// Source SDK
#include "Vector.h"
#include "ISurface.h"

// Custom
#include "IFeature.h"

class CLegitbot : public IFeature
{
public:
	CLegitbot();
	~CLegitbot();

	void SetTimeToAim(float fTimeToAim) { m_fTimeToAim = fTimeToAim; }
	float GetTimeToAim() { return m_fTimeToAim; }

	void SetDrawTarget(bool bDrawTarget) { m_bDrawTarget = bDrawTarget; }
	bool GetDrawTarget() { return m_bDrawTarget; }

	virtual void Update(void* pParameters = 0) override;
	void DrawTarget(ISurface* pSurface);
private:
	QAngle QuadraticBezier(QAngle& p0, QAngle& p1, QAngle& p2, float t);

	float m_fTimeToAim;

	float m_fStepSize;

	bool m_bHasTarget;
	int m_iTarget;
	float m_fAimProgress;

	bool m_bDrawTarget;
	bool m_bHasDrawTarget;

	QAngle m_qStart;
	QAngle m_qIntermediate;
	QAngle m_qEnd;
};

#endif // __LEGITBOT_H__