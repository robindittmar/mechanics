#include "Application.h"

CreateMove_t CApplication::m_pCreateMove;
FrameStageNotify_t CApplication::m_pFrameStageNotify;
OverrideView_t CApplication::m_pOverrideView;
DrawModelExecute_t CApplication::m_pDrawModelExecute;
PaintTraverse_t CApplication::m_pPaintTraverse;
PlaySound_t CApplication::m_pPlaySound;
GetViewModelFov_t CApplication::m_pGetViewModelFov;
FireEventClientSide_t CApplication::m_pFireEventClientSide;
RenderView_t CApplication::m_pRenderViewFn;
RenderSmokeOverlay_t CApplication::m_pRenderSmokeOverlay;
EmitSound1_t CApplication::m_pEmitSound1;
EmitSound2_t CApplication::m_pEmitSound2;

RecvVarProxy_t CApplication::m_pSequenceProxy;
RecvVarProxy_t CApplication::m_pLowerBodyYawProxy;

CApplication* CApplication::Instance()
{
	static CApplication inst;
	return &inst;
}

void CApplication::Run(HMODULE hModule)
{
	m_hModule = hModule;

	this->Setup();
	this->Hook();
}

void CApplication::Detach()
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Detaching ...\n");
#endif

	// Enable mouse again
	this->m_pGui->SetEnableMouse(true);

	// Unregister listener
	this->m_pGameEventManager->RemoveListener(&this->m_gameEventListener);

	// Unhook everything
	this->Unhook();

	// Free ResourceManager
	delete g_pResourceManager;
	g_pResourceManager = NULL;

#ifdef _DEBUG
	// Free console
	delete g_pConsole;
	g_pConsole = NULL;
#endif

	// ----------- Our stuff -----------

	// Thirdperson
	if (m_pInput->m_fCameraInThirdPerson)
	{
		m_pInput->m_fCameraInThirdPerson = false;
	}

	// FovChanger
	if (m_visuals.GetFovChangeScoped())
	{
		ConVar* pZoomSensitivity = m_pCVar->FindVar(CXorString("mdÍØHx‡¨dbÒ´abÒªHy‰∂~d⁄Øx~ˆß").ToCharArray());
		pZoomSensitivity->SetValue(m_visuals.GetZoomSensitivity());
	}

	// NoSmoke
	m_visuals.NoSmoke(false);

	// SkinChanger Modeldelete
	if (m_skinchanger.GetEnabled())
		m_pClientState->ForceFullUpdate();

	// ClanTag
	if (m_misc.GetIsCustomClanTag())
	{
		m_misc.SetClanTag(NULL);
		m_misc.ApplyClanTag();
	}

	// Nightmode
	if (m_visuals.GetNightmode())
	{
		m_visuals.SetNightmode(false);
		m_visuals.SetNightmodePerfomed(false);
		m_visuals.Nightmode();
	}


	// Free & Exit
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFreeLibrary, this->m_hModule, NULL, NULL);
}

