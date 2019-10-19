#include "RenderSmokeOverlay.h"
#include "../Application.h"

RenderSmokeOverlay_t g_pRenderSmokeOverlay;

void __fastcall hk_RenderSmokeOverlay(void* ecx, void* edx, bool bUnknown)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->Visuals()->GetNoSmoke())
		return;

	g_pRenderSmokeOverlay(ecx, bUnknown);
}
