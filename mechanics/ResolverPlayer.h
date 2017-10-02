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

	void SetShotsBruteforced(int iShotsBruteforced) { m_iShotsBruteforced = iShotsBruteforced; }
	int GetShotsBruteforced() { return m_iShotsBruteforced; }

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

	void SetAngles(QAngle qAngles) { m_qAngles = qAngles; }
	QAngle GetAngles() { return m_qAngles; }

	bool m_bDidSet = false;
	float m_fFirstLbySinceStanding;

	float m_fPossibleLbyBreakerReal;
	float m_fPossibleLbyBreakerFake;
	float m_fPossibleLbyBreakerStart;

	bool m_bHasFakeActive;
	bool m_bStartPredLbyBreaks;
private:
	int m_iShotsFired;
	int m_iShotsBruteforced;

	float m_fLastMovingTime;
	float m_fLbyAfterPossibleBreaker;

	float m_fLbyUpdateTime;

	float m_fLbyProxyLastValue;
	float m_fLbyProxyUpdatedTime;
	float m_fPredLbyUpdateTime;

	QAngle m_qAngles;
};

#endif // __RESOLVERPLAYER_H__