void CApplication::LoadSkinChangerConfig()
{
	// Nicos Knife
	this->m_skinchanger.AddModelReplacement(
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄Ø.TÁ£n%Ë¶{").ToCharArray(),
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄†vrÍ¨r´Øsg").ToCharArray()
	);
	this->m_skinchanger.AddSkinReplacement(
		WEAPON_KNIFE_M9_BAYONET,
		new CSkinMetadata(
			WEAPON_KNIFE_BAYONET,
			416,
			0,
			-1,
			3
		)
	);
	this->m_skinchanger.AddKillIconReplacement(
		CXorString("|eÏ§rTË˚Hi‰ªxe‡∂").ToCharArray(),
		CXorString("uj¸≠ynÒ").ToCharArray()
	);

	// Robins Knifes (T -> Butterfly Fade Stattrak)
	this->m_skinchanger.AddSkinReplacement(
		WEAPON_KNIFE_T,
		new CSkinMetadata(
			WEAPON_KNIFE_BUTTERFLY,
			38,
			0,
			420,
			3
		)
	);
	this->m_skinchanger.AddModelReplacement(
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄¶rm‰∑{⁄∂9f·Æ").ToCharArray(),
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄†bÒßemÈª9f·Æ").ToCharArray()
	);
	this->m_skinchanger.AddKillIconReplacement(
		CXorString("|eÏ§rTÒ").ToCharArray(),
		CXorString("|eÏ§rTÁ∑c‡∞qg¸").ToCharArray()
	);

	// Robins Knifes (CT -> Karambit Fade Stattrak)
	this->m_skinchanger.AddSkinReplacement(
		WEAPON_KNIFE,
		new CSkinMetadata(
			WEAPON_KNIFE_KARAMBIT,
			38,
			0,
			420,
			3
		)
	);
	this->m_skinchanger.AddModelReplacement(
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄¶rm‰∑{⁄°c%Ë¶{").ToCharArray(),
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄©vy‰Ø9f·Æ").ToCharArray()
	);
	this->m_skinchanger.AddKillIconReplacement(
		CXorString("|eÏ§rT·ßqjÆcTÊ∂").ToCharArray(),
		CXorString("|eÏ§rTÓ£ejË†~").ToCharArray()
	);
}

void CApplication::Unhook()
{
	// Proxy functiions
	m_pNetVarLowerBodyYaw->UnhookProxy();
	m_pNetVarSequence->UnhookProxy();

	// Reverse order, in case of any dependencies
	this->m_pEngineSoundHook->Restore();
	this->m_pViewRenderHook->Restore();
	this->m_pGameEventManagerHook->Restore();
	this->m_pSurfaceHook->Restore();
	this->m_pPanelHook->Restore();
	this->m_pClientHook->Restore();
	this->m_pModelRenderHook->Restore();
	this->m_pClientModeHook->Restore();

	this->m_bIsHooked = false;
}

void CApplication::Rehook()
{
	this->m_pClientModeHook->Rehook();
	this->m_pModelRenderHook->Rehook();
	this->m_pClientHook->Rehook();
	this->m_pPanelHook->Rehook();
	this->m_pSurfaceHook->Rehook();
	this->m_pGameEventManagerHook->Rehook();
	this->m_pViewRenderHook->Rehook();
	this->m_pEngineSoundHook->Rehook();

	m_pSequenceProxy = m_pNetVarSequence->HookProxy(CApplication::hk_SetViewModelSequence);
	m_pLowerBodyYawProxy = m_pNetVarLowerBodyYaw->HookProxy(CApplication::hk_SetLowerBodyYawTarget);

	this->m_bIsHooked = true;
}

IClientEntity* CApplication::GetLocalPlayer(bool bGetTargetIfLocalDead)
{
	IClientEntity* pEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());
	if (bGetTargetIfLocalDead && !pEntity->IsAlive())
		pEntity = pEntity->GetObserverTarget();

	return pEntity;
}


int tickcount = 0;
bool __fastcall CApplication::hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = m_pCreateMove(ecx, fInputSampleTime, pUserCmd);

	CApplication* pApp = CApplication::Instance();
	tickcount = pUserCmd->tick_count;

	// Instantly return
	if (!pUserCmd || !pUserCmd->command_number)
		return false;

	int iOldSeed = pUserCmd->random_seed;
	pUserCmd->random_seed = MD5_PseudoRandom(pUserCmd->command_number) & 0x7FFFFFFF;

	if (!pApp->m_bGotSendPackets)
	{
		uintptr_t* fp;
		__asm mov fp, ebp;
		pApp->m_bSendPackets = (bool*)(*fp - 0x1C);
		pApp->m_bGotSendPackets = true;
	}

	if (pApp->EngineClient()->IsInGame())
	{
		IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

		if (pLocalEntity->IsAlive())
		{
			//todo: ghetto !
			static int bNoFlashTriggered = 0;
			if (bNoFlashTriggered < 2)
			{
				if (bNoFlashTriggered == 0 ||
					*(float*)((DWORD)pLocalEntity + Offsets::m_flFlashMaxAlpha) != (255.0f - (255.0f * (1.0f - (pApp->Visuals()->GetFlashPercentage() / 100.0f)))))
				{
					pApp->Visuals()->NoFlash(pApp->Visuals()->GetFlashPercentage());
					bNoFlashTriggered++;
				}
			}

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

			// New tick, so we didn't get any targets yet
			pApp->m_targetSelector.SetHasTargets(false);

			// Update Aimbot & AutoRevolver
			pApp->Ragebot()->Update((void*)&createMoveParam);
			pApp->Ragebot()->AutoRevolver(pUserCmd);

			// Update Legitbot
			pApp->Legitbot()->Update((void*)&createMoveParam);

			// Update Triggerbot
			pApp->Triggerbot()->Update((void*)&createMoveParam);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update AntiAim
			pApp->AntiAim()->Update(pUserCmd);

			// Miscs
			pApp->Misc()->AutoPistol(pUserCmd);
			pApp->Misc()->NoRecoil(pUserCmd);
			pApp->Misc()->Fakelag(pUserCmd);
			pApp->Misc()->AutoStrafe(pUserCmd);
			pApp->Misc()->CircleStrafe(pLocalEntity, pUserCmd);
			pApp->Misc()->JumpScout(pUserCmd);

			// Visuals
			pApp->Visuals()->Nightmode();

			if (pUserCmd->buttons & IN_ATTACK)
			{
				CTarget* pTarget = pApp->TargetSelector()->GetTarget(pApp->Ragebot()->GetTargetCriteria());
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

			if (!*pApp->m_bSendPackets && pApp->AntiAim()->IsFakeYaw() ||
				*pApp->m_bSendPackets && !pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bLbyUpdate ||
				!pApp->AntiAim()->GetEnabled() ||
				(pUserCmd->buttons & IN_ATTACK) && !pApp->Ragebot()->DoingAutoRevolver() ||
				!bIsSniper && pUserCmd->buttons & IN_ATTACK2)
			{
				pApp->m_qLastTickAngles.x = pUserCmd->viewangles[0];
				pApp->m_qLastTickAngles.y = pUserCmd->viewangles[1];
				pApp->m_qLastTickAngles.z = pUserCmd->viewangles[2];
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

void __fastcall CApplication::hk_FrameStageNotify(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (pApp->EngineClient()->IsInGame() && pLocalEntity->IsAlive())
		{
			if (pApp->Resolver()->GetResolverType() != RESOLVERTYPE_NONE)
			{
				// Setting LowerBodyYaw
				for (int i = 1; i < pApp->EngineClient()->GetMaxClients(); i++)
				{
					IClientEntity* pCurEntity = pApp->EntityList()->GetClientEntity(i);

					if (!pCurEntity)
						continue;

					if (pCurEntity->IsDormant())
						continue;

					if (!(pCurEntity->GetFlags() & FL_CLIENT))
						continue;

					if (pCurEntity->GetTeamNum() == pLocalEntity->GetTeamNum()) // same team dont need to resolve
						continue;

					pCurEntity->GetAngEyeAngles()->y = pCurEntity->GetLowerBodyYaw();
				}
			}

			pApp->LagCompensation()->Update((void*)tickcount);

			pApp->Resolver()->Update();

			pApp->SkinChanger()->Update(pLocalEntity);
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
		}

		// Menu input handling
		pApp->m_pMenu->HandleInput();
	}

	m_pFrameStageNotify(ecx, curStage);
}

void __fastcall CApplication::hk_OverrideView(void* ecx, void* edx, CViewSetup* pViewSetup)
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
	return m_pOverrideView(ecx, pViewSetup);
}

void __fastcall CApplication::hk_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	CApplication* pApp = CApplication::Instance();

	IMaterial* pHands = NULL;
	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);

		pApp->Chams()->DrawFakeAngle(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pApp->Chams()->Render(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pHands = pApp->Visuals()->HandsDrawStyle(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}

	// Call original func
	m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
	// This is necessary for stuff to work properly
	pApp->ModelRender()->ForcedMaterialOverride(NULL);
}

void __fastcall CApplication::hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	CApplication* pApp = CApplication::Instance();
	ISurface* pSurface = pApp->Surface();

	static unsigned int vguiMatSystemTopPanel;
	if (vguiMatSystemTopPanel == NULL)
	{
		static CXorString matSystemTopPanel("ZjÒënxÒßz_Í≤GjÎß{");
		const char* szName = pApp->Panel()->GetName(vguiPanel);
		if (stricmp(szName, matSystemTopPanel.ToCharArray()) == 0)
		{
			vguiMatSystemTopPanel = vguiPanel;
		}
	}

	if (pApp->EngineClient()->IsInGame())
	{
		if (pApp->Visuals()->NoScope(vguiPanel))
			return;

		if (vguiMatSystemTopPanel == vguiPanel)
		{

			pApp->Gui()->GetWorldToScreenMatrix();

			// Draw NoScope
			pApp->Visuals()->DrawNoScope();

			// Draw SpecList
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Update((void*)pSurface);

			// Draw Weapon Esp
			pApp->WeaponEsp()->Update();

			// Draw Sound Esp
			pApp->SoundEsp()->Update();

			// Draw rear mirror
			pApp->Mirror()->Render(pSurface, pApp->Menu()->GetMirrorWindow());

			// Draw Hitmarker
			pApp->Visuals()->DrawHitmarker(pSurface);

			// Draw SpreadCone
			pApp->Visuals()->DrawSpreadCone();

			// Draw Crosshair last (but not least)
			pApp->Visuals()->DrawCrosshair();

			// LBY Indicator
			pApp->AntiAim()->DrawLBYIndicator();

			// LC Draw
			pApp->LagCompensation()->DrawLagCompensationEntries();
			pApp->LagCompensation()->DrawLagCompensationIndicator();

			// ****TEST*****
			/*pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);
			pApp->Surface()->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_NORMAL));
			IClientEntity* pLocal = pApp->GetLocalPlayer();
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				IClientEntity* pCurEnt = pApp->EntityList()->GetClientEntity(i);
				CResolverPlayer* pCurRes = pApp->Resolver()->GetResolverPlayer(i);

				if (!pCurEnt)
					continue;

				if (pCurEnt->IsDormant())
					continue;

				if (!pCurEnt->IsAlive())
					continue;

				if (!pCurRes)
					continue;

				Vector screen, origin = *pCurEnt->GetOrigin();
				if (pApp->Gui()->WorldToScreen(origin, screen))
				{
					if (pCurRes->m_bHasFakeActive)
					{
						pApp->Surface()->DrawSetTextPos(screen.x - 3, screen.y + 10);
						pApp->Surface()->DrawPrintText(L"FAKE", lstrlenW(L"FAKE"));
					}

					if (pCurRes->m_bStartPredLbyBreaks)
					{
						pApp->Surface()->DrawSetTextPos(screen.x - 8, screen.y + 25);
						pApp->Surface()->DrawPrintText(L"LBY-Breaker", lstrlenW(L"LBY-Breaker"));
					}
				}
			}*/
		}
	}

	if (vguiMatSystemTopPanel == vguiPanel)
	{
		// Draw Menu least ;)
		pApp->Menu()->Draw(pSurface);
	}

	m_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}

void __fastcall CApplication::hk_PlaySound(void* ecx, void* edx, const char* fileName)
{
	CApplication* pApp = CApplication::Instance();
	pApp->Misc()->AutoAccept(fileName);

	pApp->m_pPlaySound(ecx, fileName);
}

float __fastcall CApplication::hk_GetViewModelFov(void* ecx, void* edx)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->Visuals()->GetViewmodelFov())
	{
		return pApp->Visuals()->GetViewmodelFovValue();
	}
	return m_pGetViewModelFov(ecx);
}

bool __fastcall CApplication::hk_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();
	pApp->m_skinchanger.ApplyCustomKillIcon(pEvent);

	return m_pFireEventClientSide(ecx, pEvent);
}

void __fastcall CApplication::hk_RenderView(void* ecx, void* edx, const CViewSetup& view, CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw)
{
	CApplication* pApp = CApplication::Instance();
	pApp->m_mirror.OnRenderView(ecx, view, hudViewSetup, nClearFlags, whatToDraw);

	m_pRenderViewFn(ecx, view, hudViewSetup, nClearFlags, whatToDraw);
}

void __fastcall CApplication::hk_RenderSmokeOverlay(void* ecx, void* edx, bool bUnknown)
{
	CApplication* pApp = CApplication::Instance();

	// Idk what this is used for,
	// more or less pasta from AimTux :D
	if (pApp->Visuals()->GetNoSmoke())
		return;

	pApp->m_pRenderSmokeOverlay(ecx, bUnknown);
}

int __fastcall CApplication::hk_EmitSound1(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, CUtlVector<Vector>* pUtlVecOrigins,
	bool bUpdatePositions, float soundtime, int speakerentity)
{
	CApplication* pApp = CApplication::Instance();

	if (pOrigin)
		pApp->SoundEsp()->AddSound(new CSoundInfo(iEntIndex, *pOrigin, pSample));

	return pApp->m_pEmitSound1(ecx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, iSoundlevel, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}

int __fastcall CApplication::hk_EmitSound2(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, CUtlVector<Vector>* pUtlVecOrigins,
	bool bUpdatePositions, float soundtime, int speakerentity)
{
	CApplication* pApp = CApplication::Instance();

	if (pOrigin)
		pApp->SoundEsp()->AddSound(new CSoundInfo(iEntIndex, *pOrigin, pSample));

	return pApp->m_pEmitSound2(ecx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}

void __cdecl CApplication::hk_SetViewModelSequence(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	// Make the incoming data editable.
	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;

	// Confirm that we are replacing our view model and not someone elses.
	CBaseViewModel* pViewModel = (CBaseViewModel*)pStruct;

	if (pViewModel) {
		IClientEntity* pOwner = pApp->EntityList()->GetClientEntityFromHandle(pViewModel->GetOwner());

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->EntIndex() == pApp->EngineClient()->GetLocalPlayer()) {
			// Get the filename of the current view model.
			const model_t* pModel = pApp->ModelInfo()->GetModel(pViewModel->GetModelIndex());
			const char* szModel = pApp->ModelInfo()->GetModelName(pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;

			if (!strcmp(szModel, CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄†bÒßemÈª9f·Æ").ToCharArray())) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomIntDef(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomIntDef(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(szModel, CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄§vgÊ™~dÎùvoÛ£yh‡¶9f·Æ").ToCharArray())) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomIntDef(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomIntDef(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(szModel, CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄≤bxÌÏzoÈ").ToCharArray())) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomIntDef(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomIntDef(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(szModel, CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄±byÛ´ajÈùudÚ´r%Ë¶{").ToCharArray())) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}

			// Set the fixed sequence.
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	pApp->m_pSequenceProxy(pData, pStruct, pOut);
}

void __cdecl CApplication::hk_SetLowerBodyYawTarget(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;
	IClientEntity* pEntity = (IClientEntity*)pStruct;
	IClientEntity* pLocal = pApp->GetLocalPlayer();
	if (pData && pEntity && pLocal)
	{
		CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());

		if (fabsf(pResolverPlayer->GetLbyProxyLastValue() - pData->m_Value.m_Float) >= 30.0f)
		{
			pResolverPlayer->SetLbyProxyLastValue(pData->m_Value.m_Float);
			pResolverPlayer->SetLbyUpdateTime(pApp->GlobalVars()->curtime);
			pResolverPlayer->SetLbyProxyUpdatedTime(pApp->GlobalVars()->curtime);
		}
	}

	pApp->m_pLowerBodyYawProxy(pDataConst, pStruct, pOut);
}

void CApplication::Setup()
{
#ifdef _DEBUG
	// Setup console
	g_pConsole = new CConsole();
	g_pConsole->WritePlain(".mechanics\n > Build date: %s\n > Build time: %s\n\n", __DATE__, __TIME__);
#endif

	// Grab info about our own dll path etc
	this->GetModuleInfo();

	// Setup strings
	CXorString xorCreateInterface("Ty‡£cnÃ¨cn˜§vh‡");

	// Grab engine addresses
	this->m_dwClientDll = (DWORD)GetModuleHandle(/*client.dll*/CXorString("tgÏßy´¶{g").ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(/*engine.dll*/CXorString("re‚´yn´¶{g").ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(/*materialsystem.dll*/CXorString("zjÒßeb‰Ædrˆ∂rf´¶{g").ToCharArray());
	this->m_dwVGui2Dll = (DWORD)GetModuleHandle(/*vgui2.dll*/CXorString("al´%%·Æ{").ToCharArray());
	this->m_dwVGuiSurfaceDll = (DWORD)GetModuleHandle(/*vguimatsurface.dll*/CXorString("al´zjÒ±by„£tn´¶{g").ToCharArray());
	this->m_dwVPhysicsDll = (DWORD)GetModuleHandle(/*vphysics.dll*/CXorString("a{ÌªdbÊ±9oÈÆ").ToCharArray());
	this->m_dwVStdLibDll = (DWORD)GetModuleHandle(/*vstdlib.dll*/CXorString("axÒ¶{bÁÏsgÈ").ToCharArray());
	CreateInterfaceFn ClientFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn EngineFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn MaterialSystemFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VGui2Factory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGui2Dll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VGuiSurfaceFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGuiSurfaceDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VPhysicsFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVPhysicsDll, xorCreateInterface.ToCharArray());
	CreateInterfaceFn VStdLibFactory = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVStdLibDll, xorCreateInterface.ToCharArray());

	m_pRandomSeed = (RandomSeed_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomSeed*/CXorString("EjÎ¶xf÷ßro").ToCharArray());
	m_pRandomInt = (RandomInt_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomInt*/CXorString("EjÎ¶xfÃ¨c").ToCharArray());
	m_pRandomFloat = (RandomFloat_t)GetProcAddress((HMODULE)this->m_dwVStdLibDll, /*RandomFloat*/CXorString("EjÎ¶xf√ÆxjÒ").ToCharArray());

	m_pEngineClient = (IVEngineClient*)EngineFactory(/*VEngineClient014*/CXorString("ANÎ•~e‡Å{b‡¨c;¥ˆ").ToCharArray(), NULL);
	m_pClient = (IBaseClientDLL*)ClientFactory(/*VClient018*/CXorString("AHÈ´reÒÚ&3").ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)ClientFactory(/*VClientEntityList003*/CXorString("AHÈ´reÒáyÏ∂nGÏ±c;µÒ").ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)EngineFactory(/*VModelInfoClient004*/CXorString("AFÍ¶rgÃ¨qd∆Æ~nÎ∂';±").ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)EngineFactory(/*VEngineModel016*/CXorString("ANÎ•~e‡èxo‡Æ':≥").ToCharArray(), NULL);
	m_pRenderView = (IVRenderView*)EngineFactory(/*VEngineRenderView014*/CXorString("ANÎ•~e‡êre·ße]Ïß`;¥ˆ").ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)EngineFactory(/*EngineTraceClient004*/CXorString("Re‚´yn—∞vh‡Å{b‡¨c;µˆ").ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)MaterialSystemFactory(/*VMaterialSystem080*/CXorString("AF‰∂ryÏ£{X¸±cnËÚ/;").ToCharArray(), NULL);
	m_pCVar = (ICVar*)VStdLibFactory(/*VEngineCvar007*/CXorString("ANÎ•~e‡Åaj˜Ú'<").ToCharArray(), NULL);
	m_pPanel = (IPanel*)VGui2Factory(/*VGUI_Panel009*/CXorString("AL–ãH[‰¨rgµÚ.").ToCharArray(), NULL);
	m_pSurface = (ISurface*)VGuiSurfaceFactory(/*VGUI_Surface031*/CXorString("AL–ãHX∞qjÊß'8¥").ToCharArray(), NULL);
	m_pGameEventManager = (IGameEventManager2*)EngineFactory(/*GAMEEVENTSMANAGER002*/CXorString("PJ»áR]¿åCX»ÉYJ¬áE;µ").ToCharArray(), NULL);
	m_pPhysicsSurfaceProps = (IPhysicsSurfaceProps*)VPhysicsFactory(/*VPhysicsSurfaceProps001*/CXorString("A[ÌªdbÊ±D~˜§vh‡íedı±';¥").ToCharArray(), NULL);
	m_pEngineSound = (IEngineSound*)EngineFactory(/*IEngineSoundClient003*/CXorString("^NÎ•~e‡ëx~Î¶TgÏßyµÚ$").ToCharArray(), NULL); // TODO

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS);

	// KeyValues::Init
	DWORD dwInitKeyValuesTemp = (CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56",
		"a----bbb----ccccccc-----ddd"
	) + 0x07);
	m_pInitKeyValues = (InitKeyValues_t)(dwInitKeyValuesTemp + (*(DWORD_PTR*)(dwInitKeyValuesTemp + 1)) + 0x05);

	// KeyValues::LoadFromBuffer
	DWORD dwLoadFromBufferTemp = CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89\x4C\x24\x04",
		"abcdefghijjjjjjjq"
	);
	m_pLoadFromBuffer = (LoadFromBuffer_t)dwLoadFromBufferTemp;

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "client.dll\t\t=>\t0x%08X\n", m_dwClientDll);
	g_pConsole->Write(LOGLEVEL_INFO, "engine.dll\t\t=>\t0x%08X\n", m_dwEngineDll);
	g_pConsole->Write(LOGLEVEL_INFO, "materialsystem.dll\t=>\t0x%08X\n", m_dwMaterialSystemDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vgui2.dll\t\t=>\t0x%08X\n", m_dwVGui2Dll);
	g_pConsole->Write(LOGLEVEL_INFO, "vguimatsurface.dll\t=>\t0x%08X\n", m_dwVGuiSurfaceDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vphysics.dll\t\t=>\t0x%08X\n", m_dwVPhysicsDll);
	g_pConsole->Write(LOGLEVEL_INFO, "vstdlib.dll\t\t=>\t0x%08X\n", m_dwVStdLibDll);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "VEngineClient014\t\t=>\t0x%08X\n", m_pEngineClient);
	g_pConsole->Write(LOGLEVEL_INFO, "VClient018\t\t=>\t0x%08X\n", m_pClient);
	g_pConsole->Write(LOGLEVEL_INFO, "VClientEntityList003\t=>\t0x%08X\n", m_pEntityList);
	g_pConsole->Write(LOGLEVEL_INFO, "VModelInfoClient004\t=>\t0x%08X\n", m_pModelInfo);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineModel016\t\t=>\t0x%08X\n", m_pModelRender);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineRenderView014\t=>\t0x%08X\n", m_pRenderView);
	g_pConsole->Write(LOGLEVEL_INFO, "EngineTraceClient004\t=>\t0x%08X\n", m_pEngineTrace);
	g_pConsole->Write(LOGLEVEL_INFO, "VMaterialSystem080\t=>\t0x%08X\n", m_pMaterialSystem);
	g_pConsole->Write(LOGLEVEL_INFO, "VEngineCvar007\t\t=>\t0x%08X\n", m_pCVar);
	g_pConsole->Write(LOGLEVEL_INFO, "VGUI_Panel009\t\t=>\t0x%08X\n", m_pPanel);
	g_pConsole->Write(LOGLEVEL_INFO, "VGUI_Surface031\t\t=>\t0x%08X\n", m_pSurface);
	g_pConsole->Write(LOGLEVEL_INFO, "GAMEEVENTSMANAGER002\t=>\t0x%08X\n", m_pGameEventManager);
	g_pConsole->Write(LOGLEVEL_INFO, "VPhysicsSurfaceProps001\t=>\t0x%08X\n", m_pPhysicsSurfaceProps);
	g_pConsole->Write(LOGLEVEL_INFO, "IEngineSoundClient003\t=>\t0x%08X\n", m_pEngineSound);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "GlobalVars\t\t=>\t0x%08X\n", m_pGlobalVars);
	g_pConsole->WritePlain("\n");

	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::Init\t\t=>\t0x%08X\n", m_pInitKeyValues);
	g_pConsole->Write(LOGLEVEL_INFO, "KeyValues::LoadFromBuffer\=>\t0x%08X\n", m_pLoadFromBuffer);
	g_pConsole->WritePlain("\n");
