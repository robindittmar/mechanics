#include "FrameStageNotify.h"
#include "Application.h"

FrameStageNotify_t g_pFrameStageNotify;

void __fastcall hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (pApp->EngineClient()->IsInGame() && pLocalEntity->IsAlive())
		{
			pApp->Resolver()->Think();

			pApp->LagCompensation()->Think();

			pApp->SkinChanger()->Think(pLocalEntity);
		}
	}
	else if (curStage == FRAME_RENDER_START)
	{
		if (pApp->EngineClient()->IsInGame())
		{
			if (pLocalEntity->IsAlive())
			{
				pApp->Visuals()->ThirdpersonAntiAim();
			}

			// PVS Fix, only needed while Rage and Mirror
			if (pApp->Ragebot()->GetEnabled() || pApp->Mirror()->GetEnabled())
			{
				for (int i = 0; i < pApp->EngineClient()->GetMaxClients(); i++)
				{
					IClientEntity* pCurEntity = pApp->EntityList()->GetClientEntity(i);

					if (!pCurEntity)
						continue;

					*(int*)((DWORD)pCurEntity + OFFSET_LASTOCCLUSIONCHECK) = pApp->GlobalVars()->framecount;
					*(int*)((DWORD)pCurEntity + OFFSET_OCCLUSIONFLAGS) = 0;
				}
			}
		}

		// Menu input handling
		pApp->Menu()->HandleInput();
	}

	g_pFrameStageNotify(ecx, curStage);
}
