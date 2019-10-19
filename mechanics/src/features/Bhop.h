#ifndef __BHOP_H__
#define __BHOP_H__

// Std Lib

// Source SDK
#include "../source_sdk/UserCmd.h"
#include "../source_sdk/ClientEntity.h"

// Custom
#include "IFeature.h"

#define AUTOSTRAFEMODE_NONE				0
#define AUTOSTRAFEMODE_LEGIT			1
#define AUTOSTRAFEMODE_RAGE				2

#define CIRCLESTRAFEDIRECTION_RIGHT		1
#define CIRCLESTRAFEDIRECTION_LEFT		-1

class CBhop : public IFeature
{
public:
	CBhop();
	virtual ~CBhop();

	void SetAutoStrafeMode(int iAutoStrafeMode) { m_iAutoStrafeMode = iAutoStrafeMode; }
	int GetAutoStrafeMode() { return m_iAutoStrafeMode; }

	void SetCircleStrafe(bool bCircleStrafe) { m_bCircleStrafe = bCircleStrafe; }
	bool GetCircleStrafe() { return m_bCircleStrafe; }

	void SetCircleStrafeStartDirection(int iCircleStrafeStartDirection) { m_iCircleStrafeStartDirection = iCircleStrafeStartDirection; }
	int GetCircleStrafeStartDirection() { return m_iCircleStrafeStartDirection; }

	virtual void Think(void* pParameters = nullptr) override;
private:
	void Bhop(IClientEntity* pLocalEntity, CUserCmd* pUserCmd);
	void AutoStrafe(IClientEntity* pLocalEntity, CUserCmd* pUserCmd);
	void CircleStrafe(IClientEntity* pLocalEntity, CUserCmd* pUserCmd);

	int m_iAutoStrafeMode;

	bool m_bCircleStrafe;
	int m_iCircleStrafeStartDirection;

};

#endif // __BHOP_H__