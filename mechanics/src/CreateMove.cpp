#include "CreateMove.h"
#include "Application.h"

CreateMove_t g_pCreateMove;

bool __fastcall hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = g_pCreateMove(ecx, fInputSampleTime, pUserCmd);

	CApplication* pApp = CApplication::Instance();
	pApp->LagCompensation()->SetTickcount(pUserCmd->tick_count);

	// Instantly return
	if (!pUserCmd || !pUserCmd->command_number)
		return false;

	int iOldSeed = pUserCmd->random_seed;
	pUserCmd->random_seed = MD5_PseudoRandom(pUserCmd->command_number) & 0x7FFFFFFF;

	if (!pApp->m_bGotSendPackets)
	{
		uintptr_t* fp;
#ifdef __MINGW32__
		asm("mov %%ebp, a" : /* no output */ : "a"(fp) : "cc"/*??*/ );
#else
		__asm mov fp, ebp;
#endif
		pApp->m_bSendPackets = (bool*)(*fp - 0x1C);
		pApp->m_bGotSendPackets = true;
	}

	if (pApp->EngineClient()->IsInGame())
	{
		IClientEntity* pLocalEntity = pApp->GetLocalPlayer();

		// Visuals
		pApp->SoundEsp()->Think();
		pApp->GunHud()->Think();
		pApp->Radar()->Think();

		if (pLocalEntity->IsAlive())
		{
			//todo: ghetto !
			/*static int bNoFlashTriggered = 0;
			if (bNoFlashTriggered < 2)
			{
				if (bNoFlashTriggered == 0 ||
					*(float*)((DWORD)pLocalEntity + Offsets::m_flFlashMaxAlpha) != (255.0f - (255.0f * (1.0f - (pApp->Visuals()->GetFlashPercentage() / 100.0f)))))
				{
					pApp->Visuals()->NoFlash(pApp->Visuals()->GetFlashPercentage());
					bNoFlashTriggered++;
				}
			}*/

			// Setting every lby update prediction
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				CResolverPlayer* pCurResolverPlayer = pApp->Resolver()->GetResolverPlayer(i);
				pCurResolverPlayer->SetPredLbyUpdateTime(pApp->GlobalVars()->curtime + 1.1f);
			}

			// Save Viewangles before doing stuff
			pApp->EngineClient()->GetViewAngles(pApp->GetClientViewAngles());
			QAngle qOldAngles = pApp->GetClientViewAngles();

			// Create instance of CreateMoveParam
			CreateMoveParam createMoveParam = { fInputSampleTime, pUserCmd };

			// Update Aimbot & AutoRevolver
			pApp->Ragebot()->Think((void*)&createMoveParam);
			pApp->Ragebot()->AutoRevolver(pUserCmd);

			// Update Legitbot
			pApp->Legitbot()->Think((void*)&createMoveParam);

			// Update Triggerbot
			pApp->Triggerbot()->Think((void*)&createMoveParam);

			// Update Bunnyhop
			pApp->Bhop()->Think(pUserCmd);

			// Update AntiAim
			// *m_pApp->m_bSendPacket = pUserCmd->command_number % 2;
			// dann in Antiaim, fakeduck etc auf sendPacket false oder true checken
			// nur beim lby break auf false setzen
			// und fakelag als erstes direkt damit das entscheiden kann ob gechoked oder nicht
			// zu sehen wv grad gechoked sind -> netchannel->choked_packets (oder so)
			pApp->Fakelag()->CalcAdaptiveChokeAmount();
			// FAKEWALK
			pApp->Fakeduck()->Think(pUserCmd);
			pApp->AntiAim()->Think(pUserCmd);
			//}

			// Miscs
			pApp->Misc()->AutoPistol(pUserCmd);
			pApp->Misc()->NoRecoil(pUserCmd);
			pApp->Fakelag()->Think((void*)pUserCmd);
			pApp->Misc()->JumpScout(pUserCmd);

			// Apply NoRecoil/NoSpread
			pApp->GunAccuracy()->Think((void*)&createMoveParam);

			// todo: auslagern!
			if (pUserCmd->buttons & IN_ATTACK)
			{
				if (pApp->Ragebot()->GetSelectedTargetIdx() != -1)
				{
					CTarget* pTarget = pApp->Ragebot()->GetSelectedTarget();
					if (pTarget)
					{
						int iIsBacktracked = pTarget->GetIsBacktracked();
						if (iIsBacktracked != -1)
						{
							IClientEntity* pTargetEntity = pTarget->GetEntity();
							if (pTargetEntity)
							{
								//pApp->LagCompensation()->GetLCList(pTarget->GetEntity()->EntIndex()).SetPlayerEntry(pTarget->GetEntity(), TEST_INDEX);
								pUserCmd->tick_count = pApp->LagCompensation()->GetLCList(pTargetEntity->EntIndex())->m_pPlayerEntries[iIsBacktracked].m_iTickCount + 1;
							}
						}
					}
				}
				
				int iTick = pApp->LagCompensation()->RestorePlayerClosestToCrosshair();
				if (iTick != -1)
				{
					pUserCmd->tick_count = iTick + 1;
				}
			}

			// Correct movement & angles
			CorrectMovement(pUserCmd, qOldAngles);
			NormalizeAngles(pUserCmd);

			// Set ViewAngles we prepared for display
			pApp->EngineClient()->SetViewAngles(pApp->GetClientViewAngles());

			bool bIsSniper = false;
			CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
			if (pActiveWeapon)
			{
				bIsSniper = pActiveWeapon->IsSniper();
			}

			// TODO: besser!!
			if (!*pApp->m_bSendPackets && pApp->AntiAim()->IsFakeYaw() ||
				*pApp->m_bSendPackets && !pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bLbyUpdate ||
				!pApp->AntiAim()->GetEnabled() ||
				(pUserCmd->buttons & IN_ATTACK) && !pApp->Ragebot()->IsDoingAutoRevolver() ||
				!bIsSniper && pUserCmd->buttons & IN_ATTACK2 ||
				pUserCmd->buttons & IN_USE)
			{
				QAngle cmdAngles(pUserCmd->viewangles);
				pApp->SetLastTickViewAngles(cmdAngles);
			}
		}
		else
		{
			pApp->m_bLBY = false;
		}
	}

	// Restore old value, just incase Valve decides to check it one day
	pUserCmd->random_seed = iOldSeed;
	return false;
}
