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
	DWORD moveType = *(DWORD*)((DWORD)pLocalEntity + 0x258);
	
	if (moveType & MOVETYPE_LADDER)
		return;

	AntiAim aa = { DOWN, STATIC_JITTER_BACKWARDS };
	QAngle angles = m_pApp->ClientViewAngles();

	// Pitch
	switch (aa.pitchAA)
	{
	case DOWN:
		angles.x = 89.0f;
		break;
	}


	// Yaw
	static float trigger = 0.0f;
	switch (aa.yawAA)
	{
	case BACKWARDS:
		angles.y -= 180.0f;
		break;
	case STATIC_JITTER_BACKWARDS:
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
