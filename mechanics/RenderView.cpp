#include "RenderView.h"
#include "Application.h"

RenderView_t g_pRenderView;

void __fastcall hk_RenderView(void* ecx, void* edx, const CViewSetup& view, CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw)
{
	CApplication* pApp = CApplication::Instance();

	pApp->Mirror()->OnRenderView(ecx, view, hudViewSetup, nClearFlags, whatToDraw);
	pApp->Visuals()->DrawBulletTracer();

	g_pRenderView(ecx, view, hudViewSetup, nClearFlags, whatToDraw);
}
