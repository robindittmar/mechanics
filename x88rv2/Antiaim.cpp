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

	/*static CConsole console;
	console.Write("%d | %d\n", pUserCmd->command_number, pUserCmd->tick_count);*/

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

	AntiAim aa = { PitchDown, YawBackwards };
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
	//static int iCountFake = 0;
	static bool bFakeAngles = true;
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
			/*if(iCountFake < 8)
			{
				angles.y += 90.0f;
				*m_pApp->m_bSendPackets = false;
				iCountFake++;
			}
			else
			{
				angles.y -= 180.0f;
				*m_pApp->m_bSendPackets = true;
				iCountFake = 0;
			}*/
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
	}

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
