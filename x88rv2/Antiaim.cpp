#include "Antiaim.h"
#include "Application.h"

#define OFFSET_NEXTPRIMARYATTACK 0x31D8
#define OFFSET_TICKBASE 0x3424

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
	if (pUserCmd->buttons & IN_USE )//|| pUserCmd->buttons & IN_ATTACK)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (activeWeapon->IsNade())
	{
		CGrenade* activeGrenade = (CGrenade*)activeWeapon;
		if (activeGrenade->ThrowTime() > 0.f)
			return;
	}
	float test = *(float*)((DWORD)activeWeapon + OFFSET_NEXTPRIMARYATTACK);
	float  test3 = *(int*)((DWORD)pLocalEntity + OFFSET_TICKBASE) * 0.0078125;
	if (test > test3)
		return;

	if (pLocalEntity->MoveType() & MOVETYPE_LADDER)
		return;

	AntiAim aa = { PitchDown, YawStaticJitterBackwards };
	QAngle angles = m_pApp->ClientViewAngles();

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
	}

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
