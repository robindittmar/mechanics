#ifndef __FIREEVENTCLIENTSIDE_H__
#define __FIREEVENTCLIENTSIDE_H__

#include "IGameEvent.h"

struct FireEventClientSideParam
{
	IGameEvent* pEvent;
};

typedef bool(__thiscall *FireEventClientSide_t)(void*, IGameEvent*);
extern FireEventClientSide_t g_pFireEventClientSide;

bool __fastcall hk_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent);

#endif // __FIREEVENTCLIENTSIDE_H__