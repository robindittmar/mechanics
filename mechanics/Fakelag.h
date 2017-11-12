#ifndef __FAKELAG_H__
#define __FAKELAG_H__

// Std Lib
#include <assert.h>

// Source SDK
#include "UserCmd.h"
#include "ICVar.h"

// Custom
#include "IFeature.h"

#define MAX_CHOKE_PACKETS					15
#define LAGCOMPENSATION_TELEPORT_DIST		4096.0f

#define FAKELAG_TYPE_FACTOR					0
#define FAKELAG_TYPE_ADAPTIVE				1
//#define FAKELAG_TYPE_

class CFakelag : public IFeature
{
public:
	CFakelag();
	~CFakelag();

	bool IsChoking() { return m_bIsChoking; }
	int AmountPacketsChoked() { return m_iPacketsChoked; }

	void SetChokeAmount(int iChokeAmount) { m_iChokeAmount = iChokeAmount; }
	int GetChokeAmountMenu() { return m_iChokeAmount; }
	int GetChokeAmount();

	void SetOnlyInAir(bool bOnlyInAir) { m_bOnlyInAir = bOnlyInAir; }
	bool GetOnlyInAir() { return m_bOnlyInAir; }

	void SetLagType(int iLagType) { m_iLagType = iLagType; }
	int GetLagType() { return m_iLagType; }

	void CalcAdaptiveChokeAmount();

	virtual void Update(void* pParameters = 0) override;
private:
	bool m_bIsChoking;

	int m_iPacketsChoked;
	int m_iChokeAmount;
	int m_iAdaptiveChokeAmount;

	bool m_bOnlyInAir;
	int m_iLagType;
};

#endif // __FAKELAG_H__