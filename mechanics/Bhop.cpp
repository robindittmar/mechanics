#include "Bhop.h"
#include "Application.h"

CBhop::CBhop()
{
}

CBhop::~CBhop()
{
}

void CBhop::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

	if (!pParameters)
		return;

	if (!pLocalEntity)
		return;
	
	DWORD flags = pLocalEntity->GetFlags();
	DWORD moveType = pLocalEntity->GetMoveType();
	if (m_pApp->Gui()->IsMouseEnabled() &&
		pUserCmd->buttons & IN_JUMP)
	{
		if (!(flags & FL_ONGROUND) && !(flags & FL_SWIM) && moveType != MOVETYPE_LADDER)
		{
			pUserCmd->buttons &= ~IN_JUMP;
		}
	}
}