#endif

	if (m_pEngineClient->IsInGame())
	{
		SetRecoilCompensation(atof(m_pCVar->FindVar(/*weapon_recoil_scale*/CXorString("`n‰≤xe⁄∞rhÍ´{Tˆ°vg‡").ToCharArray())->value));
	}
	m_visuals.SetZoomSensitivity(atof(m_pCVar->FindVar(CXorString("mdÍØHx‡¨dbÒ´abÒªHy‰∂~d⁄Øx~ˆß").ToCharArray())->value));

	// Resource manager
	g_pResourceManager = new CResourceManager();
	g_pResourceManager->Init(this);
	m_pResourceManager = g_pResourceManager;

	// Create Resources
	m_pResourceManager->CreateTextures();
	m_pResourceManager->CreateFonts();

	// NetVar Dumps
	//FILE* pFile = fopen("C:\\Users\\Robin\\Desktop\\dump.txt", "w");
	//if (pFile)
	//{
	//	g_pConsole->Write(LOGLEVEL_INFO, "Dumping NetVars ... ");
	//	//CNetVarManager::DumpAll(pFile, m_pClient->GetAllClasses());
	//	//CNetVarManager::DumpClientClasses(pFile, m_pClient->GetAllClasses());
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_CollisionProperty");
	//	CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_CSPlayer");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BasePlayer");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseAnimating");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseCombatWeapon");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseViewModel");

	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_AttributeContainer");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_ScriptCreatedItem");
	//	//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseAttributableItem");

	//	fclose(pFile);
	//	g_pConsole->WritePlain("Done!\n");
	//}

	CXorString xorBaseEntity("S_⁄Ävx‡áyÏ∂n"); // DT_BaseEntity
	CXorString xorBasePlayer("S_⁄Ävx‡í{j¸ße"); // DT_BasePlayer
	CXorString xorCollisionProperty("S_⁄ÅxgÈ´dbÍ¨GyÍ≤ryÒª");
	CXorString xorCSPlayer("S_⁄ÅD[È£nn˜"); // DT_CSPlayer
	CXorString xorLocalPlayerExclusive("S_⁄éxh‰ÆGg‰ªry¿∫tg±~}‡"); // DT_LocalPlayerExclusive
	CXorString xorBaseCombatWeapon("S_⁄Ävx‡ÅxfÁ£c\\‡£gdÎ"); // DT_BaseCombatWeapon
	CXorString xorWeaponCSBase("S_⁄ïrjı≠yH÷Ävx‡"); // DT_WeaponCSBase
	CXorString xorBaseCSGrenade("S_⁄Ävx‡ÅDL˜ßyj·ß"); // DT_BaseCSGrenade
	CXorString xorBaseCombatCharacter("S_⁄Ävx‡ÅxfÁ£cHÌ£ejÊ∂ry"); // DT_BaseCombatCharacter
	CXorString xorBaseViewModel("S_⁄Ävx‡î~nÚèxo‡Æ"); // DT_BaseViewModel
	CXorString xorPlantedC4("S_⁄í{jÎ∂ro∆ˆ");
	CXorString xorBaseAttributableItem("S_⁄Ävx‡Éc˜´u~Ò£ug‡ãcnË"); // DT_BaseAttributableItem
	CXorString xorBaseAnimating("S_⁄Ävx‡ÉybË£cbÎ•"); // DT_BaseAnimating

	m_pNetVarMgr = new CNetVarManager();
	m_pNetVarMgr->AddTable(xorBaseEntity.ToCharArray());
	m_pNetVarMgr->AddTable(xorBasePlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorCSPlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatWeapon.ToCharArray());
	m_pNetVarMgr->AddTable(xorWeaponCSBase.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCSGrenade.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatCharacter.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseViewModel.ToCharArray());
	m_pNetVarMgr->AddTable(xorPlantedC4.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseAttributableItem.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseAnimating.ToCharArray());

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Loading NetVars... ");
#endif

	m_pNetVarMgr->LoadTables(m_pClient->GetAllClasses(), true);

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n");
	g_pConsole->Write(LOGLEVEL_INFO, "Loading Offsets... ");
