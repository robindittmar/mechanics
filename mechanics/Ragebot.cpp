#include "Ragebot.h"
#include "Application.h"

CRagebot::CRagebot()
{
	m_bSilentAim = false;
	m_bAutoshoot = true;
	m_bAutoscope = false;
	m_bDoNoRecoil = true;
	m_bDoNoSpread = false;
	m_fHitchance = 0.0f;
	m_bAutoReload = true;

	m_iTargetCriteria = TARGETCRITERIA_UNSPECIFIED;
}

CRagebot::~CRagebot()
{
}

void CRagebot::Setup()
{
	m_pApp = CApplication::Instance();

	m_pTargetSelector = m_pApp->TargetSelector();
	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

/*int GetBoneByName(CApplication* pApp, IClientEntity* player, const char* bone)
{
	studiohdr_t pStudioModel = pApp->ModelInfo()->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return -1;

	return 0;
}*/

void CRagebot::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	// Grab IsShooting before reseting for new tick
	bool m_bDidShootLastTick = m_bIsShooting;

	// Reset
	this->ResetTickVariables();

	// Local vars
	CreateMoveParam* pParam;
	CUserCmd* pUserCmd;
	float fInputSampleTime;

	IClientEntity* pLocalEntity;
	CWeapon* pMyActiveWeapon;

	// Initialize local variables
	pParam = (CreateMoveParam*)pParameters;
	pUserCmd = pParam->pUserCmd;
	fInputSampleTime = pParam->fInputSampleTime;

	pLocalEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	pMyActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pMyActiveWeapon)
		return;

	// Aimbot invalid weapons
	if (pMyActiveWeapon->IsKnife() ||
		pMyActiveWeapon->IsNade() ||
		pMyActiveWeapon->IsC4())
		return;

	// Clip empty?
	if (pMyActiveWeapon->GetClip1() == 0)
	{
		if (m_bAutoReload)
			pUserCmd->buttons |= IN_RELOAD;

		return;
	}

	// TODO: Ghettofix for sniper not shooting after 1st shot
	if (pMyActiveWeapon->IsSniper() && m_bDidShootLastTick)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
		return;
	}

	// Choose target
	if (!m_pTargetSelector->GetHasTargets())
		m_pTargetSelector->SelectTargets(fInputSampleTime);

	m_pTarget = m_pTargetSelector->GetTarget(m_iTargetCriteria);
	if (!m_pTarget) // This should never happen
		return;

	if (!m_pTarget->GetValid())
		return;

	// Copy target angles
	m_qAimAngles = *m_pTarget->GetAimAngles();

	// Hitchance
	float fHitchance = this->CalculateHitchance(pLocalEntity, pMyActiveWeapon, m_pTarget->GetEntity());
	if (fHitchance < m_fHitchance && m_pTarget->GetIsBacktracked() == -1)
		return;

	// Checks if weapon could hit
	bool bAbleToHit = !(pMyActiveWeapon->IsTaser() && m_bAutoZeus);
	if (!bAbleToHit)
	{
		for (int i = 0; i < 2; i++)
		{
			CTarget* pTarget = (i == 0 ? m_pTarget : m_pTargetSelector->GetTarget(TARGETCRITERIA_ORIGIN));

			float fOriginDist = pTarget->GetOriginDist();
			int iTargetHealth = pTarget->GetEntity()->GetHealth();

			// Check if enough damage is made to kill
			if (fOriginDist <= 185.0f)
				bAbleToHit = true;
			else if (fOriginDist < 225.0f && iTargetHealth < 72)
				bAbleToHit = true;
			else if (fOriginDist < 230.0f && iTargetHealth < 60)
				bAbleToHit = true;

			m_qAimAngles = *pTarget->GetAimAngles();
			if (bAbleToHit || m_iTargetCriteria == TARGETCRITERIA_ORIGIN)
				break;
		}
	}

	// Set ClientViewAngles if we don't have silentaim activated
	// (we do so before applying NoRecoil :D)
	if (!this->m_bSilentAim)
	{
		m_pApp->SetClientViewAngles(m_qAimAngles);
	}

	// Do NoRecoil
	this->ApplyNoRecoil(pLocalEntity);
	// Do Nospread
	this->ApplyNoSpread(pLocalEntity, pMyActiveWeapon, pUserCmd->random_seed);
	// Apply viewangles & shoot if necessary
	this->ApplyViewanglesAndShoot(pUserCmd, pLocalEntity, bAbleToHit);
}

