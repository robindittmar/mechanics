#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "IFeature.h"
#include "Vector.h"

class CApplication;

// Pitch
#define PITCHANTIAIM_NONE						0
#define PITCHANTIAIM_UP							1
#define PITCHANTIAIM_DOWN						2

// Yaw
#define YAWANTIAIM_NONE							0
#define YAWANTIAIM_STATIC						1
#define YAWANTIAIM_BACKWARDS					2
#define YAWANTIAIM_STATICJITTERBACKWARDS		3

// Yaw Fake
#define FAKEYAWANTIAIM_NONE						0
#define FAKEYAWANTIAIM_STATIC					1

class CAntiAim : public IFeature
{
public:
	CAntiAim();
	~CAntiAim();

	void SetPitchSetting(int iPitchSetting) { m_iPitchSetting = iPitchSetting; }
	int GetPitchSetting() { return m_iPitchSetting; }

	void SetYawSetting(int iYawSetting) { m_iYawSetting = iYawSetting; }
	int GetYawSetting() { return m_iYawSetting; }

	void SetYawFakeSetting(int iYawFakeSetting) { m_iYawFakeSetting = iYawFakeSetting; }
	int GetYawFakeSetting() { return m_iYawFakeSetting; }


	void SetPitchOffset(float fPitchOffset) { m_fPitchOffset = fPitchOffset; }
	float GetPitchOffset() { return m_fPitchOffset; }

	void SetYawOffset(float fYawOffset) { m_fYawOffset = fYawOffset; }
	float GetYawOffset() { return m_fYawOffset; }

	void SetYawFakeOffset(float fYawFakeOffset) { m_fYawFakeOffset = fYawFakeOffset; }
	float GetYawFakeOffset() { return m_fYawFakeOffset; }

	void SetDrawLbyIndicator(bool bDrawLbyIndicator) { m_bDrawLbyIndicator = bDrawLbyIndicator; }
	bool GetDrawLbyIndicator() { return m_bDrawLbyIndicator; }

	void SetLbyBreaker(bool bLbyBreaker) { m_bLbyBreaker = bLbyBreaker; }
	bool GetLbyBreaker() { return m_bLbyBreaker; }

	bool IsFakeYaw() { return m_bIsFakeYaw; }

	void DrawLBYIndicator();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	int m_iPitchSetting;
	int m_iYawSetting;
	int m_iYawFakeSetting;

	float m_fPitchOffset;
	float m_fYawOffset;
	float m_fYawFakeOffset;

	bool m_bIsFakeYaw;

	bool m_bDrawLbyIndicator;
	bool m_bLbyBreaker;

	void ApplyPitchAntiAim(QAngle* angles);
	void ApplyYawAntiAim(QAngle* angles);
	void ApplyYawFakeAntiAim(QAngle* angles, float fRealAngle);
};

#endif // __ANTIAIM_H__