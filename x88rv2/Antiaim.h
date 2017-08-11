#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "Feature.h"

class CApplication;

enum PitchAntiAims {
	PitchNone,
	PitchUp,
	PitchDown
};

enum YawAntiAims {
	YawNone,
	YawBackwards,
	YawStaticJitterBackwards,
	YawBackwardsFakeRight
};

struct AntiAim {
	PitchAntiAims pitchAA;
	YawAntiAims yawAA;
};

class CAntiAim : public IFeature
{
public:
	CAntiAim();
	~CAntiAim();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
};

#endif // __ANTIAIM_H__