#endif

	Offsets::m_angRotation = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_angRotation*/CXorString("zT‰¨pYÍ∂vÏ≠y").ToCharArray());
	Offsets::m_nModelIndex = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_nModelIndex*/CXorString("zTÎèxo‡Æ^e·ßo").ToCharArray());
	Offsets::m_hMyWeapons = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter.ToCharArray(), /*m_hMyWeapons*/CXorString("zTÌèn\\‡£gdÎ±").ToCharArray());
	Offsets::m_hViewModel = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_hViewModel[0]*/CXorString("zTÌî~nÚèxo‡ÆL;ÿ").ToCharArray());
	Offsets::m_flSimulationTime = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_flSimulationTime*/CXorString("zT„ÆDbË∑{jÒ´xe—´zn").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_vecMins = m_pNetVarMgr->GetOffset(2, xorBaseEntity.ToCharArray(), xorCollisionProperty.ToCharArray(), /*m_vecMins*/CXorString("zTÛßtFÏ¨d").ToCharArray());
	Offsets::m_vecMaxs = m_pNetVarMgr->GetOffset(2, xorBaseEntity.ToCharArray(), xorCollisionProperty.ToCharArray(), /*m_vecMaxs*/CXorString("zTÛßtF‰∫d").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_vecOrigin = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_vecOrigin*/CXorString("zTÛßtD˜´pbÎ").ToCharArray());
	Offsets::m_vecViewOffset = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_vecViewOffset[0]*/CXorString("zTÛßt]Ïß`D„§dnÒô'V").ToCharArray());
	Offsets::m_angEyeAngles = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_angEyeAngles*/CXorString("zT‰¨pN¸ßVe‚Ærx").ToCharArray());
	m_pNetVarLowerBodyYaw = m_pNetVarMgr->GetNetVar(xorCSPlayer.ToCharArray(), /*m_flLowerBodyYawTarget*/CXorString("zT„Æ[dÚßeIÍ¶nR‰µCj˜•r").ToCharArray());
	Offsets::m_flLowerBodyYawTarget = m_pNetVarLowerBodyYaw->GetOffset();
	Offsets::m_vecVelocity = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_vecVelocity[0]*/CXorString("zTÛßt]‡ÆxhÏ∂nPµü").ToCharArray());
	Offsets::m_iTeamNum = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_iTeamNum*/CXorString("zTÏñrjËåbf").ToCharArray());
	Offsets::m_lifeState = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_lifeState*/CXorString("zTÈ´qn÷∂v‡").ToCharArray());
	Offsets::m_iHealth = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_iHealth*/CXorString("zTÏärjÈ∂").ToCharArray());
	Offsets::m_ArmorValue = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_ArmorValue*/CXorString("zTƒ∞zd˜îvgß").ToCharArray());
	Offsets::m_bHasHelmet = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bHasHelmet*/CXorString("zTÁävxÕß{f‡∂").ToCharArray());
	Offsets::m_fFlags = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_fFlags*/CXorString("zT„Ñ{j‚±").ToCharArray());
	Offsets::m_bSpotted = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_bSpotted*/CXorString("zTÁëgdÒ∂ro").ToCharArray());
	Offsets::m_bGunGameImmunity = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bGunGameImmunity*/CXorString("zTÁÖbe¬£znÃØz~Î´cr").ToCharArray());
	Offsets::m_flFlashMaxAlpha = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_flFlashMaxAlpha*/CXorString("zT„ÆQg‰±F‰∫Vgı™v").ToCharArray());
	//Offsets::m_bMoveType = m_pNetVarMgr->GetOffset("DT_BaseEntity", "m_bMoveType"); <= Not existant :/
	Offsets::m_bIsScoped = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bIsScoped*/CXorString("zTÁãdXÊ≠gn·").ToCharArray());
	Offsets::m_iShotsFired = m_pNetVarMgr->GetOffset(2, xorCSPlayer.ToCharArray(),
		/*DT_CSLocalPlayerExclusive*/CXorString("S_⁄ÅDGÍ°vg’Ævr‡∞RsÊÆbxÏ¥r").ToCharArray(),
		/*m_iShotsFired*/CXorString("zTÏëdÒ±Qb˜ßs").ToCharArray());
	Offsets::m_hActiveWeapon = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter.ToCharArray(), /*m_hActiveWeapon*/CXorString("zTÌÉtÏ¥r\\‡£gdÎ").ToCharArray());
	Offsets::m_hObserverTarget = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_hObserverTarget*/CXorString("zTÌçux‡∞an˜ñvy‚ßc").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::deadflag = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(),
		/*DT_PlayerState*/CXorString("S_⁄í{j¸ßeXÒ£cn").ToCharArray(),
		/*deadflag*/CXorString("sn‰¶qg‰•").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_nTickBase = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_nTickBase*/CXorString("zTÎñ~hÓÄvx‡").ToCharArray());
	Offsets::m_bIsDefusing = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_bIsDefusing*/CXorString("zTÁãdO‡§bxÏ¨p").ToCharArray());
	Offsets::m_flC4Blow = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_flC4Blow*/CXorString("zT„ÆT?«Æx|").ToCharArray());
	Offsets::m_flDefuseCountDown = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_flDefuseCountDown*/CXorString("zT„ÆSn„∑dn∆≠beÒÜx|Î").ToCharArray());
	Offsets::m_hBombDefuser = m_pNetVarMgr->GetOffset(xorPlantedC4.ToCharArray(), /*m_hBombDefuser*/CXorString("zTÌÄxfÁÜrm±ry").ToCharArray());

	CNetVar* pDtLocal = m_pNetVarMgr->GetNetVar(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*DT_Local*/CXorString("S_⁄éxh‰Æ").ToCharArray());
	Offsets::m_nJumpTimeMsecs = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_nJumpTimeMsecs*/CXorString("zTÎàbfıñ~f‡èdnÊ±").ToCharArray())->GetOffset();
	Offsets::m_flFallVelocity = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_flFallVelocity*/CXorString("zT„ÆQjÈÆAnÈ≠tbÒª").ToCharArray())->GetOffset();
	Offsets::m_viewPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_viewPunchAngle*/CXorString("zTÛ´r|’∑yhÌÉylÈß").ToCharArray())->GetOffset();
	Offsets::m_aimPunchAngle = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngle*/CXorString("zT‰´z[¨tcƒ¨pg‡").ToCharArray())->GetOffset();
	Offsets::m_aimPunchAngleVel = pDtLocal->GetOffset() + pDtLocal->GetChild(/*m_aimPunchAngleVel*/CXorString("zT‰´z[¨tcƒ¨pg‡îrg").ToCharArray())->GetOffset();

	m_pNetVarMgr->SetSummarizeOffsets(true);
	Offsets::m_iItemDefinitionIndex = m_pNetVarMgr->GetOffset(4, xorBaseCombatWeapon.ToCharArray(),
		/*DT_EconEntity*/CXorString("S_⁄átdÎáyÏ∂n").ToCharArray(),
		/*DT_AttributeContainer*/CXorString("S_⁄Éc˜´u~ÒßTdÎ∂vbÎße").ToCharArray(),
		/*DT_ScriptCreatedItem*/CXorString("S_⁄ëtyÏ≤cH˜ßv‡¶^‡Ø").ToCharArray(),
		/*m_iItemDefinitionIndex*/CXorString("zTÏãcnËÜrmÏ¨~Ï≠yBÎ¶rs").ToCharArray());
	m_pNetVarMgr->SetSummarizeOffsets(false);
	Offsets::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_hOwner*/CXorString("zTÌç`e‡∞").ToCharArray());
	Offsets::m_iClip1 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip1*/CXorString("zTÏÅ{bıÛ").ToCharArray());
	Offsets::m_iClip2 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip2*/CXorString("zTÏÅ{bı").ToCharArray());
	Offsets::m_flNextPrimaryAttack = m_pNetVarMgr->GetOffset(2, xorBaseCombatWeapon.ToCharArray(),
		/*DT_LocalActiveWeaponData*/CXorString("S_⁄éxh‰ÆVhÒ´an“ßv{Í¨SjÒ£").ToCharArray(),
		/*m_flNextPrimaryAttack*/CXorString("zT„ÆYn˝∂GyÏØvy¸Éc‰°|").ToCharArray());

	Offsets::m_fAccuracyPenalty = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_fAccuracyPenalty*/CXorString("zT„Éth∞vh¸íre‰Æcr").ToCharArray());
	Offsets::m_flPostponeFireReadyTime = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_flPostponeFireReadyTime*/CXorString("zT„ÆGdˆ∂gdÎßQb˜ßEn‰¶n_ÏØr").ToCharArray());

	Offsets::m_fThrowTime = m_pNetVarMgr->GetOffset(xorBaseCSGrenade.ToCharArray(), /*m_fThrowTime*/CXorString("zT„ñyÍµCbËß").ToCharArray());

	CNetVar* pDtAttributeManager = m_pNetVarMgr->GetNetVar(xorBaseAttributableItem.ToCharArray(), /*DT_AttributeContainer*/CXorString("S_⁄Éc˜´u~ÒßTdÎ∂vbÎße").ToCharArray());
	CNetVar* pDtItem = pDtAttributeManager->GetChild(/*DT_ScriptCreatedItem*/CXorString("S_⁄ëtyÏ≤cH˜ßv‡¶^‡Ø").ToCharArray());
	int iOffset = pDtAttributeManager->GetOffset() + pDtItem->GetOffset();

	Offsets::m_iItemIDHigh = iOffset + pDtItem->GetChild(/*m_iItemIDHigh*/CXorString("zTÏãcnËãSCÏ•").ToCharArray())->GetOffset();
	Offsets::m_iItemIDLow = iOffset + pDtItem->GetChild(/*m_iItemIDLow*/CXorString("zTÏãcnËãSGÍµ").ToCharArray())->GetOffset();
	Offsets::m_iAccountID = iOffset + pDtItem->GetChild(/*m_iAccountID*/CXorString("zTÏÉthÍ∑yÃÜ").ToCharArray())->GetOffset();
	Offsets::m_iEntityQuality = iOffset + pDtItem->GetChild(/*m_iEntityQuality*/CXorString("zTÏáyÏ∂nZ£{bÒª").ToCharArray())->GetOffset();
	Offsets::m_szCustomName = iOffset + pDtItem->GetChild(/*m_szCustomName*/CXorString("zTˆ∏T~ˆ∂xfÀ£zn").ToCharArray())->GetOffset();

	Offsets::m_OriginalOwnerXuidLow = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_OriginalOwnerXuidLow*/CXorString("zT ∞~lÏ¨vg µyn˜öbb·éx|").ToCharArray());
	Offsets::m_OriginalOwnerXuidHigh = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_OriginalOwnerXuidHigh*/CXorString("zT ∞~lÏ¨vg µyn˜öbb·ä~lÌ").ToCharArray());
	Offsets::m_nFallbackPaintKit = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackPaintKit*/CXorString("ì†ëπüììùüúîØüñëãµñã", 0xAFFEAFFE).ToCharArray());
	Offsets::m_nFallbackSeed = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackSeed*/CXorString("zTÎÑvgÈ†vhÓërn·").ToCharArray());
	Offsets::m_flFallbackWear = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_flFallbackWear*/CXorString("zT„ÆQjÈÆujÊ©@n‰∞").ToCharArray());
	Offsets::m_nFallbackStatTrak = m_pNetVarMgr->GetOffset(xorBaseAttributableItem.ToCharArray(), /*m_nFallbackStatTrak*/CXorString("zTÎÑvgÈ†vhÓëcjÒñejÓ").ToCharArray());

	Offsets::DT_BaseViewModel::m_hWeapon = m_pNetVarMgr->GetOffset(xorBaseViewModel.ToCharArray(), /*m_hWeapon*/CXorString("zTÌïrjı≠y").ToCharArray());
	Offsets::DT_BaseViewModel::m_hOwner = m_pNetVarMgr->GetOffset(xorBaseViewModel.ToCharArray(), /*m_hOwner*/CXorString("zTÌç`e‡∞").ToCharArray());

	Offsets::m_nSequence = m_pNetVarMgr->GetOffset(xorBaseAnimating.ToCharArray(), /*m_nSequence*/CXorString("zTÎërzßyh‡").ToCharArray());
	Offsets::m_flPoseParameter = m_pNetVarMgr->GetOffset(xorBaseAnimating.ToCharArray(), /*m_flPoseParameter*/CXorString("zT„ÆGdˆßGj˜£znÒße").ToCharArray());
	Offsets::m_flCycle = m_pNetVarMgr->GetOffset(2, xorBaseAnimating.ToCharArray(),
		/*DT_ServerAnimationData*/CXorString("S_⁄ëryÛßeJÎ´zjÒ´xe¡£cj").ToCharArray(),
		/*m_flCycle*/CXorString("zT„ÆTrÊÆr").ToCharArray());

