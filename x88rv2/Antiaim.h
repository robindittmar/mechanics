#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "IFeature.h"

class CApplication;

// Pitch
#define PITCHANTIAIM_NONE						0
#define PITCHANTIAIM_UP							1
#define PITCHANTIAIM_DOWN						2

// Yaw
#define YAWANTIAIM_NONE						0
#define YAWANTIAIM_BACKWARDS					1
#define YAWANTIAIM_STATICJITTERBACKWARDS		2
#define YAWANTIAIM_REALLEFTFAKERIGHT			3

class CAntiAim : public IFeature
{
public:
	CAntiAim();
	~CAntiAim();

	void SetPitchSetting(int iPitchSetting) { m_iPitchSetting = iPitchSetting; }
	int GetPitchSetting() { return m_iPitchSetting; }

	void SetYawSetting(int iYawSetting) { m_iYawSetting = iYawSetting; }
	int GetYawSetting() { return m_iYawSetting; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	int m_iPitchSetting;
	int m_iYawSetting;
};

#endif // __ANTIAIM_H__