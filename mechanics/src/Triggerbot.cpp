#include "Triggerbot.h"
#include "Application.h"

CTriggerbot::CTriggerbot()
	: m_bKeyDown(false), m_fCurtimeTargetAquired(0.0f), m_bAlreadyWaitedOnTarget(false),
	m_pCurTarget(nullptr), m_iShotsOnTarget(0), m_iShotsForThisTarget(0),
	m_bTriggerBurst(false), m_iMinShots(0), m_iMaxShots(0),
	m_fMaxFlashPercentage(0.0f), m_fShootDelay(0.0f), m_fShootDelayJitter(0.0f)
{
}

CTriggerbot::~CTriggerbot()
{
}

void CTriggerbot::TriggerKeyDown()
{
	m_bKeyDown = true;
}

void CTriggerbot::TriggerKeyUp()
{
	m_bKeyDown = false;

	m_pCurTarget = nullptr;
}

void CTriggerbot::Setup()
{
	IFeature::Setup();

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
	m_pEngineTrace = m_pApp->EngineTrace();
}

void CTriggerbot::Update(void* pParameters)
{
	assert(pParameters != nullptr);

	if (!m_bIsEnabled)
		return;

	if (!m_bKeyDown)
		return;

	// Grab params
	CreateMoveParam* pCreateMoveParam = (CreateMoveParam*)pParameters;

	float fInputSampleTime = pCreateMoveParam->fInputSampleTime;
	CUserCmd* pUserCmd = pCreateMoveParam->pUserCmd;

	// Get curtime
	float fCurtime = m_pApp->GlobalVars()->curtime;

	// Get Local player and check if we are good to check for trigger target
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsAlive())
		return;

	int iLocalTeamNum = pLocalEntity->GetTeamNum();
	if (iLocalTeamNum != TEAMNUM_T && iLocalTeamNum != TEAMNUM_CT)
		return;

	// TODO: Check is flashed?
	// m_fMaxFlashPercentage

	// Get weapon and check if valid weapon
	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4())
		return;

	if (!m_pCurTarget)
	{
		// Get weapon info
		CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
		if (!pWeaponInfo)
			return;

		// Get local angles and forward vector
		QAngle qLocalAngles = m_pApp->GetClientViewAngles() + (*pLocalEntity->GetAimPunchAngle() * m_pApp->GetRecoilCompensation());

		Vector vForward;
		AngleVectors(qLocalAngles, &vForward);

		// Get trace positions
		Vector vStartPos = (*pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset()) + (*pLocalEntity->GetVelocity() * fInputSampleTime);
		Vector vEndPos = vStartPos + (vForward * pWeaponInfo->flRange);

		// Tracing
		Ray_t ray;
		trace_t trace;
		CTraceFilterSkipEntity traceFilter(pLocalEntity);

		ray.Init(vStartPos, vEndPos);
		m_pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &trace);
		/*if (!trace.IsVisible())
			return;*/

		// Check if we hit a target we want to shoot
		IClientEntity* pHitEntity = trace.hit_entity;
		if (!pHitEntity)
			return;

		if (pHitEntity->IsDormant())
			return;

		if (pHitEntity->GetClientClass()->m_ClassID != CCSPlayer)
			return;

		if (!pHitEntity->IsAlive())
			return;

		int iHitEntityTeamNum = pHitEntity->GetTeamNum();
		if (iHitEntityTeamNum != TEAMNUM_T &&
			iHitEntityTeamNum != TEAMNUM_CT ||
			iHitEntityTeamNum == iLocalTeamNum)
			return;
		
		m_fCurtimeTargetAquired = fCurtime;
		m_pCurTarget = pHitEntity;

		m_bAlreadyWaitedOnTarget = false;
		m_iShotsOnTarget = 0;
		m_iShotsForThisTarget = m_bTriggerBurst ? RandomIntDef(m_iMinShots, m_iMaxShots) : 1;
	}

	if (!m_bAlreadyWaitedOnTarget)
	{
		// Should be we shooting? (timewise)
		float fShootDelay = m_fShootDelay + (m_pApp->RandomFloat()(0.0f, m_fShootDelayJitter) - (m_fShootDelayJitter / 2));
		if ((fCurtime - m_fCurtimeTargetAquired) < fShootDelay)
			return;

		m_bAlreadyWaitedOnTarget = true;
	}

	// TODO: Check hitbox (intersectswithoobb?)

	if (m_iShotsOnTarget < m_iShotsForThisTarget)
	{
		float fNextPrimaryAttack = pActiveWeapon->GetNextPrimaryAttack();
		float fServerTime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;
		if (fNextPrimaryAttack <= fServerTime)
		{
			// Shoot
			pUserCmd->buttons |= IN_ATTACK;
			m_iShotsOnTarget++;
		}
	}
	else
	{
		m_pCurTarget = nullptr;
	}
}