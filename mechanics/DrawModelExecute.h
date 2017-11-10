#ifndef __DRAWMODELEXECUTE_H__
#define __DRAWMODELEXECUTE_H__

#include "IMatRenderContext.h"
#include "IVModelRender.h"
#include "Vector.h"

struct DrawModelExecuteParam
{
	IMatRenderContext* ctx;
	const DrawModelState_t& state;
	const ModelRenderInfo_t& pInfo;
	matrix3x4_t* pCustomBoneToWorld;
};

typedef void(__thiscall *DrawModelExecute_t)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
extern DrawModelExecute_t g_pDrawModelExecute;

void __fastcall hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);

#endif // __DRAWMODELEXECUTE_H__