#ifdef _DEBUG
	g_pConsole->WritePlain("Done\n\n");
#endif

	// Grab NetVars later required for hooking
	m_pNetVarSequence = m_pNetVarMgr->GetNetVar(xorBaseViewModel.ToCharArray(), /*m_nSequence*/CXorString("zTÎërzßyh‡").ToCharArray());

	m_misc.SetReadyCallback((IsReadyCallback_t)(CPattern::FindPattern(
		(BYTE*)this->ClientDll(),
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE",
		"ghdrtzuigkog----trg"
	)));

	CConfig config;
	config.Init(this);
	config.LoadFile("testconfig.cfg");

	// Target Selector
	this->m_targetSelector.Setup(this);
	this->m_targetSelector.SetMultipoint(false);
	this->m_targetSelector.SetVisibleMode(VISIBLEMODE_CANHIT);
	this->m_targetSelector.SetCheckHitbox(TARGET_HITBOX_HEAD, true);

	// Setups
	this->m_ragebot.Setup();
	this->m_legitbot.Setup();
	this->m_triggerbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_weaponesp.Setup();
	this->m_soundEsp.Setup();
	this->m_chams.Setup();
	this->m_misc.Setup();
	this->m_resolver.Setup();
	this->m_skinchanger.Setup();
	this->m_visuals.Setup();
	this->m_mirror.Setup();
	this->m_lagcompensation.Setup();

	// Aimbot
	this->m_ragebot.SetEnabled(false);
	this->m_ragebot.SetAutoshoot(true);
	this->m_ragebot.SetAutoscope(true);
	this->m_ragebot.SetSilentAim(true);
	this->m_ragebot.SetAutoReload(true);
	this->m_ragebot.SetAutoZeus(true);
	this->m_ragebot.SetTargetCriteria(TARGETCRITERIA_VIEWANGLES);
	this->m_ragebot.SetCalculateHitchance(true);
	this->m_ragebot.SetHitchance(25.0f);
	this->m_ragebot.SetNoSpread(false);
	this->m_ragebot.SetAutoRevolver(true);

	// Legitbot
	this->m_legitbot.SetEnabled(true);
	this->m_legitbot.SetTimeToAim(0.15f);

	// Triggerbot
	this->m_triggerbot.SetEnabled(false);
	this->m_triggerbot.SetShootDelay(50);
	this->m_triggerbot.SetShootDelayJitter(15);

	// Antiaim
	this->m_antiAim.SetEnabled(false);
	this->m_antiAim.SetDrawLbyIndicator(true);
	this->m_antiAim.SetDoEdgeAntiAim(true);
	this->m_antiAim.SetLbyBreaker(true);

	// Standing
	this->m_antiAim.SetPitchSettingStanding(PITCHANTIAIM_DOWN);
	this->m_antiAim.SetYawSettingStanding(YAWANTIAIM_STATIC);
	this->m_antiAim.SetYawOffsetStanding(-90);
	this->m_antiAim.SetYawFakeSettingStanding(FAKEYAWANTIAIM_STATIC);
	this->m_antiAim.SetYawFakeOffsetStanding(90);
	// Moving
	this->m_antiAim.SetPitchSettingMoving(PITCHANTIAIM_DOWN);
	this->m_antiAim.SetYawSettingMoving(YAWANTIAIM_BACKWARDS);
	this->m_antiAim.SetYawOffsetMoving(0);
	this->m_antiAim.SetYawFakeSettingMoving(FAKEYAWANTIAIM_STATIC);
	this->m_antiAim.SetYawFakeOffsetMoving(90);

	// Resolver
	this->m_resolver.SetEnabled(true);
	this->m_resolver.SetResolverType(RESOLVERTYPE_NONE);

	// Bhop
	this->m_bhop.SetEnabled(true);

	// Esp
	this->m_esp.SetEnabled(true);
	this->m_esp.SetFillBoundingBox(false);
	this->m_esp.SetDrawBoundingBox(true);
	this->m_esp.SetDrawNames(true);
	this->m_esp.SetDrawHealthBar(true);
	this->m_esp.SetDrawHealthNumber(true);
	this->m_esp.SetDrawArmorBar(false);
	this->m_esp.SetDrawActiveWeapon(true);
	this->m_esp.SetDrawAmmoBar(true);
	this->m_esp.SetDrawAmmoNumber(true);
	this->m_esp.SetDrawOwnTeam(false);
	this->m_esp.SetDrawOwnModel(false);
	this->m_esp.SetDrawOnlyVisible(false);
	this->m_esp.SetDrawOnlySpotted(false);
	this->m_esp.SetDrawOutline(true);
	this->m_esp.SetDrawViewangles(false);
	this->m_esp.SetViewanglesLength(45);
	this->m_esp.SetFadeoutEnabled(true);
	this->m_esp.SetFadeoutTime(1.0f);
	this->m_esp.SetColorCT(Color(0, 0, 255));
	this->m_esp.SetColorT(Color(255, 0, 0));
	this->m_esp.SetDrawSkeleton(true);
	//this->m_esp.SetColorSpotted();

	// WeaponEsp
	this->m_weaponesp.SetEnabled(true);
	this->m_weaponesp.SetDrawWeaponName(false);
	this->m_weaponesp.SetDrawWeaponBoundingBox(false);
	this->m_weaponesp.SetDrawGrenadeName(true);
	this->m_weaponesp.SetDrawGrenadeBoundingBox(false);
	this->m_weaponesp.SetDrawBombName(false);
	this->m_weaponesp.SetDrawBombBoundingBox(false);
	this->m_weaponesp.SetDrawBombTimer(true);
	this->m_weaponesp.SetDrawBombDefuseTimer(true);
	this->m_weaponesp.SetDrawBombDamageIndicator(false);

	// Sound Esp
	this->m_soundEsp.SetEnabled(false);
	this->m_soundEsp.SetShowTime(1.0f);
	this->m_soundEsp.SetFadeTime(1.0f);
	this->m_soundEsp.SetFadeoutEnabled(true);
	this->m_soundEsp.SetDrawOwnTeam(false);
	this->m_soundEsp.SetDrawVisible(false);

	// Chams
	this->m_chams.SetEnabled(true);
	this->m_chams.SetRenderTeam(false);
	this->m_chams.SetRenderLocalplayer(false);
	this->m_chams.SetOnlyVisible(false);
	this->m_chams.SetFlatModels(false);
	this->m_chams.SetRenderFakeAngle(true);
	this->m_chams.SetColorHiddenCT(Color(0, 0, 255));
	this->m_chams.SetColorVisibleCT(Color(0, 255, 0));
	this->m_chams.SetColorHiddenT(Color(255, 0, 0));
	this->m_chams.SetColorVisibleT(Color(255, 255, 0));

	// Misc
	this->m_misc.SetEnabled(true);
	this->m_misc.SetNoRecoil(false);
	this->m_misc.SetFakelag(false);
	this->m_misc.SetFakelagChokeAmount(10);
	this->m_misc.SetAutoStrafeMode(AUTOSTRAFEMODE_LEGIT);
	this->m_misc.SetCircleStrafe(true);
	this->m_misc.SetAutoPistol(false);
	this->m_misc.SetShowSpectators(false);
	this->m_misc.SetShowOnlyMySpectators(false);
	this->m_misc.SetShowOnlyMyTeamSpectators(false);
	this->m_misc.SetJumpScout(true);
	this->m_misc.SetNoName(false);
	this->m_misc.SetAutoAccept(false);
	this->m_misc.SetSpamName(false);

	// SkinChanger
	this->m_skinchanger.SetEnabled(false);
	// TODO: Config und sowas
	this->LoadSkinChangerConfig();

	// Visuals
	this->m_visuals.SetEnabled(true);

	this->m_visuals.SetCrosshair(true);
	this->m_visuals.SetCrosshairShowRecoil(true);
	this->m_visuals.SetSpreadCone(true);
	this->m_visuals.SetHitmarker(true);
	this->m_visuals.NoSmoke(false);
	this->m_visuals.SetHandsDrawStyle(HANDSDRAWSTYLE_NONE);
	this->m_visuals.SetNoVisualRecoil(false);
	this->m_visuals.DisablePostProcessing(true);
	this->m_visuals.SetNoScope(true);
	this->m_visuals.SetNightmode(false);

	this->m_visuals.SetNoFlash(false);
	this->m_visuals.SetFlashPercentage(0.0f);

	this->m_visuals.SetThirdperson(false);
	this->m_visuals.SetThirdpersonDistance(80);

	this->m_visuals.SetFovChange(true);
	this->m_visuals.SetFovValue(110);
	this->m_visuals.SetFovChangeScoped(false);

	this->m_visuals.SetViewmodelFov(true);
	this->m_visuals.SetViewmodelFovValue(90);

	// Mirror
	this->m_mirror.SetEnabled(false);

	// LagCompensation
	this->m_lagcompensation.SetEnabled(true);
	this->m_lagcompensation.SetRageLagCompensationEnabled(true);
	this->m_lagcompensation.SetLegitLagCompensationEnabled(true);
	this->m_lagcompensation.SetLegitLagCompensationDuration(200);
	this->m_lagcompensation.SetDrawStyle(LC_DRAWSTYLE_BONES);
	this->m_lagcompensation.SetDrawFrequency(2);
	this->m_lagcompensation.SetDrawOnlyVisible(false);

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("pjËßHe‡µzjı").ToCharArray(), false); // game_newmap
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("tx⁄•vf‡ùsbˆ°xeÎßt‡¶").ToCharArray(), false); // cs_game_disconnected
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("d|Ï∂tc⁄∂rjË").ToCharArray(), false); // switch_team
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄±gjÚ¨ro").ToCharArray(), false); // player_spawned
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄™byÒ").ToCharArray(), false); // player_hurt
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄¶rjÒ™").ToCharArray(), false); // player_death
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sTˆ∂vyÒ").ToCharArray(), false); // round_start
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sT‡¨s").ToCharArray(), false); // round_end
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("`n‰≤xe⁄§~y‡").ToCharArray(), false); // weapon_fire

	// CGui initialization
	m_pGui = CGui::Instance();
	m_pGui->Setup();

	// Initialize menu
	m_pMenu = CMenu::Instance();
	m_pMenu->Init(this);
	m_pMenu->CreateMenu();
	m_pMenu->ApplySettings();
}

