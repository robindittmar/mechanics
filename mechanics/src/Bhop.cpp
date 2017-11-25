#include "Bhop.h"
#include "Application.h"

CBhop::CBhop()
	: m_iAutoStrafeMode(AUTOSTRAFEMODE_NONE), m_bCircleStrafe(false),
	m_iCircleStrafeStartDirection(CIRCLESTRAFEDIRECTION_RIGHT)
{
}

CBhop::~CBhop()
{
}

void CBhop::Think(void* pParameters)
{
	assert(pParameters != nullptr);

	if (!m_bIsEnabled)
		return;

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity)
		return;
	
	this->Bhop(pLocalEntity, pUserCmd);
	this->AutoStrafe(pLocalEntity, pUserCmd);
	this->CircleStrafe(pLocalEntity, pUserCmd);
}

void CBhop::Bhop(IClientEntity* pLocalEntity, CUserCmd* pUserCmd)
{
	DWORD flags = pLocalEntity->GetFlags();
	DWORD moveType = pLocalEntity->GetMoveType();
	if (CGui::Instance()->IsMouseEnabled() &&
		pUserCmd->buttons & IN_JUMP)
	{
		if (!(flags & FL_ONGROUND) && !(flags & FL_SWIM) && moveType != MOVETYPE_LADDER)
		{
			pUserCmd->buttons &= ~IN_JUMP;
		}
	}
}

void CBhop::AutoStrafe(IClientEntity* pLocalEntity, CUserCmd* pUserCmd)
{
	if (m_iAutoStrafeMode == 0)
		return;

	DWORD moveType = pLocalEntity->GetMoveType();
	if (!(pLocalEntity->GetFlags() & FL_ONGROUND) &&
		!(moveType & MOVETYPE_NOCLIP) &&
		!(moveType & MOVETYPE_LADDER))
	{
		switch (m_iAutoStrafeMode)
		{
		case AUTOSTRAFEMODE_RAGE:
			pUserCmd->forwardmove = (1800.f * 4.f) / pLocalEntity->GetVelocity()->Length();
			if (pUserCmd->mousedx > 1 || pUserCmd->mousedx < -1) {
				pUserCmd->sidemove = pUserCmd->mousedx < 0.f ? -450.f : 450.f;
			}
			else {
				pUserCmd->sidemove = (pUserCmd->command_number % 2) == 0 ? -450.f : 450.f;
			}
			break;
		case AUTOSTRAFEMODE_LEGIT:
			if (pUserCmd->mousedx > 0)
			{
				pUserCmd->sidemove = 450;
			}
			else if (pUserCmd->mousedx < 0)
			{
				pUserCmd->sidemove = -450;
			}
			break;
		case AUTOSTRAFEMODE_NONE:
		default:
			break;
		}
	}
}

void CBhop::CircleStrafe(IClientEntity* pLocalEntity, CUserCmd* pUserCmd)
{
	if (!m_bCircleStrafe)
		return;

	//todo: which key? maybe better method?
	if (!GetAsyncKeyState(VK_MENU))
		return;

	static float fStrafeAngle = 0.0f;

	Vector vOrigin = *pLocalEntity->GetOrigin();

	Vector Velocity = *pLocalEntity->GetVelocity();
	Velocity.z = 0;

	float Speed = Velocity.Length();
	if (Speed < 45)
		Speed = 45;

	if (Speed > 750)
		Speed = 750;

	float FinalPath = Utils::GetTraceFractionWorldProps(vOrigin + Vector(0, 0, 10), vOrigin + Vector(0, 0, 10) + Velocity / 2.0f);
	float DeltaAngle = m_iCircleStrafeStartDirection * fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / m_pApp->GlobalVars()->interval_per_tick)) * 4.20f, 2.0f);
	fStrafeAngle += DeltaAngle;

	// Autojump
	if (!(pLocalEntity->GetFlags() & FL_ONGROUND))
		pUserCmd->buttons &= ~IN_JUMP;
	else
		pUserCmd->buttons |= IN_JUMP;

	if (fabs(fStrafeAngle) >= 360.0f)
	{
		fStrafeAngle = 0.0f;
	}
	else
	{
		pUserCmd->forwardmove = cos((fStrafeAngle + 90 * m_iCircleStrafeStartDirection) * (PI_F / 180.0f)) * 450.0f;
		pUserCmd->sidemove = sin((fStrafeAngle + 90 * m_iCircleStrafeStartDirection) * (PI_F / 180.0f)) * 450.0f;
	}
}