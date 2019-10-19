#include "FireEventClientSide.h"
#include "../Application.h"

FireEventClientSide_t g_pFireEventClientSide;

bool __fastcall hk_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();
	pApp->SkinChanger()->ApplyCustomKillIcon(pEvent);

	return g_pFireEventClientSide(ecx, pEvent);
}