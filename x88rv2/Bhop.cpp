#include "Bhop.h"
#include "Application.h"

CBhop::CBhop()
{
}

CBhop::~CBhop()
{
}

void CBhop::Setup()
{
	m_pApp = CApplication::Instance();
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
	if (!m_pApp->EngineClient()->Con_IsVisible() &&
		GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (flags & FL_ONGROUND || flags & FL_INWATER)
		{
			pUserCmd->buttons |= IN_JUMP;
		}
		else
		{
			pUserCmd->buttons &= ~IN_JUMP;
		}
	}
}
