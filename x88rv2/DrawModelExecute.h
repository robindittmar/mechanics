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

#endif // __DRAWMODELEXECUTE_H__