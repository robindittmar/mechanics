#ifndef __FRAMESTAGENOTIFY_H__
#define __FRAMESTAGENOTIFY_H__

#include "../source_sdk/ClientFrameStage.h"

struct FrameStageNotifyParam
{
	ClientFrameStage_t curStage;
};

typedef void(__thiscall *FrameStageNotify_t)(void*, ClientFrameStage_t);
extern FrameStageNotify_t g_pFrameStageNotify;

void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage);

#endif // __FRAMESTAGENOTIFY_H__