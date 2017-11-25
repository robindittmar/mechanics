#include "GunAccuracy.h"
#include "Application.h"

CGunAccuracy::CGunAccuracy()
	: m_fRecoilCompensation(2.0f), m_bNoRecoil(false), m_bNoSpread(false)
{
}

CGunAccuracy::~CGunAccuracy()
{
}

void CGunAccuracy::Think(void* pParameters)
{
	assert(pParameters != nullptr);

	if (!m_bIsEnabled)
		return;

	CreateMoveParam* pParam = (CreateMoveParam*)pParameters;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsAlive())
		return;

	CWeapon* pWeapon = pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (pWeapon->IsKnife() ||
		pWeapon->IsNade() ||
		pWeapon->IsC4())
		return;

	// Store angles
	m_qAngles = QAngle(pParam->pUserCmd->viewangles);

	// Modify angles
	this->ApplyNoRecoil(pLocalEntity);
	this->ApplyNoSpread(pLocalEntity, pWeapon, pParam->pUserCmd->random_seed);

	// Restore new calculated angles
	pParam->pUserCmd->viewangles[0] = m_qAngles.x;
	pParam->pUserCmd->viewangles[1] = m_qAngles.y;
	pParam->pUserCmd->viewangles[2] = m_qAngles.z;
}

void CGunAccuracy::ApplyNoRecoil(IClientEntity* pLocalEntity)
{
	if (!m_bNoRecoil)
		return;
	
	QAngle qAimPunch = *pLocalEntity->GetAimPunchAngle() * m_pApp->GunAccuracy()->GetRecoilCompensation();
	m_qAngles -= qAimPunch;
	m_pApp->m_oldAimPunchAngle = qAimPunch;
}

void CGunAccuracy::ApplyNoSpread(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, int iSeed)
{
	if (!m_bNoSpread)
		return;

	/*if (IsNoSpread())
		return;

	if (!IsAbleToApplyNoSpread())
		return;*/

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

	m_qAngles.x += RAD2DEG(atan2f(fSpreadY, sqrtf(1.0f + fSpreadX * fSpreadX)));
	m_qAngles.y += RAD2DEG(atanf(fSpreadX));

	// TODO
	/*if (not_untrusted)
	{
	pCmd->viewangles.pitch += RadToDeg(atan2(fSpreadY, sqrt(1.f + fSpreadX*fSpreadX))); //pitch/yaw
	pCmd->viewangles.yaw += RadToDeg(atan(fSpreadX));
	}
	else // untrusted
	{
	pCmd->viewangles.pitch += RadToDeg(atan(sqrt(fSpreadX * fSpreadX + fSpreadY * fSpreadY))); //pitch/roll nospread
	pCmd->viewangles.roll += RadToDeg(atan2(-fSpreadX, fSpreadY));
	}*/
}