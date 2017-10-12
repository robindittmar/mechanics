#ifndef __LAGCOMPENSATION_H__
#define __LAGCOMPENSATION_H__

#include "IFeature.h"
#include "ClientEntity.h"
#include "XorString.h"

#include "LagCompensationPlayerEntry.h"
#include "LagCompensationPlayerList.h"

class CApplication;

#define MAX_PLAYERS			64

#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / CApplication::Instance()->GlobalVars()->interval_per_tick))

#define LC_DRAWSTYLE_NONE		0
#define LC_DRAWSTYLE_CROSS		1
#define LC_DRAWSTYLE_BONES		2

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

	void SetRageLagCompensationEnabled(bool bRageLagCompensationEnabled) { m_bRageLagCompensationEnabled = bRageLagCompensationEnabled; }
	bool GetRageLagCompensationEnabled() { return m_bRageLagCompensationEnabled; }

	void SetLegitLagCompensationEnabled(bool bLegitLagCompensationEnabled) { m_bLegitLagCompensationEnabled = bLegitLagCompensationEnabled; }
	bool GetLegitLagCompensationEnabled() { return m_bLegitLagCompensationEnabled; }

	void SetLegitLagCompensationDuration(int bLegitLagCompensationDuration) { m_bLegitLagCompensationDuration = bLegitLagCompensationDuration; }
	int GetLegitLagCompensationDuration() { return m_bLegitLagCompensationDuration; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	int RestorePlayerClosestToCrosshair();

	void DrawLagCompensationEntries();
	void DrawLagCompensationIndicator();
	CLagCompensationPlayerList* GetLCList(int index);
private:
	int m_iDrawStyle;
	int m_iDrawFrequency;

	bool m_bDrawOnlyVisible;

	bool m_bRageLagCompensationEnabled;

	bool m_bLegitLagCompensationEnabled;
	int m_bLegitLagCompensationDuration;

	CLagCompensationPlayerList m_pPlayerList[MAX_PLAYERS];
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