void inline CRagebot::ResetTickVariables()
{
	m_bIsShooting = false;
	m_bDidNoRecoil = false;
	m_bDidNoSpread = false;

	m_fDamage = 0.0f;
}

void CRagebot::ApplyNoRecoil(IClientEntity* pLocalEntity)
{
	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if (m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *pLocalEntity->GetAimPunchAngle();
		m_qAimAngles.x -= aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_qAimAngles.y -= aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_bDidNoRecoil = true;
	}
}

void CRagebot::ApplyNoSpread(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, int iSeed)
{
	if (!m_bDoNoSpread)
		return;

	if (IsNoSpread())
		return;

	if (!IsAbleToApplyNoSpread())
		return;

	pActiveWeapon->UpdateAccuracyPenalty();

	m_pApp->RandomSeed()((iSeed & 255) + 1);

	float fRand1 = m_pApp->RandomFloat()(0.0f, 1.0f);
	float fRandPi1 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);
	float fRand2 = m_pApp->RandomFloat()(0.0f, 1.0f);
	float fRandPi2 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);

	float fRandInaccurary = fRand1 * pActiveWeapon->GetInaccuracy();
	float fRandSpread = fRand2 * pActiveWeapon->GetSpread();

	float fSpreadX = cos(fRandPi1) * fRandInaccurary + cos(fRandPi2) * fRandSpread;
	float fSpreadY = sin(fRandPi1) * fRandInaccurary + sin(fRandPi2) * fRandSpread;

	m_qAimAngles.x += RAD2DEG(atan2f(fSpreadY, sqrtf(1.0f + fSpreadX * fSpreadX)));
	m_qAimAngles.y += RAD2DEG(atanf(fSpreadX));

	m_bDidNoSpread = true;

	// TODO
	/*if (HackGui::Instance()->Misc_AntiUnstrusted.GetValue())
	{
	pCmd->viewangles.pitch += RadToDeg(atan2(fSpreadY, sqrt(1.f + fSpreadX*fSpreadX))); //pitch/yaw
	pCmd->viewangles.yaw += RadToDeg(atan(fSpreadX));
	}
	else
	{
	pCmd->viewangles.pitch += RadToDeg(atan(sqrt(fSpreadX * fSpreadX + fSpreadY * fSpreadY))); //pitch/roll nospread
	pCmd->viewangles.roll += RadToDeg(atan2(-fSpreadX, fSpreadY));
	}*/
}

void CRagebot::ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity, bool bAbleToHit)
{
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	float fNextattack = pActiveWeapon->GetNextPrimaryAttack();
	float fServertime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;

	if (this->m_bAutoshoot && pActiveWeapon->IsSniper())
	{
		if (this->m_bAutoscope)
		{
			if (pLocalEntity->IsScoped())
			{
				this->Shoot(pUserCmd, fNextattack, fServertime);
			}
			else
			{
				pUserCmd->buttons |= IN_ATTACK2;
			}
		}
		else
		{
			this->Shoot(pUserCmd, fNextattack, fServertime);
		}
	}
	else if (this->m_bAutoshoot && bAbleToHit)
	{
		this->Shoot(pUserCmd, fNextattack, fServertime);
	}
	else if (!this->m_bAutoshoot && bAbleToHit)
	{
		if (m_pTarget->GetIsBacktracked() != -1)
		{
			this->Shoot(pUserCmd, fNextattack, fServertime);
		}
		else
		{
			this->Aim(pUserCmd);
		}
	}
}

