#ifndef __LAGCOMPENSATION_H__
#define __LAGCOMPENSATION_H__

#include "IFeature.h"
#include "ClientEntity.h"
#include "XorString.h"

class CApplication;

#define MAX_PLAYERS			64
#define LC_MAXSAVEDTICKS	26
#define MAXSTUDIOPOSEPARAM	24
#define MAXSTUDIOBONES		128

#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / CApplication::Instance()->GlobalVars()->interval_per_tick))

#define LC_DRAWSTYLE_NONE		0
#define LC_DRAWSTYLE_CROSS		1
#define LC_DRAWSTYLE_BONES		2

class CLagCompensationPlayerEntry
{
public:
	CLagCompensationPlayerEntry();

	int m_iTickCount;

	matrix3x4_t m_pBoneMatrix[MAXSTUDIOBONES];

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

	void SetDrawStyle(int iDrawStyle) { m_iDrawStyle = iDrawStyle; }
	int GetDrawStyle() { return m_iDrawStyle; }

	void SetDrawFrequency(int iDrawFrequency) { m_iDrawFrequency = iDrawFrequency; }
	int GetDrawFrequency() { return m_iDrawFrequency; }

	void SetDrawOnlyVisible(bool bDrawOnlyVisible) { m_bDrawOnlyVisible = bDrawOnlyVisible; }
	bool GetDrawOnlyVisible() { return m_bDrawOnlyVisible; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	int RestorePlayerClosestToCrosshair();

	void DrawLagCompensationEntries();
	LagCompensationList* GetLCList(int index);
private:
	int m_iDrawStyle;
	int m_iDrawFrequency;

	bool m_bDrawOnlyVisible;

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