void CApplication::Hook()
{
	// IClientMode
	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClient))[10] + 0x05));
	// CInput
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[15] + 0x01);

	// ClientState
	this->m_pClientState = **(IClientState***)(CPattern::FindPattern(
		(BYTE*)this->EngineDll(),
		ENGINEDLL_SIZE,
		(BYTE*)"\xA1\x00\x00\x00\x00\x8B\x80\x00\x00\x00\x00\xC3",
		"a----gh----e"
	) + 0x01);

	// IViewRender
	m_pViewRender = **(IViewRender***)(CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\xFF\x75\x10\x8B\x0D\x00\x00\x00\x00\xFF\x75\x0C",
		"abcdefgh----ijk"
	) + 0x08);

	m_pClientModeHook = new VTableHook((DWORD*)dwClientMode);
	m_pOverrideView = (OverrideView_t)m_pClientModeHook->Hook(18, (DWORD*)hk_OverrideView);
	m_pCreateMove = (CreateMove_t)m_pClientModeHook->Hook(24, (DWORD*)hk_CreateMove);
	m_pGetViewModelFov = (GetViewModelFov_t)m_pClientModeHook->Hook(35, (DWORD*)hk_GetViewModelFov);

	m_pModelRenderHook = new VTableHook((DWORD*)this->m_pModelRender);
	m_pDrawModelExecute = (DrawModelExecute_t)m_pModelRenderHook->Hook(21, (DWORD*)hk_DrawModelExecute);

	m_pClientHook = new VTableHook((DWORD*)this->m_pClient);
	m_pFrameStageNotify = (FrameStageNotify_t)m_pClientHook->Hook(36, (DWORD*)hk_FrameStageNotify);

	m_pPanelHook = new VTableHook((DWORD*)this->m_pPanel);
	m_pPaintTraverse = (PaintTraverse_t)m_pPanelHook->Hook(41, (DWORD*)hk_PaintTraverse);

	m_pSurfaceHook = new VTableHook((DWORD*)this->m_pSurface);
	m_pPlaySound = (PlaySound_t)m_pSurfaceHook->Hook(82, (DWORD*)hk_PlaySound);

	m_pGameEventManagerHook = new VTableHook((DWORD*)this->m_pGameEventManager);
	m_pFireEventClientSide = (FireEventClientSide_t)m_pGameEventManagerHook->Hook(9, (DWORD*)hk_FireEventClientSide);

	m_pViewRenderHook = new VTableHook((DWORD*)m_pViewRender);
	m_pRenderViewFn = (RenderView_t)m_pViewRenderHook->Hook(6, (DWORD*)hk_RenderView);
	m_pRenderSmokeOverlay = (RenderSmokeOverlay_t)m_pViewRenderHook->Hook(40, (DWORD*)hk_RenderSmokeOverlay);

	m_pEngineSoundHook = new VTableHook((DWORD*)m_pEngineSound);
	m_pEmitSound1 = (EmitSound1_t)m_pEngineSoundHook->Hook(5, (DWORD*)hk_EmitSound1);
	m_pEmitSound2 = (EmitSound2_t)m_pEngineSoundHook->Hook(6, (DWORD*)hk_EmitSound2);

	// Proxy functions
	m_pSequenceProxy = m_pNetVarSequence->HookProxy(CApplication::hk_SetViewModelSequence);
	m_pLowerBodyYawProxy = m_pNetVarLowerBodyYaw->HookProxy(CApplication::hk_SetLowerBodyYawTarget);

	//this->m_misc.SetClanTag(".mechanics"); //todo: dynamic!!

	this->m_bIsHooked = true;
	this->m_bInitialHookDone = true;
}

