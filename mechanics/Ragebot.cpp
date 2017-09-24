#include "Ragebot.h"
#include "Application.h"

CRagebot::CRagebot()
{
	m_bSilentAim = false;
	m_bAutoshoot = true;
	m_bAutoscope = false;
	m_bDoNoRecoil = true;
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
	// Apply viewangles & shoot if necessary
	this->ApplyViewanglesAndShoot(pUserCmd, pLocalEntity, bAbleToHit);
}

void inline CRagebot::ResetTickVariables()
{
	m_bIsShooting = false;
	m_bDidNoRecoil = false;

	m_fDamage = 0.0f;
}

void CRagebot::ApplyNoRecoil(IClientEntity* pLocalEntity)
{
	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if (m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		m_qAimAngles.x -= aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_qAimAngles.y -= aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_bDidNoRecoil = true;
	}
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
		this->Aim(pUserCmd);
	}
}

void inline CRagebot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		this->Aim(pUserCmd);

		// Shoot
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