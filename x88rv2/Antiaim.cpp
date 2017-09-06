#include "Antiaim.h"
#include "Application.h"
#include "Console.h"

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

float GetOutgoingLatency()
{
	INetChannelInfo *nci = CApplication::Instance()->EngineClient()->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}

bool NextLBYUpdate()
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

	pApp->m_bLbyUpdate = false;

	bool bPlayerMoving = pLocalEntity->GetVelocity()->Length2D() > 0.1f;
	if (bPlayerMoving)
		return false;

	float value = abs(pApp->m_flLbyUpdateTime - pApp->m_flPredLbyUpdateTime - GetOutgoingLatency());
	if (value > 1.0f)
	{
		if (value > 1.1f)
		{
			pApp->m_flLbyUpdateTime += 1.125f;
			return false;
		}

		pApp->m_bLbyUpdate = true;
		return true;
	}
	return false;
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
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (activeWeapon->IsKnife() && pUserCmd->buttons & IN_ATTACK2)
		return;

	if (activeWeapon->IsNade())
	{
		CGrenade* activeGrenade = (CGrenade*)activeWeapon;
		if (activeGrenade->GetThrowTime() > 0.f)
			return;
	}

	if (pLocalEntity->GetMoveType() & MOVETYPE_LADDER)
		return;

	QAngle angles;
	if (m_pApp->Ragebot()->HasTarget())
	{
		angles = *m_pApp->Ragebot()->GetAimAngles();
	}
	else
	{
		angles = m_pApp->GetClientViewAngles();
	}

	static bool bFakeAngles = true;

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

	static float trigger = 0.0f;

	static float flRealAngle = 0.0f;
	static float flFakeAngle = 0.0f;

	// Yaw
	switch (m_iYawSetting)
	{
	case YAWANTIAIM_REALRIGHTFAKELEFT:
		m_bIsFakeYaw = true;

		flRealAngle = 90.0f;
		flFakeAngle = -90.0f;

		if (bFakeAngles)
		{
			angles.y += flRealAngle;
		}
		else
		{
			angles.y += flFakeAngle;
		}
		*m_pApp->m_bSendPackets = bFakeAngles;
		bFakeAngles = !bFakeAngles;
		break;

	case YAWANTIAIM_REALLEFTFAKERIGHT:
		m_bIsFakeYaw = true;

		flRealAngle = 90.0f;
		flFakeAngle = -90.0f;

		if (bFakeAngles)
		{
			angles.y += flFakeAngle;
		}
		else
		{
			angles.y += flRealAngle;
		}

		*m_pApp->m_bSendPackets = bFakeAngles;
		bFakeAngles = !bFakeAngles;
		break;
	case YAWANTIAIM_BACKWARDS:
		m_bIsFakeYaw = false;

		angles.y -= 180.0f;
		break;
	case YAWANTIAIM_STATICJITTERBACKWARDS:
		m_bIsFakeYaw = false;

		trigger += 15.0f;
		angles.y -= trigger > 50.0f ? -145.0f : 145.0f;

		if (trigger > 100.0f)
		{
			trigger = 0.0f;
		}
		break;
	case YAWANTIAIM_NONE:
	default:
		m_bIsFakeYaw = false;
		break;
	}

	if (m_bIsFakeYaw && NextLBYUpdate() && !*m_pApp->m_bSendPackets)
	{
		angles.y += -flRealAngle + flFakeAngle;
	}
	
	bool lbyUpdate = false;
	if (m_pApp->m_flOldLby != pLocalEntity->GetLowerBodyYaw())
	{
		m_pApp->m_flOldLby = pLocalEntity->GetLowerBodyYaw();
		m_pApp->m_flRealLbyUpdateTime = m_pApp->m_flLbyUpdateTime = m_pApp->GlobalVars()->curtime;
		lbyUpdate = true;
	}
	
	if (m_bIsFakeYaw && pLocalEntity->GetVelocity()->Length2D() > 0.1f)
	{
		m_pApp->m_bLbyUpdate = true;
		angles.y += RandomInt(-180, 180);
	}

	m_pApp->m_bLBY = m_pApp->m_flRealLbyUpdateTime + 1.2 < m_pApp->GlobalVars()->curtime;

	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}
