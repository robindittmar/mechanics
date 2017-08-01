#include "Misc.h"
#include "Application.h"

CMisc::CMisc()
{
}

CMisc::~CMisc()
{
}

void CMisc::Setup()
{
	m_pApp = CApplication::Instance();
}

void CMisc::NoFlash(int flashPercentage)
{
	if (m_bIsEnabled)
	{
		//todo: check if noflash enabled
		IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

		*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f - (255.0f * (1.0f - ((float)flashPercentage / 100.0f)));
	}
}

void CMisc::Update(void* pParameters)
{
	// Update code (once per in-game loop)
}
