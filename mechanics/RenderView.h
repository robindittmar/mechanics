#ifndef __RENDERVIEW_H__
#define __RENDERVIEW_H__

#include "CViewSetup.h"

struct RenderViewParam
{
	const CViewSetup& view;
	CViewSetup& hudViewSetup;
	int nClearFlags;
	int whatToDraw;
};

typedef void(__thiscall *RenderView_t)(void*, const CViewSetup&, const CViewSetup&, int, int);
extern RenderView_t g_pRenderView;

void __fastcall hk_RenderView(void* ecx, void* edx, const CViewSetup& view, CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);

#endif // __RENDERVIEW_H__