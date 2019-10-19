#ifndef __OVERRIDEVIEW_H__
#define __OVERRIDEVIEW_H__

#include "../source_sdk/CViewSetup.h"

struct OverrideViewParam
{
	CViewSetup* pViewSetup;
};

typedef void(__thiscall *OverrideView_t)(void*, CViewSetup*);
extern OverrideView_t g_pOverrideView;

void __fastcall hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup);

#endif // __OVERRIDEVIEW_H__