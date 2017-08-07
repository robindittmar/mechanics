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
		
	DWORD flag = *(DWORD*)((DWORD)pLocalEntity + JUMP_FLAG_OFFSET);
	if (pLocalEntity->GetVelocity() > 0.f &&
		!m_pApp->EngineClient()->Con_IsVisible() &&
		GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (flag & FL_ONGROUND || flag & FL_INWATER)
		{
			pUserCmd->buttons |= IN_JUMP;
		}
		else
		{
			pUserCmd->buttons &= ~IN_JUMP;
		}
	}
}
