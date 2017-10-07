#ifndef __LAGCOMPENSATION_H__
#define __LAGCOMPENSATION_H__

#include "IFeature.h"
#include "ClientEntity.h"
#include "XorString.h"

class CApplication;

#define MAX_PLAYERS 64
#define LC_MAXSAVEDTICKS 26
#define MAXSTUDIOPOSEPARAM 24

#define TIME_TO_TICKS(dt) ((int)( 0.5f + (float)(dt) / CApplication::Instance()->GlobalVars()->interval_per_tick))

class CLagCompensationPlayerEntry
{
public:
	CLagCompensationPlayerEntry();

	int m_iTickCount;

	Vector m_vOrigin;
	Vector m_vHeadPos;
	Vector m_vVelocity;
	QAngle m_angEyeAngles;
	float m_fSimulationTime;
	int m_iSequenceNumber;
	DWORD m_fFlags;
	float m_fCycle;
	float m_fLowerBodyYaw;
	float m_fPoseParameters[MAXSTUDIOPOSEPARAM];

	ULONGLONG m_llAddTime;
	bool m_bIsEndOfList;

	//int FixedTickcount(IClientEntity* pCur);
private:
	//float LerpTime();
};

class LagCompensationList
{
public:
	int m_iEntryCount = 0;
	CLagCompensationPlayerEntry m_pPlayerEntries[LC_MAXSAVEDTICKS];

	void RemoveInvalidPlayerEntries();
	void AddPlayerEntry(IClientEntity* pCurEnt, int tickcount);
	void RestorePlayerEntry(IClientEntity* pCurEnt, int iEntryIndex);
};

class CLagCompensation : public IFeature
{
public:
	CLagCompensation();
	~CLagCompensation();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void DrawLagCompensationEntries();
	LagCompensationList* GetLCList(int index);
private:
	LagCompensationList m_pPlayerList[MAX_PLAYERS];
};

template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

#endif // __LAGCOMPENSATION_H__