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

	static bool bToggle = false;
	Antiaim antiAimInitizalize = { DOWN, BACKWARDS };
	QAngle angles = m_pApp->ClientViewAngles();

	// Pitch
	switch (antiAimInitizalize.pitch)
	{
	case DOWN:
		angles.x = 89.0f;
		break;
	}

	// Yaw
	switch (antiAimInitizalize.yaw)
	{
	case BACKWARDS:
		angles.y -= 180.0f;
		break;
	case JITTER_BACKWARDS:
		if (bToggle)
		{
			angles.y -= 145.0f;
		}
		else
		{
			angles.y -= 215.0f;
		}
		bToggle = !bToggle;
		break;
	}

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
