#ifndef __TRIGGERBOT_H__
#define __TRIGGERBOT_H__

// Std lib
#include <assert.h>

// Source SDK
#include "UserCmd.h"
#include "ClientEntityList.h"
#include "IVEngineClient.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"
#include "ray.h"
#include "CWeapon.h"

// Custom
#include "IFeature.h"

class CTriggerbot : public IFeature
{
public:
	CTriggerbot();
	virtual ~CTriggerbot();

	void TriggerKeyDown();
	void TriggerKeyUp();

	void SetTriggerKey(int iKey) { m_iKey = iKey;}
	int GetTriggerKey() { return m_iKey; }

	void SetTriggerBurst(bool bTriggerBurst) { m_bTriggerBurst = bTriggerBurst; }
	bool GetTriggerBurst() { return m_bTriggerBurst; }

	void SetMinShots(int iMinShots) { m_iMinShots = iMinShots; }
	int GetMinShots() { return m_iMinShots; }

	void SetMaxShots(int iMaxShots) { m_iMaxShots = iMaxShots; }
	int GetMaxShots() { return m_iMaxShots; }

	// 0 - 100%
	void SetMaxFlashPercentage(float fMaxFlashPercentage) { m_fMaxFlashPercentage = fMaxFlashPercentage / 100.0f; }
	float GetMaxFlashPercentage() { return m_fMaxFlashPercentage * 100.0f; }

	// Unit: milliseconds
	void SetShootDelay(int iShootDelay) { m_fShootDelay = iShootDelay / 1000.0f; }
	int GetShootDelay() { return (int)(m_fShootDelay * 1000.0f); }

	// Unit: milliseconds
	void SetShootDelayJitter(int iShootDelayJitter) { m_fShootDelayJitter = iShootDelayJitter / 1000.0f; }
	int GetShootDelayJitter() { return (int)(m_fShootDelayJitter * 1000.0f); }

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;
private:
	bool m_bKeyDown;
	int m_iKey;

	float m_fCurtimeTargetAquired;
	bool m_bAlreadyWaitedOnTarget;
	IClientEntity* m_pCurTarget;

	int m_iShotsForThisTarget;
	int m_iShotsOnTarget;

	bool m_bTriggerBurst;
	int m_iMinShots;
	int m_iMaxShots;

	float m_fMaxFlashPercentage;

	float m_fShootDelay;
	float m_fShootDelayJitter;

	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
	IEngineTrace* m_pEngineTrace;
};

#endif // __TRIGGERBOT_H__