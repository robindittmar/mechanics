#ifndef __RESOLVERPLAYER_H__
#define __RESOLVERPLAYER_H__

#include "../source_sdk/Vector.h"

class CResolverPlayer
{
public:
	CResolverPlayer();
	~CResolverPlayer();

	void SetShotsFired(int iShotsFired) { m_iShotsFired = iShotsFired; }
	int GetShotsFired() { return m_iShotsFired; }

	void SetShotHit(int iShotHit) { m_iShotHit = iShotHit; }
	int GetShotHit() { return m_iShotHit; }


	void SetBreakingShotsFired(int iBreakingShotsFired) { m_iBreakingShotsFired = iBreakingShotsFired; }
	int GetBreakingShotsFired() { return m_iBreakingShotsFired; }

	void SetBreakingShotHit(int iBreakingShotHit) { m_iBreakingShotHit = iBreakingShotHit; }
	int GetBreakingShotHit() { return m_iBreakingShotHit; }

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

	float m_fBalanceAdjustStart;

	bool m_bWasInAir;

	bool m_bHadFakeActive;
	bool m_bFakeActive;

	bool m_bWasBreakingLby;
	bool m_iBreakingLbyInteruptCount;
	bool m_bBreakingLby;

	bool m_bFakewalking;
	bool m_bMoving;
	bool m_bOnGround;

	bool m_bLbyPredict;

	int m_iLbyUpdateTickCount;
private:
	int m_iShotsFired;
	int m_iShotHit;
	int m_iBreakingShotsFired;
	int m_iBreakingShotHit;

	float m_fLbyUpdateTime;

	float m_fLbyProxyLastValue;
	float m_fLbyProxyUpdatedTime;
	float m_fPredLbyUpdateTime;


	QAngle m_qAngles;
};

#endif // __RESOLVERPLAYER_H__