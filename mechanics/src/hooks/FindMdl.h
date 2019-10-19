#ifndef __FINDMDL_H__
#define __FINDMDL_H__

#include "../source_sdk/IMDLCache.h"

struct FindMdlParam
{
	const char* pName;
};

typedef MDLHandle_t(__thiscall *FindMdl_t)(void*, char*);
extern FindMdl_t g_pFindMdl;

MDLHandle_t __fastcall hk_FindMDL(void* ecx, void* edx, char* FilePath);

#endif // __FINDMDL_H__