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
	~CTriggerbot();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
	IEngineTrace* m_pEngineTrace;
};

#endif // __TRIGGERBOT_H__