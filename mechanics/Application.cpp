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
	// Enable mouse again
	this->m_pGui->SetEnableMouse(true);

	// Unregister listener
	this->m_pGameEventManager->RemoveListener(&this->m_gameEventListener);

	// Unhook everything
	this->Unhook();

	// Free ResourceManager
	delete g_pResourceManager;
	g_pResourceManager = NULL;

	// Free console
	delete g_pConsole;
	g_pConsole = NULL;


	// ----------- Our stuff -----------

	// FovChanger
	if (m_visuals.GetFovChangeScoped())
	{
		ConVar* pZoomSensitivity = m_pCVar->FindVar(CXorString("mdÍØHx‡¨dbÒ´abÒªHy‰∂~d⁄Øx~ˆß").ToCharArray());
		pZoomSensitivity->SetValue(m_visuals.GetZoomSensitivity());
	}

	// SkinChanger Modeldelete
	if (m_skinchanger.GetEnabled())
		m_pClientState->ForceFullUpdate();

	// ClanTag
	if (m_misc.GetIsCustomClanTag())
		m_misc.SetClanTag(NULL);
	else if (m_misc.GetNoName())
		m_misc.SetNoNameClanTag(false);


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

	this->m_bIsHooked = true;
}

bool __fastcall CApplication::hk_CreateMove(void* ecx, void* edx, float fInputSampleTime, CUserCmd* pUserCmd)
{
	bool rtn = m_pCreateMove(ecx, fInputSampleTime, pUserCmd);

	CApplication* pApp = CApplication::Instance();

	// Update timer for hitmarker
	pApp->Visuals()->UpdateHitmarker(fInputSampleTime);

	// Instantly return
	if (!pUserCmd || !pUserCmd->command_number)
		return false;

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
			pApp->m_flPredLbyUpdateTime = pApp->GlobalVars()->curtime + 1.1f;

			// Save Viewangles before doing stuff
			pApp->EngineClient()->GetViewAngles(pApp->GetClientViewAngles());
			QAngle qOldAngles = pApp->GetClientViewAngles();

			// Create instance of CreateMoveParam
			CreateMoveParam createMoveParam = { fInputSampleTime, pUserCmd };

			// New tick, so we didn't get any targets yet
			pApp->m_targetSelector.SetHasTargets(false);

			// Update Aimbot
			pApp->Ragebot()->Update((void*)&createMoveParam);
			// Update Triggerbot
			pApp->Triggerbot()->Update((void*)&createMoveParam);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Miscs
			pApp->Misc()->AutoRevolver(pUserCmd);
			pApp->Misc()->AutoPistol(pUserCmd);
			pApp->Misc()->NoRecoil(pUserCmd);
			pApp->Misc()->Fakelag(pUserCmd);
			pApp->Misc()->AutoStrafe(pUserCmd);
			pApp->Misc()->JumpScout(pUserCmd);

			// Update AntiAim
			pApp->AntiAim()->Update(pUserCmd);

			// Correct movement & angles
			CorrectMovement(pUserCmd, qOldAngles);
			NormalizeAngles(pUserCmd);

			// Set ViewAngles we prepared for display
			pApp->EngineClient()->SetViewAngles(pApp->GetClientViewAngles());

			if (!*pApp->m_bSendPackets && pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bSendPackets && !pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bLbyUpdate ||
				!pApp->AntiAim()->GetEnabled())
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
			for (int i = 0; i < pApp->EngineClient()->GetMaxClients(); i++)
			{
				IClientEntity* pEntity = pApp->EntityList()->GetClientEntity(i);

				if (!pEntity)
					continue;

				// TODO: crash access violation beim schreiben
				//		 hint: vllt Entity checks?
				pEntity->GetAngEyeAngles()->y = pEntity->GetLowerBodyYaw();
			}

			pApp->SkinChanger()->Update(pLocalEntity);
		}
	}
	else if (curStage == FRAME_RENDER_START)
	{
		if (pApp->EngineClient()->IsInGame())
		{
			if (pLocalEntity->IsAlive())
			{
				pApp->Visuals()->NoSmoke();
				pApp->Visuals()->ThirdpersonAntiAim();
			}

			pApp->Visuals()->NoFlash();
		}

		// Menu input handling
		pApp->m_pMenu->HandleInput();
	}
	else if (curStage == FRAME_NET_UPDATE_END)
	{
		//g_pConsole->Write("%f\n", pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer())->GetSimulationTime());
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

		pApp->Chams()->Render(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pHands = pApp->Visuals()->HandsDrawStyle(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}

	//IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
	//IClientEntity* pRenderEntity = pApp->EntityList()->GetClientEntity(pInfo.entity_index);

	//if(pLocalEntity == pRenderEntity/* && pApp->Visuals()->GetThirdperson()*/)
	//{
	//	matrix3x4_t pMyMat[MAXSTUDIOBONES];
	//	matrix3x4_t pBoneMat[MAXSTUDIOBONES];
	//	Vector origin, newOrigin;
	//	QAngle ang;// = *pLocalEntity->GetAngEyeAngles();
	//	//ang.y = 90.0f;
	//	ang.x = 0.0f;
	//	ang.y = 180.0f;
	//	ang.z = 0.0f;
	//	for(int i = 0; i < MAXSTUDIOBONES; i++)
	//	{
	//		MatrixCopy(pCustomBoneToWorld[i], pBoneMat[i]);

	//		origin.x = pCustomBoneToWorld[i][0][3];
	//		origin.y = pCustomBoneToWorld[i][1][3];
	//		origin.z = pCustomBoneToWorld[i][2][3];
	//		
	//		AngleMatrix(ang, pMyMat[i]);
	//		//VectorRotate(pInfo.origin, pMyMat[i], &newOrigin);
	//		VectorRotate(origin, pMyMat[i], &newOrigin);

	//		//VectorTransform(

	//		MatrixSetColumn(newOrigin, 3, pBoneMat[i]);
	//	}

	//	m_pDrawModelExecute(ecx, ctx, state, pInfo, pBoneMat);
	//}

	// Call original func
	m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);

	// This is necessary for stuff to work properly
	pApp->ModelRender()->ForcedMaterialOverride(NULL);
}

