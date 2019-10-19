#include "FindMdl.h"

FindMdl_t g_pFindMdl;

MDLHandle_t __fastcall hk_FindMDL(void* ecx, void* edx, char* filePath)
{
	return g_pFindMdl(ecx, filePath);
}
