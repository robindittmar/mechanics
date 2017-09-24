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

typedef unsigned __int64 ULONGLONG;

class CTriggerbot : public IFeature
{
public:
	CTriggerbot();
	~CTriggerbot();

	// Unit: milliseconds
	void SetShootDelay(int iShootDelay) { m_iShootDelay = iShootDelay; }
	int GetShootDelay() { return m_iShootDelay; }

	// Unit: milliseconds
	void SetShootDelayJitter(int iShootDelayJitter) { m_iShootDelayJitter = iShootDelayJitter; }
	int GetShootDelayJitter() { return m_iShootDelayJitter; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	ULONGLONG m_llTimestampTargetAquired;
	IClientEntity* m_pCurTarget;

	int m_iShootDelay;
	int m_iShootDelayJitter;

	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
	IEngineTrace* m_pEngineTrace;
};

#endif // __TRIGGERBOT_H__