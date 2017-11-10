#include "DrawModelExecute.h"
#include "Application.h"

DrawModelExecute_t g_pDrawModelExecute;

void __fastcall hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	IMaterial* pHands = NULL;
	if (pInfo.pModel && pApp->EngineClient()->IsInGame())
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);

		pApp->Chams()->DrawFakeAngle(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pApp->Chams()->RenderPlayerChams(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pApp->Chams()->RenderWeaponChams(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pHands = pApp->Visuals()->HandsDrawStyle(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}

	// Call original func
	g_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
	// This is necessary for stuff to work properly
	pApp->ModelRender()->ForcedMaterialOverride(NULL);
}
