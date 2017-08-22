#include "Antiaim.h"
#include "Application.h"

CAntiAim::CAntiAim()
{
	m_iPitchSetting = PITCHANTIAIM_NONE;
	m_iYawSetting = YAWANTIAIM_NONE;
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
	if (pUserCmd->buttons & IN_USE || pUserCmd->buttons & IN_ATTACK || pUserCmd->buttons & IN_ATTACK2)
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
	switch (m_iPitchSetting)
	{
	case PITCHANTIAIM_UP:
		angles.x = -89.0f;
		break;
	case PITCHANTIAIM_DOWN:
		angles.x = 89.0f;
		break;
	case PITCHANTIAIM_NONE:
	default:
		break;
	}

	static bool bFakeAngles = true;
	static float trigger = 0.0f;
	// Yaw
	switch (m_iYawSetting)
	{
	case YAWANTIAIM_BACKWARDS:
		angles.y -= 180.0f;
		break;
	case YAWANTIAIM_STATICJITTERBACKWARDS:
		trigger += 15.0f;
		angles.y -= trigger > 50.0f ? -145.0f : 145.0f;

		if (trigger > 100.0f)
		{
			trigger = 0.0f;
		}
		break;
	case YAWANTIAIM_REALLEFTFAKERIGHT:
		if(m_pApp->m_bGotSendPackets)
		{
			if(bFakeAngles)
			{
				angles.y += 90.0f;
			}
			else
			{
				//angles.y -= 180.0f;
				angles.y -= 90.0f;
			}

			*m_pApp->m_bSendPackets = bFakeAngles;
			bFakeAngles = !bFakeAngles;
		}
		break;
	case YAWANTIAIM_NONE:
	default:
		break;
	}

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
