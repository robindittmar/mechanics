#ifndef __CREATEMOVE_H__
#define __CREATEMOVE_H__

#include "UserCmd.h"

struct CreateMoveParam
{
	float fInputSampleTime;
	CUserCmd* pUserCmd;
};

typedef bool(__thiscall *CreateMove_t)(void*, float, CUserCmd*);
extern CreateMove_t g_pCreateMove;

bool __fastcall hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd);

#endif // __CREATEMOVE_H__