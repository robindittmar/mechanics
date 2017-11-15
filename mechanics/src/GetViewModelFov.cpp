#include "GetViewModelFov.h"
#include "Application.h"

GetViewModelFov_t g_pGetViewModelFov;

float __fastcall hk_GetViewModelFov(void* ecx, void* edx)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->Visuals()->GetViewmodelFov())
	{
		return pApp->Visuals()->GetViewmodelFovValue();
	}
	return g_pGetViewModelFov(ecx);
}