void inline CRagebot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		this->Aim(pUserCmd);

		pUserCmd->buttons |= IN_ATTACK;
		m_bIsShooting = true;
	}
}

void inline CRagebot::Aim(CUserCmd* pUserCmd)
{
	// Write viewangles
	pUserCmd->viewangles[0] = m_qAimAngles.x;
	pUserCmd->viewangles[1] = m_qAimAngles.y;
}

bool CRagebot::IsAbleToApplyNoSpread()
{
	static ConVar* pCustomRandomSeed = m_pApp->CVar()->FindVar(CXorString("d}Ú·dn÷¡zoÚ¡bxñ­zT÷£yoê¯Hxà§s").ToCharArray());
	return atoi(pCustomRandomSeed->value) == 0;
}
bool CRagebot::IsNoSpread()
{
	static ConVar* pWeaponAccuracity = m_pApp->CVar()->FindVar(CXorString("`nä²xeÚ£thð°vhüydö²enä¦").ToCharArray());
	return atoi(pWeaponAccuracity->value) == 1;
}


void CRagebot::AutoRevolver(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	m_bDoingAutoRevolver = false;

	if (!m_bAutoRevolver)
		return;

	if (m_bIsShooting)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!activeWeapon)
		return;

	if (activeWeapon->GetWeaponId() != WEAPON_REVOLVER)
		return;

	if (activeWeapon->GetClip1() == 0)
		return;

	m_bDoingAutoRevolver = true;
	pUserCmd->buttons |= IN_ATTACK;
	float flPostponeFireReady = activeWeapon->GetPostPoneFireReady();
	if (flPostponeFireReady > 0 && flPostponeFireReady <= (pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick))
	{
		pUserCmd->buttons &= ~IN_ATTACK;
	}
}


float CRagebot::CalculateHitchance(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, IClientEntity* pTarget)
{
	if (m_bDoNoSpread && IsAbleToApplyNoSpread() ||
		IsNoSpread() ||
		!m_bCalculateHitchance)
		return 1.0f;

	int iHits = 0;
	const float fMaxHits = 256.0f;

	Vector vHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	Vector vForward;
	QAngle qCurAngles;

	Ray_t ray;
	trace_t trace;
	CTraceFilterOnlyPlayers traceFilter(pLocalEntity);

	pActiveWeapon->UpdateAccuracyPenalty();

	for (int i = 1; i <= 256; i++)
	{
		m_pApp->RandomSeed()(i);

		float fRand1 = m_pApp->RandomFloat()(0.0f, 1.0f);
		float fRandPi1 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);
		float fRand2 = m_pApp->RandomFloat()(0.0f, 1.0f);
		float fRandPi2 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);

		float fRandInaccurary = fRand1 * pActiveWeapon->GetInaccuracy();
		float fRandSpread = fRand2 * pActiveWeapon->GetSpread();

		float fSpreadX = cos(fRandPi1) * fRandInaccurary + cos(fRandPi2) * fRandSpread;
		float fSpreadY = sin(fRandPi1) * fRandInaccurary + sin(fRandPi2) * fRandSpread;

		qCurAngles = m_qAimAngles;
		qCurAngles.x -= RAD2DEG(atan2f(fSpreadY, sqrtf(1.0f + fSpreadX * fSpreadX)));
		qCurAngles.y -= RAD2DEG(atanf(fSpreadX));
		AngleVectors(qCurAngles, &vForward);

		ray.Init(vHeadPos, vHeadPos + (vForward * 8192.0f));
		m_pApp->EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
		if (trace.DidHitEntity(pTarget))
		{
			iHits++;
		}
	}

	return (float)(iHits / fMaxHits);
}