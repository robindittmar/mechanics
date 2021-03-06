#include "GunAccuracy.h"
#include "../Application.h"

CGunAccuracy::CGunAccuracy()
	: m_fRecoilCompensation(2.0f), m_bNoRecoil(false), m_bNoSpread(false)
{
	// TODO: This might go into the menu somewhere
	m_bIsEnabled = true;
}

CGunAccuracy::~CGunAccuracy()
{
}

void CGunAccuracy::ApplySpreadToAngles(QAngle& qAngles, IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, int iSeed)
{
	m_pApp->RandomSeed()((iSeed & 255) + 1);

	float fRand1 = m_pApp->RandomFloat()(0.0f, 1.0f);
	float fRandPi1 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);
	float fRand2 = m_pApp->RandomFloat()(0.0f, 1.0f);
	float fRandPi2 = m_pApp->RandomFloat()(0.0f, 2.0f * PI_F);

	float fRandInaccurary = fRand1 * pActiveWeapon->GetInaccuracy();
	float fRandSpread = fRand2 * pActiveWeapon->GetSpread();

	float fSpreadX = cos(fRandPi1) * fRandInaccurary + cos(fRandPi2) * fRandSpread;
	float fSpreadY = sin(fRandPi1) * fRandInaccurary + sin(fRandPi2) * fRandSpread;

	qAngles.x += RAD2DEG(atan2f(fSpreadY, sqrtf(1.0f + fSpreadX * fSpreadX)));
	qAngles.y += RAD2DEG(atanf(fSpreadX));
}

void CGunAccuracy::Think(void* pParameters)
{
	assert(pParameters != nullptr);

	if (!m_bIsEnabled)
		return;

	CreateMoveParam* pParam = (CreateMoveParam*)pParameters;
	CUserCmd* pUserCmd = pParam->pUserCmd;

	if (!(pUserCmd->buttons & IN_ATTACK))
		return;

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
	m_qAngles = QAngle(pUserCmd->viewangles);

	// Modify angles
	this->ApplyNoRecoil(pLocalEntity);
	this->ApplyNoSpread(pLocalEntity, pWeapon, pUserCmd->random_seed);

	// Restore new calculated angles
	pUserCmd->viewangles[0] = m_qAngles.x;
	pUserCmd->viewangles[1] = m_qAngles.y;
	pUserCmd->viewangles[2] = m_qAngles.z;
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