void CApplication::GetModuleInfo()
{
	int iLenFullpath;
	char pFullpath[MAX_PATH];

	iLenFullpath = GetModuleFileName(m_hModule, pFullpath, MAX_PATH);

	for (int i = iLenFullpath - 1; i >= 0; i--)
	{
		if (pFullpath[i] == '\\')
		{
			m_iLenFilepath = i + 1;
			m_pFilepath = new char[m_iLenFilepath + 1];
			strncpy(m_pFilepath, pFullpath, m_iLenFilepath + 1);
			m_pFilepath[m_iLenFilepath] = '\0';

			m_iLenFilename = iLenFullpath - i - 1;
			m_pFilename = new char[m_iLenFilename + 1];
			strcpy(m_pFilename, pFullpath + i + 1);

			break;
		}
	}

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Module path: %s\n", m_pFilepath);
	g_pConsole->Write(LOGLEVEL_INFO, "Module name: %s\n", m_pFilename);
	g_pConsole->WritePlain("\n");
#endif
}

// Singleton
CApplication::CApplication()
{
	// TODO: Der konstruktor muss *ALLE* pointer auf NULL setzen, der destruktor
	//		 alle ptr != NULL lˆschen
	m_pNetVarMgr = NULL;

	m_pClientModeHook = NULL;
	m_pModelRenderHook = NULL;
	m_pClientHook = NULL;
	m_pPanelHook = NULL;
	m_pSurfaceHook = NULL;
	m_pGameEventManagerHook = NULL;
	m_pViewRenderHook = NULL;
	m_pEngineSoundHook = NULL;

	m_bGotSendPackets = false;

	m_bInitialHookDone = false;
	m_bIsHooked = false;
}

