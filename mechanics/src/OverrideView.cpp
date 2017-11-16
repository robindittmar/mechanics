#include "OverrideView.h"
#include "Application.h"

OverrideView_t g_pOverrideView;

void __fastcall hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup)
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity;

	if (pApp->EngineClient()->IsInGame())
	{
		pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

		if (pLocalEntity->IsAlive())
		{
			pApp->Visuals()->FovChange(pViewSetup);
			pApp->Visuals()->NoVisualRecoil(pViewSetup);
		}
		pApp->Visuals()->Thirdperson();
	}

	// Store FOV to use it for SpreadCone and other FOV based renderings
	pApp->SetRenderFieldOfView(pViewSetup->fov);

	return g_pOverrideView(ecx, pViewSetup);
}
