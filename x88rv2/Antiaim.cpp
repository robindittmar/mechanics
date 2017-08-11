#include "Antiaim.h"
#include "Application.h"


CAntiAim::CAntiAim()
{
}

CAntiAim::~CAntiAim()
{
}

void CAntiAim::Setup()
{
	m_pApp = CApplication::Instance();
}

void CAntiAim::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	if (!pUserCmd)
		return;

	// Allow us to use (open doors, defuse bomb) and shoot normally
	if (pUserCmd->buttons & IN_USE || pUserCmd->buttons & IN_ATTACK)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (activeWeapon->IsNade())
	{
		CGrenade* activeGrenade = (CGrenade*)activeWeapon;
		if (activeGrenade->ThrowTime() > 0.f)
			return;
	}

	if (pLocalEntity->MoveType() & MOVETYPE_LADDER)
		return;

	AntiAim aa = { PitchDown, YawBackwardsFakeRight };
	QAngle angles;
	if(m_pApp->Aimbot()->HasTarget())
	{
		angles = *m_pApp->Aimbot()->GetAimAngles();
	}
	else
	{
		angles = m_pApp->ClientViewAngles();
	}

	// Pitch
	switch (aa.pitchAA)
	{
	default:
		break;
	case PitchUp:
		angles.x = -89.0f;
		break;
	case PitchDown:
		angles.x = 89.0f;
		break;
	}

	// Yaw
	static bool bSendPacketsToggle = true;
	static float trigger = 0.0f;
	switch (aa.yawAA)
	{
	default:
		break;
	case YawBackwards:
		angles.y -= 180.0f;
		break;
	case YawStaticJitterBackwards:
		trigger += 15.0f;
		angles.y -= trigger > 50.0f ? -145.0f : 145.0f;

		if (trigger > 100.0f)
		{
			trigger = 0.0f;
		}
		break;
	case YawBackwardsFakeRight:
		if(m_pApp->m_bGotSendPackets)
		{
			if(bSendPacketsToggle)
			{
				angles.y -= 90.0f;
			}
			else
			{
				angles.y -= 180.0f;
			}
			*m_pApp->m_bSendPackets = bSendPacketsToggle;
			bSendPacketsToggle = !bSendPacketsToggle;
		}
		break;
	}

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