CApplication::CApplication(CApplication const&)
{
}

CApplication::~CApplication()
{
	if (m_pEngineSoundHook)
		delete m_pEngineSoundHook;

	if (m_pViewRenderHook)
		delete m_pViewRenderHook;

	if (m_pGameEventManagerHook)
		delete m_pGameEventManagerHook;

	if (m_pSurfaceHook)
		delete m_pSurfaceHook;

	if (m_pPanelHook)
		delete m_pPanelHook;

	if (m_pClientHook)
		delete m_pClientHook;

	if (m_pModelRenderHook)
		delete m_pModelRenderHook;

	if (m_pClientModeHook)
		delete m_pClientModeHook;

	if (m_pNetVarMgr)
		delete m_pNetVarMgr;
}

void CorrectMovement(CUserCmd* pCmd, QAngle& qOrigAngles)
{
	CApplication* pApp = CApplication::Instance();

	Vector vecViewForward, vecViewRight, vecViewUp, vecAimForward, vecAimRight, vecAimUp;
	QAngle qViewAngles, qAimAngles;

	pApp->EngineClient()->GetViewAngles(qViewAngles);

	float flForward = pCmd->forwardmove;
	float flRight = pCmd->sidemove;
	float flUp = pCmd->upmove;

	qViewAngles = QAngle(0.0f, qViewAngles.y, 0.0f);
	qAimAngles = QAngle(0.0f, pCmd->viewangles[1], 0.0f);

	AngleVectors(qViewAngles, &vecViewForward, &vecViewRight, &vecViewUp);
	AngleVectors(qAimAngles, &vecAimForward, &vecAimRight, &vecAimUp);

	vecViewForward.NormalizeAngles();
	vecViewRight.NormalizeAngles();
	vecViewUp.NormalizeAngles();

	Vector vecForwardNorm = vecViewForward * flForward;
	Vector vecRightNorm = vecViewRight * flRight;
	Vector vecUpNorm = vecViewUp * flUp;

	pCmd->forwardmove = vecForwardNorm.Dot(vecAimForward) + vecRightNorm.Dot(vecAimForward) + vecUpNorm.Dot(vecAimForward);
	pCmd->sidemove = vecForwardNorm.Dot(vecAimRight) + vecRightNorm.Dot(vecAimRight) + vecUpNorm.Dot(vecAimRight);
	pCmd->upmove = vecForwardNorm.Dot(vecAimUp) + vecRightNorm.Dot(vecAimUp) + vecUpNorm.Dot(vecAimUp);

	ClampMovement(pCmd);
}

void NormalizeAngles(CUserCmd* pUserCmd)
{
	// Normalize pitch
	while (pUserCmd->viewangles[0] > 89.0f)
	{
		pUserCmd->viewangles[0] -= 178.0f;
	}
	while (pUserCmd->viewangles[0] < -89.0f)
	{
		pUserCmd->viewangles[0] += 178.0f;
	}

	// Normalize yaw
	while (pUserCmd->viewangles[1] > 179.9999f)
	{
		pUserCmd->viewangles[1] -= 360.0f;
	}
	while (pUserCmd->viewangles[1] < -179.9999f)
	{
		pUserCmd->viewangles[1] += 360.0f;
	}

	pUserCmd->viewangles[2] = 0.0f;
}

void ClampMovement(CUserCmd* pUserCmd)
{
	// Clamp forward
	if (pUserCmd->forwardmove > 450.0f)
	{
		pUserCmd->forwardmove = 450.0f;
	}
	else if (pUserCmd->forwardmove < -450.0f)
	{
		pUserCmd->forwardmove = -450.0f;
	}

	// Clamp sidemove
	if (pUserCmd->sidemove > 450.0f)
	{
		pUserCmd->sidemove = 450.0f;
	}
	else if (pUserCmd->sidemove < -450.0f)
	{
		pUserCmd->sidemove = -450.0f;
	}

	// Clamp upmove
	if (pUserCmd->sidemove > 320.0f)
	{
		pUserCmd->sidemove = 320.0f;
	}
	else if (pUserCmd->sidemove < -320.0f)
	{
		pUserCmd->sidemove = -320.0f;
	}
}

DWORD ThreadFreeLibrary(void* pParam)
{
	// Wait 1 sec for hooks to finish running
	Sleep(1000);
	FreeLibraryAndExitThread((HMODULE)pParam, 0);

	return 0;
}