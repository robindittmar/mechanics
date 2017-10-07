#include "Triggerbot.h"
#include "Application.h"

CTriggerbot::CTriggerbot()
{
}

CTriggerbot::~CTriggerbot()
{
}

void CTriggerbot::Setup()
{
	m_pApp = CApplication::Instance();

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
	m_pEngineTrace = m_pApp->EngineTrace();
}

void CTriggerbot::Update(void* pParameters)
{
	assert(pParameters != NULL);

	if (!m_bIsEnabled)
		return;

	if (!(GetAsyncKeyState(VK_XBUTTON1) & 0x8000))
		return;

	// Grab params
	CreateMoveParam* pCreateMoveParam = (CreateMoveParam*)pParameters;
	if (!pCreateMoveParam) // Shouldn't happen, but just to be safe
		return;

	float fInputSampleTime = pCreateMoveParam->fInputSampleTime;
	CUserCmd* pUserCmd = pCreateMoveParam->pUserCmd;

	// Get Local player and check if we are good to check for trigger target
	IClientEntity* pLocalEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsAlive())
		return;

	int iLocalTeamNum = pLocalEntity->GetTeamNum();
	if (iLocalTeamNum != TEAMNUM_T && iLocalTeamNum != TEAMNUM_CT)
		return;

	// TODO: Check is flashed?

	// Get weapon and check if valid weapon
	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4())
		return;

	// Get weapon info
	CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
	if (!pWeaponInfo)
		return;

	// Get local angles and forward vector
	QAngle qLocalAngles = m_pApp->GetClientViewAngles();

	QAngle aimPunchAngle = *pLocalEntity->GetAimPunchAngle();
	qLocalAngles.x -= aimPunchAngle.x * m_pApp->GetRecoilCompensation();
	qLocalAngles.y -= aimPunchAngle.y * m_pApp->GetRecoilCompensation();

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
	{
		m_pCurTarget = NULL;
		return;
	}

	if (pHitEntity->IsDormant())
	{
		m_pCurTarget = NULL;
		return;
	}

	if (pHitEntity->GetClientClass()->m_ClassID != CCSPlayer)
	{
		m_pCurTarget = NULL;
		return;
	}

	if (!pHitEntity->IsAlive())
	{
		m_pCurTarget = NULL;
		return;
	}

	int iHitEntityTeamNum = pHitEntity->GetTeamNum();
	if (iHitEntityTeamNum != TEAMNUM_T &&
		iHitEntityTeamNum != TEAMNUM_CT ||
		iHitEntityTeamNum == iLocalTeamNum)
	{
		m_pCurTarget = NULL;
		return;
	}

	ULONGLONG llTimestamp = GetTickCount64();
	if (m_pCurTarget == NULL)
	{
		m_llTimestampTargetAquired = llTimestamp;
		m_pCurTarget = pHitEntity;
	}
	
	int iShootDelay = m_iShootDelay + (RandomIntDef(0, m_iShootDelayJitter) - (m_iShootDelayJitter / 2));
	if ((llTimestamp - m_llTimestampTargetAquired) < iShootDelay)
		return;

	// TODO: Check hitbox (intersectswithoobb?)

	float fNextPrimaryAttack = pActiveWeapon->GetNextPrimaryAttack();
	float fServerTime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;
	if (fNextPrimaryAttack <= fServerTime)
	{
		// Shoot
		pUserCmd->buttons |= IN_ATTACK;
	}
}