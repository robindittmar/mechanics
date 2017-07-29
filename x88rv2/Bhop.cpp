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
	if (m_bIsEnabled)
	{
		IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

		DWORD* dwForceJump = (DWORD*)(m_pApp->ClientDll() + FORCEJUMP_OFFSET);
		DWORD flag = *(DWORD*)((DWORD)pLocalEntity + JUMP_FLAG_OFFSET);
		if (*(DWORD*)((DWORD)pLocalEntity + VELOCITY_OFFSET) > 0 &&
			!m_pApp->EngineClient()->Con_IsVisible() &&
			GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if ((flag & 1) == 1)
			{
				*dwForceJump = 5;
			}
			else
			{
				*dwForceJump = 4;
			}
		}
	}
}