void __fastcall CApplication::hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->EngineClient()->IsInGame())
	{
		if (pApp->Misc()->NoScope(vguiPanel))
			return;

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

		if (vguiMatSystemTopPanel == vguiPanel)
		{
			ISurface* pSurface = pApp->Surface();

			pApp->Gui()->GetWorldToScreenMatrix();

			// Draw NoScope & SpecList
			pApp->Misc()->DrawNoScope();
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Update((void*)pSurface);

			// Draw Sound Esp
			pApp->SoundEsp()->Update();

			// Draw rear mirror
			pApp->Mirror()->Render(pSurface, pApp->Menu()->GetMirrorWindow());

			// Draw Hitmarker
			pApp->Visuals()->DrawHitmarker();

			// Draw Crosshair last (but not least)
			pApp->Visuals()->DrawCrosshair();

			// Draw Menu least ;)
			pApp->Menu()->Draw(pSurface);

			// LBY Indicator
			pApp->AntiAim()->DrawLBYIndicator();
		}
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

	// TODO: Warum eig relativer Wert? Sollte man nicht n absolutes FOV angeben kˆnnen? :)
	//			CVar 'viewmodel_fov' steht default auf 60 (Wahrscheinlich default FOV)
	return m_pGetViewModelFov(ecx) + 20.0f;
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
	CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

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
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
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
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
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
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
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

/*void BtnDown(IControl* p)
{
CButton* btn = (CButton*)p;
g_pConsole->Write("Button down (%s)\n", btn->ContentText());
}*/

void CApplication::Setup()
{
	// Setup console
	g_pConsole = new CConsole();

	// Setup strings
	CXorString clientDll("tgÏßy´¶{g");
	CXorString engineDll("re‚´yn´¶{g");
	CXorString materialSystemDll("zjÒßeb‰Ædrˆ∂rf´¶{g");
	CXorString createInterface("Ty‡£cnÃ¨cn˜§vh‡");
	CXorString VEngineClient("ANÎ•~e‡Å{b‡¨c;¥ˆ");
	CXorString VClient("AHÈ´reÒÚ&3");
	CXorString VClientEntityList("AHÈ´reÒáyÏ∂nGÏ±c;µÒ");
	CXorString VModelInfo("¸òÖëœπ£õÃ∫©ô√∞ÑÅöÂﬁ", 0x1235AFAA);
	CXorString VModelRender("ANÎ•~e‡èxo‡Æ':≥");
	CXorString EngineTraceClient("_cajthRq{nc@vdcmn=67", 0x1A);
	CXorString VMaterialSystem("AF‰∂ryÏ£{X¸±cnËÚ/;");
	CXorString vgui2Dll("al´%%·Æ{");
	CXorString vguiSurfaceDll("al´zjÒ±by„£tn´¶{g");
	CXorString VguiPanel("AL–ãH[‰¨rgµÚ.");
	CXorString VguiSurface("AL–ãHX∞qjÊß'8¥");
	CXorString GameEventListener("l‘\a`n√\xfk∆\ade‘\r`y•z\x17", 0xF12B);
	CXorString vphysicsDll("a{ÌªdbÊ±9oÈÆ");
	CXorString physicsSurfaceProps("A[ÌªdbÊ±D~˜§vh‡íedı±';¥");
	CXorString renderView("ANÎ•~e‡êre·ße]Ïß`;¥ˆ");

	// Grab engine addresses
	this->m_dwClientDll = (DWORD)GetModuleHandle(clientDll.ToCharArray());
	this->m_dwEngineDll = (DWORD)GetModuleHandle(engineDll.ToCharArray());
	this->m_dwMaterialSystemDll = (DWORD)GetModuleHandle(materialSystemDll.ToCharArray());
	this->m_dwVGui2Dll = (DWORD)GetModuleHandle(vgui2Dll.ToCharArray());
	this->m_dwVGuiSurfaceDll = (DWORD)GetModuleHandle(vguiSurfaceDll.ToCharArray());
	this->m_dwVPhysicsDll = (DWORD)GetModuleHandle(vphysicsDll.ToCharArray());
	this->m_dwVStdLibDll = (DWORD)GetModuleHandle(CXorString("axÒ¶{bÁÏsgÈ").ToCharArray());
	CreateInterfaceFn CreateClientInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwClientDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateEngineInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwEngineDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateMaterialSystemInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwMaterialSystemDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVgui2Interface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGui2Dll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVguiSurfaceInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVGuiSurfaceDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVPhysicsInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVPhysicsDll, createInterface.ToCharArray());
	CreateInterfaceFn CreateVStdLibInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)this->m_dwVStdLibDll, createInterface.ToCharArray());

	m_pEngineClient = (IVEngineClient*)CreateEngineInterface(VEngineClient.ToCharArray(), NULL);
	m_pClient = (IBaseClientDLL*)CreateClientInterface(VClient.ToCharArray(), NULL);
	m_pEntityList = (IClientEntityList*)CreateClientInterface(VClientEntityList.ToCharArray(), NULL);
	m_pModelInfo = (IVModelInfo*)CreateEngineInterface(VModelInfo.ToCharArray(), NULL);
	m_pModelRender = (IVModelRender*)CreateEngineInterface(VModelRender.ToCharArray(), NULL);
	m_pRenderView = (IVRenderView*)CreateEngineInterface(renderView.ToCharArray(), NULL);
	m_pEngineTrace = (IEngineTrace*)CreateEngineInterface(EngineTraceClient.ToCharArray(), NULL);
	m_pMaterialSystem = (IMaterialSystem*)CreateMaterialSystemInterface(VMaterialSystem.ToCharArray(), NULL);
	m_pCVar = (ICVar*)CreateVStdLibInterface(CXorString("ANÎ•~e‡Åaj˜Ú'<").ToCharArray(), NULL);

	m_pPanel = (IPanel*)CreateVgui2Interface(VguiPanel.ToCharArray(), NULL);
	m_pSurface = (ISurface*)CreateVguiSurfaceInterface(VguiSurface.ToCharArray(), NULL);
	m_pGameEventManager = (IGameEventManager2*)CreateEngineInterface(GameEventListener.ToCharArray(), NULL);
	m_pPhysicsSurfaceProps = (IPhysicsSurfaceProps*)CreateVPhysicsInterface(physicsSurfaceProps.ToCharArray(), NULL);
	m_pEngineSound = (IEngineSound*)CreateEngineInterface("IEngineSoundClient003", NULL); // TODO

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
	/*FILE* pFile = fopen("C:\\Users\\Robin\\Desktop\\dump.txt", "w");
	if (pFile)
	{
		g_pConsole->Write("Dumping NetVars ... ");
		//CNetVarManager::DumpAll(pFile, m_pClient->GetAllClasses());
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseEntity");
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BasePlayer");
		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_CSPlayer");
		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseCombatWeapon");
		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseViewModel");

		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_AttributeContainer");
		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_ScriptCreatedItem");
		CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseAttributableItem");

		fclose(pFile);
		g_pConsole->Write("Done!\n");
	}*/

	CXorString xorBaseEntity("S_⁄Ävx‡áyÏ∂n"); // DT_BaseEntity
	CXorString xorBasePlayer("S_⁄Ävx‡í{j¸ße"); // DT_BasePlayer
	CXorString xorCSPlayer("S_⁄ÅD[È£nn˜"); // DT_CSPlayer
	CXorString xorLocalPlayerExclusive("S_⁄éxh‰ÆGg‰ªry¿∫tg±~}‡"); // DT_LocalPlayerExclusive
	CXorString xorBaseCombatWeapon("S_⁄Ävx‡ÅxfÁ£c\\‡£gdÎ"); // DT_BaseCombatWeapon
	CXorString xorWeaponCSBase("S_⁄ïrjı≠yH÷Ävx‡"); // DT_WeaponCSBase
	CXorString xorBaseCSGrenade("S_⁄Ävx‡ÅDL˜ßyj·ß"); // DT_BaseCSGrenade
	CXorString xorBaseCombatCharacter("S_⁄Ävx‡ÅxfÁ£cHÌ£ejÊ∂ry"); // DT_BaseCombatCharacter
	CXorString xorBaseViewModel("S_⁄Ävx‡î~nÚèxo‡Æ"); // DT_BaseViewModel

	m_pNetVarMgr = new CNetVarManager();
	m_pNetVarMgr->AddTable(xorBaseEntity.ToCharArray());
	m_pNetVarMgr->AddTable(xorBasePlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorCSPlayer.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatWeapon.ToCharArray());
	//m_pNetVarMgr->AddTable("DT_BaseAttributableItem");
	m_pNetVarMgr->AddTable(xorWeaponCSBase.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCSGrenade.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseCombatCharacter.ToCharArray());
	m_pNetVarMgr->AddTable(xorBaseViewModel.ToCharArray());
	m_pNetVarMgr->LoadTables(m_pClient->GetAllClasses(), true);

	Offsets::m_nModelIndex = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_nModelIndex*/CXorString("zTÎèxo‡Æ^e·ßo").ToCharArray());
	Offsets::m_hMyWeapons = m_pNetVarMgr->GetOffset(xorBaseCombatCharacter.ToCharArray(), /*m_hMyWeapons*/CXorString("zTÌèn\\‡£gdÎ±").ToCharArray());
	Offsets::m_hViewModel = m_pNetVarMgr->GetOffset(xorBasePlayer.ToCharArray(), /*m_hViewModel[0]*/CXorString("zTÌî~nÚèxo‡ÆL;ÿ").ToCharArray());
	Offsets::m_flSimulationTime = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_flSimulationTime*/CXorString("zT„ÆDbË∑{jÒ´xe—´zn").ToCharArray());
	Offsets::m_vecOrigin = m_pNetVarMgr->GetOffset(xorBaseEntity.ToCharArray(), /*m_vecOrigin*/CXorString("zTÛßtD˜´pbÎ").ToCharArray());
	Offsets::m_vecViewOffset = m_pNetVarMgr->GetOffset(2, xorBasePlayer.ToCharArray(), xorLocalPlayerExclusive.ToCharArray(), /*m_vecViewOffset[0]*/CXorString("zTÛßt]Ïß`D„§dnÒô'V").ToCharArray());
	Offsets::m_angEyeAngles = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_angEyeAngles*/CXorString("zT‰¨pN¸ßVe‚Ærx").ToCharArray());
	Offsets::m_flLowerBodyYawTarget = m_pNetVarMgr->GetOffset(xorCSPlayer.ToCharArray(), /*m_flLowerBodyYawTarget*/CXorString("zT„Æ[dÚßeIÍ¶nR‰µCj˜•r").ToCharArray());
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
	Offsets::m_iClip1 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip1*/CXorString("zTÏÅ{bıÛ").ToCharArray());
	Offsets::m_iClip2 = m_pNetVarMgr->GetOffset(xorBaseCombatWeapon.ToCharArray(), /*m_iClip2*/CXorString("zTÏÅ{bı").ToCharArray());
	Offsets::m_flNextPrimaryAttack = m_pNetVarMgr->GetOffset(2, xorBaseCombatWeapon.ToCharArray(),
		/*DT_LocalActiveWeaponData*/CXorString("S_⁄éxh‰ÆVhÒ´an“ßv{Í¨SjÒ£").ToCharArray(),
		/*m_flNextPrimaryAttack*/CXorString("zT„ÆYn˝∂GyÏØvy¸Éc‰°|").ToCharArray());

	Offsets::m_fAccuracyPenalty = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_fAccuracyPenalty*/CXorString("zT„Éth∞vh¸íre‰Æcr").ToCharArray());
	Offsets::m_flPostponeFireReadyTime = m_pNetVarMgr->GetOffset(xorWeaponCSBase.ToCharArray(), /*m_flPostponeFireReadyTime*/CXorString("zT„ÆGdˆ∂gdÎßQb˜ßEn‰¶n_ÏØr").ToCharArray());

	Offsets::m_fThrowTime = m_pNetVarMgr->GetOffset(xorBaseCSGrenade.ToCharArray(), /*m_fThrowTime*/CXorString("zT„ñyÍµCbËß").ToCharArray());

	// Grab NetVars later required for hooking
	m_pNetVarSequence = m_pNetVarMgr->GetNetVar(xorBaseViewModel.ToCharArray(), /*m_nSequence*/CXorString("zTÎërzßyh‡").ToCharArray());

	m_misc.SetReadyCallback((IsReadyCallback_t)(CPattern::FindPattern(
		(BYTE*)this->ClientDll(),
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE",
		"ghdrtzuigkog----trg"
	)));

	// Target Selector
	this->m_targetSelector.Setup(this);
	this->m_targetSelector.SetMultipoint(false);
	this->m_targetSelector.SetVisibleMode(VISIBLEMODE_CANHIT);
	this->m_targetSelector.SetCheckHitbox(TARGET_HITBOX_HEAD, true);

	// Setups
	this->m_ragebot.Setup();
	this->m_triggerbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_soundEsp.Setup();
	this->m_chams.Setup();
	this->m_misc.Setup();
	this->m_skinchanger.Setup();
	this->m_visuals.Setup();
	this->m_mirror.Setup();

	// Aimbot
	this->m_ragebot.SetEnabled(true);
	this->m_ragebot.SetAutoshoot(true);
	this->m_ragebot.SetAutoscope(true);
	this->m_ragebot.SetSilentAim(true);
	this->m_ragebot.SetAutoReload(true);
	this->m_ragebot.SetAutoZeus(true);
	this->m_ragebot.SetTargetCriteria(TARGETCRITERIA_VIEWANGLES);

	// Triggerbot
	this->m_triggerbot.SetEnabled(false);

	// Antiaim
	this->m_antiAim.SetEnabled(true);
	this->m_antiAim.SetPitchSetting(PITCHANTIAIM_DOWN);
	this->m_antiAim.SetYawSetting(YAWANTIAIM_STATIC);
	this->m_antiAim.SetYawOffset(-90);
	this->m_antiAim.SetYawFakeSetting(FAKEYAWANTIAIM_STATIC);
	this->m_antiAim.SetYawFakeOffset(90);
	this->m_antiAim.SetDrawLbyIndicator(true);
	this->m_antiAim.SetLbyBreaker(true);

	// Bhop
	this->m_bhop.SetEnabled(true);

	// Esp
	this->m_esp.SetEnabled(true);
	this->m_esp.SetFillBoundingBox(true);
	this->m_esp.SetDrawBoundingBox(true);
	this->m_esp.SetDrawNames(true);
	this->m_esp.SetDrawHealthBar(true);
	this->m_esp.SetDrawHealthNumber(true);
	this->m_esp.SetDrawArmorBar(false);
	this->m_esp.SetDrawOwnTeam(false);
	this->m_esp.SetDrawOwnModel(true);
	this->m_esp.SetDrawOnlyVisible(false);
	this->m_esp.SetDrawOnlySpotted(false);
	this->m_esp.SetDrawOutline(true);
	this->m_esp.SetFadeoutEnabled(true);
	this->m_esp.SetFadeoutTime(1.0f);
	this->m_esp.SetColorCT(Color(0, 0, 255));
	this->m_esp.SetColorT(Color(255, 0, 0));
	this->m_esp.GetColorSpotted();

	// Sound Esp
	this->m_soundEsp.SetEnabled(true);
	this->m_soundEsp.SetShowTime(1.0f);
	this->m_soundEsp.SetFadeTime(1.0f);
	this->m_soundEsp.SetFadeoutEnabled(true);
	this->m_soundEsp.SetDrawOwnTeam(false);
	this->m_soundEsp.SetDrawVisible(false);

	// Chams
	this->m_chams.SetEnabled(true);
	this->m_chams.SetRenderTeam(false);
	this->m_chams.SetRenderLocalplayer(true);
	this->m_chams.SetIgnoreZIndex(false);
	this->m_chams.SetFlatModels(false);

	// Misc
	this->m_misc.SetEnabled(true);
	this->m_misc.SetNoRecoil(false);
	this->m_misc.SetFakelag(false);
	this->m_misc.SetAutoStrafe(true);
	this->m_misc.SetNoScope(true);
	this->m_misc.SetAutoPistol(true);
	this->m_misc.SetShowSpectators(false);
	this->m_misc.SetShowOnlyMySpectators(false);
	this->m_misc.SetShowOnlyMyTeamSpectators(false);
	this->m_misc.SetJumpScout(true);
	this->m_misc.SetNoName(false);
	this->m_misc.SetAutoAccept(true);

	// SkinChanger
	this->m_skinchanger.SetEnabled(true);
	// TODO: Config und sowas
	this->LoadSkinChangerConfig();

	// Visuals
	this->m_visuals.SetEnabled(true);

	this->m_visuals.SetCrosshair(true);
	this->m_visuals.SetCrosshairShowRecoil(true);
	this->m_visuals.SetHitmarker(true);
	this->m_visuals.SetNoSmoke(true);
	this->m_visuals.SetHandsDrawStyle(HANDSDRAWSTYLE_NOHANDS);
	this->m_visuals.SetNoVisualRecoil(true);
	this->m_visuals.SetDisablePostProcessing(true);

	this->m_visuals.SetNoFlash(true);
	this->m_visuals.SetFlashPercentage(0.f);

	this->m_visuals.SetThirdperson(false);
	this->m_visuals.SetThirdpersonDistance(150);

	this->m_visuals.SetFovChange(true);
	this->m_visuals.SetFovValue(110);
	this->m_visuals.SetFovChangeScoped(true);


	// Mirror
	this->m_mirror.SetEnabled(false);

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("pjËßHe‡µzjı").ToCharArray(), false); // game_newmap
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("tx⁄•vf‡ùsbˆ°xeÎßt‡¶").ToCharArray(), false); // cs_game_disconnected
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("d|Ï∂tc⁄∂rjË").ToCharArray(), false); // switch_team
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄±gjÚ¨ro").ToCharArray(), false); // player_spawned
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄™byÒ").ToCharArray(), false); // player_hurt
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄¶rjÒ™").ToCharArray(), false); // player_death
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sTˆ∂vyÒ").ToCharArray(), false); // round_start
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sT‡¨s").ToCharArray(), false); // round_end

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

	this->m_misc.SetClanTag(".mechanics"); //todo: dynamic!!

	this->m_bIsHooked = true;
	this->m_bInitialHookDone = true;
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