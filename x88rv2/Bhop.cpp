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
	// Setup code here
}

void CBhop::Update()
{
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

	DWORD* jump = (DWORD*)(m_pApp->ClientDll() + JUMP_ADDRESS_OFFSET);
	DWORD flag = *(DWORD*)((DWORD)pLocalEntity + JUMP_FLAG_OFFSET);
	if (GetAsyncKeyState(0x20))
	{
		if ((flag & 1) == 1)
		{
			*jump = 5;
		}
		else
		{
			*jump = 4;
		}
	}
	// Update code (once per in-game loop)
}
