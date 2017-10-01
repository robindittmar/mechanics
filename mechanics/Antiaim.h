#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "IFeature.h"
#include "Vector.h"
#include "ResolverPlayer.h"

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

	// Standing
	void SetPitchSettingStanding(int iPitchSetting) { m_iPitchSettingStanding = iPitchSetting; }
	int GetPitchSettingStanding() { return m_iPitchSettingStanding; }

	void SetYawSettingStanding(int iYawSetting) { m_iYawSettingStanding = iYawSetting; }
	int GetYawSettingStanding() { return m_iYawSettingStanding; }

	void SetYawFakeSettingStanding(int iYawFakeSetting) { m_iYawFakeSettingStanding = iYawFakeSetting; }
	int GetYawFakeSettingStanding() { return m_iYawFakeSettingStanding; }

	void SetPitchOffsetStanding(float fPitchOffset) { m_fPitchOffsetStanding = fPitchOffset; }
	float GetPitchOffsetStanding() { return m_fPitchOffsetStanding; }

	void SetYawOffsetStanding(float fYawOffset) { m_fYawOffsetStanding = fYawOffset; }
	float GetYawOffsetStanding() { return m_fYawOffsetStanding; }

	void SetYawFakeOffsetStanding(float fYawFakeOffset) { m_fYawFakeOffsetStanding = fYawFakeOffset; }
	float GetYawFakeOffsetStanding() { return m_fYawFakeOffsetStanding; }

	// Moving
	void SetPitchSettingMoving(int iPitchSetting) { m_iPitchSettingMoving = iPitchSetting; }
	int GetPitchSettingMoving() { return m_iPitchSettingMoving; }

	void SetYawSettingMoving(int iYawSetting) { m_iYawSettingMoving = iYawSetting; }
	int GetYawSettingMoving() { return m_iYawSettingMoving; }

	void SetYawFakeSettingMoving(int iYawFakeSetting) { m_iYawFakeSettingMoving = iYawFakeSetting; }
	int GetYawFakeSettingMoving() { return m_iYawFakeSettingMoving; }

	void SetPitchOffsetMoving(float fPitchOffset) { m_fPitchOffsetMoving = fPitchOffset; }
	float GetPitchOffsetMoving() { return m_fPitchOffsetMoving; }

	void SetYawOffsetMoving(float fYawOffset) { m_fYawOffsetMoving = fYawOffset; }
	float GetYawOffsetMoving() { return m_fYawOffsetMoving; }

	void SetYawFakeOffsetMoving(float fYawFakeOffset) { m_fYawFakeOffsetMoving = fYawFakeOffset; }
	float GetYawFakeOffsetMoving() { return m_fYawFakeOffsetMoving; }

	void SetDrawLbyIndicator(bool bDrawLbyIndicator) { m_bDrawLbyIndicator = bDrawLbyIndicator; }
	bool GetDrawLbyIndicator() { return m_bDrawLbyIndicator; }

	void SetLbyBreaker(bool bLbyBreaker) { m_bLbyBreaker = bLbyBreaker; }
	bool GetLbyBreaker() { return m_bLbyBreaker; }

	bool IsFakeYaw();
	bool NextLBYUpdate(CResolverPlayer* pResolverPlayer, bool bIsLocalPlayer = false);

	void DrawLBYIndicator();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	// Standing
	int m_iPitchSettingStanding;
	int m_iYawSettingStanding;
	int m_iYawFakeSettingStanding;

	float m_fPitchOffsetStanding;
	float m_fYawOffsetStanding;
	float m_fYawFakeOffsetStanding;

	// Moving
	int m_iPitchSettingMoving;
	int m_iYawSettingMoving;
	int m_iYawFakeSettingMoving;

	float m_fPitchOffsetMoving;
	float m_fYawOffsetMoving;
	float m_fYawFakeOffsetMoving;

	bool m_bDrawLbyIndicator;
	bool m_bLbyBreaker;
	bool m_bNextLbyUpdate;

	bool m_bIsMoving;
	bool m_bWasMoving;

	void ApplyPitchAntiAim(QAngle* angles);
	void ApplyYawAntiAim(QAngle* angles);
	void ApplyYawFakeAntiAim(QAngle* angles, float fRealAngle);
};

#endif // __ANTIAIM_H__