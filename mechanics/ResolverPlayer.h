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

	void SetLastLby(float fLastLby) { m_fLastLby = fLastLby; }
	float GetLastLby() { return m_fLastLby; }

	void SetLbyUpdateTime(float fLbyUpdateTime) { m_fLbyUpdateTime = fLbyUpdateTime; }
	float GetLbyUpdateTime() { return m_fLbyUpdateTime; }

	void SetRealLbyUpdateTime(float fRealLbyUpdateTime) { m_fRealLbyUpdateTime = fRealLbyUpdateTime; }
	float GetRealLbyUpdateTime() { return m_fRealLbyUpdateTime; }

	void SetPredictedLbyUpdateTime(float fPredictedLbyUpdateTime) { m_fPredictedLbyUpdateTime = fPredictedLbyUpdateTime; }
	float GetPredictedLbyUpdateTime() { return m_fPredictedLbyUpdateTime; }

	void SetAngles(QAngle qAngles) { m_qAngles = qAngles; }
	QAngle GetAngles() { return m_qAngles; }

	bool m_bDidSet = false;
private:
	int m_iShotsFired;
	int m_iShotsBruteforced;

	float m_fLastLby;
	float m_fLbyUpdateTime;
	float m_fRealLbyUpdateTime;
	float m_fPredictedLbyUpdateTime;

	QAngle m_qAngles;
};

#endif // __RESOLVERPLAYER_H__