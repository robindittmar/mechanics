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
	// Setup code here
}

void CMisc::ToggleNoFlash(bool enable, int flashPercentage)
{
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

	if (enable)
	{
		// not working, setting it to 0 instead of flashpercentage
		*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f - (255.0f * (1 - (flashPercentage / 100)));
	}
	else
	{
		*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f;
	}
}

void CMisc::Update()
{
	// Update code (once per in-game loop)
}
