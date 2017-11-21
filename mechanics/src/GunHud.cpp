#include "GunHud.h"
#include "Application.h"

CGunHud::CGunHud()
	: m_pSurface(NULL), m_bCrosshair(false), m_bCrosshairShowRecoil(false),
	m_bSpreadCone(false), m_bSpreadConeShowRecoil(false), m_clrSpreadCone(SPREADCONE_DEFAULT_COLOR),
	m_bHitmarker(false), m_bHitmarkerSound(false), m_bHitmarkerHitpoint(false),
	m_fHitmarkerShowTime(HITMARKER_DEFAULT_TIME),
	m_fPlayerHurtTime(-1)
{
}

CGunHud::~CGunHud()
{
}

void CGunHud::Setup()
{
	// Gets m_pApp
	IFeature::Setup();

	// Grab GUI & default surface
	m_pGui = CGui::Instance();
	m_pSurface = m_pApp->Surface();

	sprintf(m_pHitmarkerSound, CXorString("2xè§sbäžK.ö").ToCharArray(), m_pApp->GetWorkingDirectory(), CXorString("bñ±x~ë¦9|ä´").ToCharArray());
}

void CGunHud::Think(void* pParameters)
{
	assert(pParameters != NULL);
	IClientEntity* pLocalEntity = (IClientEntity*)pParameters;

	// Get current middle
	m_iMiddleX = m_pGui->GetScreenWidth() / 2.0f;
	m_iMiddleY = m_pGui->GetScreenHeight() / 2.0f;

	// If we want to display something corresponding to recoil
	// calculate the point on screen our recoil is "aiming" at
	if (m_bCrosshairShowRecoil || m_bSpreadConeShowRecoil)
	{
		QAngle qAimPunchAngles = *pLocalEntity->GetAimPunchAngle() * m_pApp->GetRecoilCompensation();

		m_bRecoilIsNonZero = (qAimPunchAngles.x != 0.0f || qAimPunchAngles.y != 0.0f || qAimPunchAngles.z != 0.0f);
		if (m_bRecoilIsNonZero)
		{
			Vector vHeadPos, vForward, vScreenPos;

			// Get our headpos
			vHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

			// Get forward vector for our angles + recoil
			qAimPunchAngles += m_pApp->GetClientViewAngles();
			AngleVectors(qAimPunchAngles, &vForward);

			// Add forward vector to our headpos & get the screen position for that
			CGui::Instance()->WorldToScreen(vHeadPos + (vForward * 8192.0f), vScreenPos);
			m_iRcMiddleX = vScreenPos.x;
			m_iRcMiddleY = vScreenPos.y;
		}
	}
}

void CGunHud::TriggerHitmarker()
{
	if (!m_bHitmarker)
		return;

	m_fDrawHitmarkerStartTime = m_pApp->GlobalVars()->curtime;
	if (m_bHitmarkerSound)
	{
		PlaySoundA(m_pHitmarkerSound, NULL, SND_ASYNC | SND_FILENAME);
	}
}

void CGunHud::ResetHitmarker()
{
	m_fDrawHitmarkerStartTime = 0.0f;
}

void CGunHud::DrawCrosshair()
{
	if (!m_bCrosshair)
		return;

	const int crosshair_size = 12;

	int iMidX, iMidY;
	if (m_bCrosshairShowRecoil && !m_pApp->Ragebot()->DidNoRecoil() &&
		!m_pApp->Visuals()->GetNoVisualRecoil() && m_bRecoilIsNonZero)
	{
		iMidX = m_iRcMiddleX;
		iMidY = m_iRcMiddleY;
	}
	else
	{
		iMidX = m_iMiddleX;
		iMidY = m_iMiddleY;
	}

	// Outer lines
	m_pSurface->DrawSetColor(255, 255, 255, 255);
	m_pSurface->DrawLine(iMidX - crosshair_size, iMidY, iMidX + crosshair_size, iMidY);
	m_pSurface->DrawLine(iMidX, iMidY - crosshair_size, iMidX, iMidY + crosshair_size);

	// Inner crosshair
	m_pSurface->DrawSetColor(255, 255, 0, 0);
	m_pSurface->DrawLine(iMidX - (crosshair_size / 2), iMidY, iMidX + (crosshair_size / 2), iMidY);
	m_pSurface->DrawLine(iMidX, iMidY - (crosshair_size / 2), iMidX, iMidY + (crosshair_size / 2));
}

