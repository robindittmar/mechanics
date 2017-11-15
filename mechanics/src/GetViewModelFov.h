#ifndef __GETVIEWMODELFOV_H__
#define __GETVIEWMODELFOV_H__

struct GetViewModelFovParam
{
	// No params :(
};

typedef float(__thiscall *GetViewModelFov_t)(void*);
extern GetViewModelFov_t g_pGetViewModelFov;

float __fastcall hk_GetViewModelFov(void* ecx, void* edx);

#endif // __GETVIEWMODELFOV_H__