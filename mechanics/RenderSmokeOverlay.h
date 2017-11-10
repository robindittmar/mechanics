#ifndef __RENDERSMOKEOVERLAY_H__
#define __RENDERSMOKEOVERLAY_H__

struct RenderSmokeOverlayParam
{
	bool bUnknown;
};

typedef void(__thiscall *RenderSmokeOverlay_t)(void*, bool);
extern RenderSmokeOverlay_t g_pRenderSmokeOverlay;

void __fastcall hk_RenderSmokeOverlay(void* ecx, void* edx, bool bUnknown);

#endif // __RENDERSMOKEOVERLAY_H__