void CGunHud::DrawSpreadCone()
{
	if (!m_bSpreadCone)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	int iMidX, iMidY;
	if (m_bSpreadConeShowRecoil && m_bRecoilIsNonZero)
	{
		iMidX = m_iRcMiddleX;
		iMidY = m_iRcMiddleY;
	}
	else
	{
		iMidX = m_iMiddleX;
		iMidY = m_iMiddleY;
	}

	float fSpread = ((pActiveWeapon->GetInaccuracy() + pActiveWeapon->GetSpread()) * 320.f) / tanf(DEG2RAD(m_pApp->GetRenderFieldOfView()) * 0.5f) * m_pGui->GetScreenHeight() / 480.f;

	m_pSurface->DrawSetColor(m_clrSpreadCone);
	m_pSurface->DrawOutlinedCircle(iMidX, iMidY, fSpread, 64);
}

void CGunHud::DrawHitmarker()
{
	if (!m_bHitmarker)
		return;

	const int hitmarker_gap = 4;
	const int hitmarker_size = 6;

	float fTime = m_pApp->GlobalVars()->curtime - m_fDrawHitmarkerStartTime;
	if (fTime > m_fHitmarkerShowTime)
		return;

	int iAlpha = 255 - ((fTime / m_fHitmarkerShowTime) * 255.0f);
	if (iAlpha == 0) // This shouldn't appear, because if(...) above should be true then. but just to be safe
		return;

	m_pSurface->DrawSetColor(iAlpha, 255, 255, 255);
	m_pSurface->DrawLine(m_iMiddleX - (hitmarker_gap + hitmarker_size), m_iMiddleY - (hitmarker_gap + hitmarker_size), m_iMiddleX - hitmarker_gap, m_iMiddleY - hitmarker_gap); // Top left
	m_pSurface->DrawLine(m_iMiddleX - (hitmarker_gap + hitmarker_size), m_iMiddleY + (hitmarker_gap + hitmarker_size), m_iMiddleX - hitmarker_gap, m_iMiddleY + hitmarker_gap); // Bottom left
	m_pSurface->DrawLine(m_iMiddleX + hitmarker_gap, m_iMiddleY - hitmarker_gap, m_iMiddleX + (hitmarker_gap + hitmarker_size), m_iMiddleY - (hitmarker_gap + hitmarker_size)); // Top right
	m_pSurface->DrawLine(m_iMiddleX + hitmarker_gap, m_iMiddleY + hitmarker_gap, m_iMiddleX + (hitmarker_gap + hitmarker_size), m_iMiddleY + (hitmarker_gap + hitmarker_size)); // Bottom right
}

void CGunHud::DrawHitmarkerHitpoint()
{
	if (m_pApp->m_pHitmarker.size() == 0)
		return;

	float fCurtime = m_pApp->GlobalVars()->curtime;

	m_pSurface->DrawSetColor(255, 255, 255, 255);
	for (std::vector<HitmarkerEntry>::iterator it = m_pApp->m_pHitmarker.begin(); it != m_pApp->m_pHitmarker.end(); it++)
	{
		while (it->fStarttime + m_fHitmarkerShowTime < fCurtime)
		{
			it = m_pApp->m_pHitmarker.erase(it);
			if (it == m_pApp->m_pHitmarker.end())
			{
				return;
			}
		}

		if (m_fWeaponFireTime <= it->fStarttime && it->fStarttime <= m_fPlayerHurtTime)
		{
			Vector vScreen;
			if (m_pApp->Gui()->WorldToScreen(it->vHit, vScreen))
			{
				m_pSurface->DrawLine(vScreen.x - 4, vScreen.y - 4, vScreen.x + 4, vScreen.y + 4);
				m_pSurface->DrawLine(vScreen.x - 4, vScreen.y + 4, vScreen.x + 4, vScreen.y - 4);
			}
		}
	}
}