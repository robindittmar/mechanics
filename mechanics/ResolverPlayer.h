#ifndef __RESOLVERPLAYER_H__
#define __RESOLVERPLAYER_H__

#include "Vector.h"

class CResolverPlayer
{
public:
	CResolverPlayer();
	~CResolverPlayer();

	void SetShotsFired(int iShotsFired) { m_iShotsFired = iShotsFired; }
	int GetShotsFired() { return m_iShotsFired; }

	void SetShotHit(int iShotHit) { m_iShotHit = iShotHit; }
	int GetShotHit() { return m_iShotHit; }

	void SetOriginalYaw(float fOriginalYaw) { m_fOriginalYaw = fOriginalYaw; }
	float GetOriginalYaw() { return m_fOriginalYaw; }

	void SetLastMovingTime(float fLastMovingTime) { m_fLastMovingTime = fLastMovingTime; }
	float GetLastMovingTime() { return m_fLastMovingTime; }

	void SetLbyProxyLastValue(float fLastLby) { m_fLbyProxyLastValue = fLastLby; }
	float GetLbyProxyLastValue() { return m_fLbyProxyLastValue; }

	void SetLbyUpdateTime(float fLbyUpdateTime) { m_fLbyUpdateTime = fLbyUpdateTime; }
	float GetLbyUpdateTime() { return m_fLbyUpdateTime; }

	void SetLbyProxyUpdatedTime(float fRealLbyUpdateTime) { m_fLbyProxyUpdatedTime = fRealLbyUpdateTime; }
	float GetLbyProxyUpdatedTime() { return m_fLbyProxyUpdatedTime; }

	void SetPredLbyUpdateTime(float fPredictedLbyUpdateTime) { m_fPredLbyUpdateTime = fPredictedLbyUpdateTime; }
	float GetPredLbyUpdateTime() { return m_fPredLbyUpdateTime; }

	void SetPitchAngle(float fPitchAngle) { m_qAngles.x = fPitchAngle; m_qAngles.NormalizeAngles(); }
	void SetYawAngle(float fYawAngle) { m_qAngles.y = fYawAngle; m_qAngles.NormalizeAngles(); }
	QAngle GetAngles() { return m_qAngles; }

	bool m_bDidSet = false;
	float m_fFirstLbySinceStanding;

	float m_fPossibleLbyBreakerReal;
	float m_fPossibleLbyBreakerFake;
	float m_fPossibleLbyBreakerStart;


	float m_fBalanceAdjustStart;

	int m_iFakeCount;
	bool m_bAdjustedWithFake;
	bool m_bWasInAir;
	bool m_bHadFakeActive;
	bool m_bFakeActive;
	bool m_bBreakingLby;
	bool m_bStartPredLbyBreaks;
private:
	int m_iShotsFired;
	int m_iShotHit;
	float m_fOriginalYaw;

	float m_fLastMovingTime;
	float m_fLbyAfterPossibleBreaker;

	float m_fLbyUpdateTime;

	float m_fLbyProxyLastValue;
	float m_fLbyProxyUpdatedTime;
	float m_fPredLbyUpdateTime;


	QAngle m_qAngles;
};

#endif // __RESOLVERPLAYER_H__