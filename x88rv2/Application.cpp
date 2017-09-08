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
RenderSmokePostViewmodel_t CApplication::m_pRenderSmokePostViewmodel;

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
	// Unregister listener
	this->m_pGameEventManager->RemoveListener(&this->m_gameEventListener);

	// Restore proxy functions
	m_pProxyProp->m_ProxyFn = this->m_pSequenceProxy;

	// Unhook everything
	this->Unhook();

	// Free ResourceManager
	delete g_pResourceManager;
	g_pResourceManager = NULL;

	// Free console
	delete g_pConsole;
	g_pConsole = NULL;

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

	// Robins Knifes (T -> Karambit Fade Stattrak)
	this->m_skinchanger.AddSkinReplacement(
		WEAPON_KNIFE_T,
		new CSkinMetadata(
			WEAPON_KNIFE_KARAMBIT,
			38,
			0,
			1337,
			3
		)
	);
	this->m_skinchanger.AddModelReplacement(
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄¶rm‰∑{⁄∂9f·Æ").ToCharArray(),
		CXorString("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄©vy‰Ø9f·Æ").ToCharArray()
	);
	this->m_skinchanger.AddKillIconReplacement(
		CXorString("|eÏ§rTÒ").ToCharArray(),
		CXorString("|eÏ§rTÓ£ejË†~").ToCharArray()
	);

	// Robins Knifes (CT -> Karambit Fade Stattrak)
	this->m_skinchanger.AddSkinReplacement(
		WEAPON_KNIFE,
		new CSkinMetadata(
			WEAPON_KNIFE_KARAMBIT,
			38,
			0,
			1337,
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
	// Reverse order, in case of any dependencies
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

			// TODO @Nico: Muss AutoRevolver hier oben stehen? :) 
			//			(ich meine ist im endeffekt echt latte, aber ist halt schˆner
			//			 alles von misc zusammen so. Vllt hier weil wegen muss vor Aimbot executed werden?
			//			 -> dann bitte entsprechenden kommentar dran, danke <3)
			pApp->Misc()->AutoRevolver(pUserCmd);

			// Update Aimbot
			pApp->Ragebot()->Update((void*)&createMoveParam);
			// Update Triggerbot
			pApp->Triggerbot()->Update((void*)&createMoveParam);

			// Update Bunnyhop
			pApp->Bhop()->Update(pUserCmd);

			// Update NoRecoil & AutoPistol
			pApp->Misc()->AutoPistol(pUserCmd);
			pApp->Misc()->NoRecoil(pUserCmd);
			// Update Fakelag
			pApp->Misc()->Fakelag(pUserCmd);
			// Update AutoStrafe
			pApp->Misc()->AutoStrafe(pUserCmd);

			// Jumpscout fix
			pApp->Misc()->JumpScout(pUserCmd);

			// Update AntiAim
			pApp->AntiAim()->Update(pUserCmd);

			// Correct movement & angles
			CorrectMovement(pUserCmd, qOldAngles);
			NormalizeAngles(pUserCmd);

			// Set ViewAngles we prepared for display
			pApp->EngineClient()->SetViewAngles(pApp->GetClientViewAngles());

			//todo: check if fake and if sendpackets false
			if (!*pApp->m_bSendPackets && pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bSendPackets && !pApp->AntiAim()->IsFakeYaw() ||
				pApp->m_bLbyUpdate)
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

				pEntity->GetAngEyeAngles()->y = pEntity->GetLowerBodyYaw();
			}

			pApp->SkinChanger()->Update(pLocalEntity);
		}
	}
	else if (curStage == FRAME_RENDER_START)
	{
		if (pApp->EngineClient()->IsInGame())
		{
			pApp->Misc()->DisablePostProcessing();

			if (pLocalEntity->IsAlive())
			{
				pApp->Visuals()->NoSmoke();
				pApp->Visuals()->ThirdpersonAntiAim();
			}

			pApp->Visuals()->NoFlash();
		}

		// Input handling
		pApp->m_inputEvent.Clear();
		pApp->m_inputHandler.CreateInput(&pApp->m_inputEvent, pApp->m_pWindow->IsVisible());
		if (pApp->m_inputEvent.eventType == EVENT_TYPE_KEYBOARD)
		{
			if (pApp->m_inputEvent.buttons & EVENT_BTN_TOGGLEMENU &&
				pApp->m_inputEvent.buttonProperties & EVENT_BTN_TOGGLEMENU)
			{
				bool bVis = !pApp->m_pWindow->IsVisible();
				pApp->m_pWindow->IsVisible(bVis);
				
				// Mouse stuff
				pApp->m_pGui->SetEnableMouse(!bVis);
				pApp->m_pGui->SetDrawMouse(bVis);
			}
			else if (pApp->m_inputEvent.buttons & EVENT_BTN_DETACH &&
				pApp->m_inputEvent.buttonProperties & EVENT_BTN_DETACH)
			{
				pApp->Detach();
			}
			else if (pApp->m_inputEvent.buttons & EVENT_BTN_THIRDPERSON &&
				!(pApp->m_inputEvent.buttonProperties & EVENT_BTN_THIRDPERSON))
			{
				bool bNewValue = !pApp->m_visuals.GetThirdperson();

				pApp->m_pGuiThirdpersonCheckbox->SetChecked(bNewValue);
				pApp->m_visuals.SetThirdperson(bNewValue);
			}
		}

		if (pApp->m_pWindow->IsVisible())
		{
			pApp->m_pWindow->ProcessEvent(&pApp->m_inputEvent);
		}
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

	if (pInfo.pModel)
	{
		const char* pszModelName = pApp->ModelInfo()->GetModelName(pInfo.pModel);

		pApp->Chams()->Render(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
		pApp->Visuals()->HandsDrawStyle(pszModelName, ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}

	/*IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
	IClientEntity* pRenderEntity = pApp->EntityList()->GetClientEntity(pInfo.entity_index);

	if(pLocalEntity == pRenderEntity && pApp->Visuals()->GetThirdperson())
	{
	matrix3x4_t pMyMat[MAXSTUDIOBONES];
	Vector origin = *pLocalEntity->GetOrigin();
	QAngle ang = *pLocalEntity->GetAngEyeAngles();
	ang.y = 90.0f;

	for(int i = 0; i < MAXSTUDIOBONES; i++)
	{
	//origin = Vector(pCustomBoneToWorld[i].c[3]);
	//origin.x = pCustomBoneToWorld[i].c[0][3] + 30.0f;
	//origin.y = pCustomBoneToWorld[i].c[1][3];
	//origin.z = pCustomBoneToWorld[i].c[2][3];

	AngleMatrix(ang, pInfo.origin, pMyMat[i]);

	//pCustomBoneToWorld->c[3][0] += 50.0f;
	//pCustomBoneToWorld->c[3][1] += 50.0f;

	//pRenderInfo->pModelToWorld
	//pCustomBoneToWorld->c[3][2] += 50.0f;
	}

	m_pDrawModelExecute(ecx, ctx, state, pInfo, pMyMat);
	}*/

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

			// Draw NoScope & SpecList
			pApp->Misc()->DrawNoScope();
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Update((void*)pSurface);

			// Draw Hitmarker
			pApp->Visuals()->DrawHitmarker();

			// Draw Crosshair last (but not least)
			pApp->Visuals()->DrawCrosshair();

			// Draw Menu least ;)
			pApp->m_pWindow->Draw(pApp->Surface());
			pApp->m_pGui->DrawMouse(pApp->Surface());

			// LBY Indicator
			if (pApp->Visuals()->GetDrawLbyIndicator())
			{
				IClientEntity* pLocalEntity = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetLocalPlayer());
				// todo. draw lby auslagern
				static unsigned long font = NULL;
				if (font == NULL)
				{
					font = pApp->Surface()->SCreateFont();
					pApp->Surface()->SetFontGlyphSet(font, "Arial-Black", 20, 255, 0, 0, 0x200);
				}
				pApp->Surface()->DrawSetTextFont(font);

				if (pApp->m_bLBY)
					pApp->Surface()->DrawSetTextColor(255, 0, 255, 0);
				else
					pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);

				int w, h;
				pApp->EngineClient()->GetScreenSize(w, h);
				pApp->Surface()->DrawSetTextPos(25, h - 55);
				pApp->Surface()->DrawPrintText(L"LBY", 4);

				static unsigned long font1 = NULL;
				if (font1 == NULL)
				{
					font1 = pApp->Surface()->SCreateFont();
					pApp->Surface()->SetFontGlyphSet(font1, "Arial-Black", 14, 255, 0, 0, 0x200);
				}
				pApp->Surface()->DrawSetTextFont(font1);
				pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);

				char pBuffer[16];
				snprintf(pBuffer, 16, "%i", (int)pLocalEntity->GetLowerBodyYaw());
				wchar_t pBuffW[16];
				mbstowcs(pBuffW, pBuffer, 16);
				int len = lstrlenW(pBuffW);

				pApp->Surface()->DrawSetTextPos(65, h - 57);
				pApp->Surface()->DrawPrintText(pBuffW, len);

				char pBuffer1[16];

				snprintf(pBuffer1, 16, "%i", (int)pApp->m_qLastTickAngles.y);
				wchar_t pBuffW1[16];
				mbstowcs(pBuffW1, pBuffer1, 16);
				int len1 = lstrlenW(pBuffW1);

				pApp->Surface()->DrawSetTextPos(65, h - 46);
				pApp->Surface()->DrawPrintText(pBuffW1, len1);
			}
		}
	}

	m_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}

void __fastcall CApplication::hk_PlaySound(void* ecx, void* edx, const char* fileName)
{
	CApplication* pApp = CApplication::Instance();

	// TODO: Temporary
	g_pConsole->Write("PlaySound => '%s'\n", fileName);

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

	m_pRenderViewFn(ecx, view, hudViewSetup, nClearFlags, whatToDraw);

	/*static ITexture* pRenderTexture = NULL;
	if (!pRenderTexture)
	{
		pApp->MaterialSystem()->BeginRenderTargetAllocation();
		ITexture* tex1 = pApp->MaterialSystem()->CreateRenderTargetTexture(300, 200, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
		ITexture* tex2 = pApp->MaterialSystem()->CreateNamedRenderTargetTextureEx("mirror_ex2", 120, 80, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
		ITexture* tex3 = pApp->MaterialSystem()->CreateNamedRenderTargetTextureEx("mirror_ex3", 120, 80, RT_SIZE_NO_CHANGE, IMAGE_FORMAT_ABGR8888);
		ITexture* tex4 = pApp->MaterialSystem()->CreateNamedRenderTargetTextureEx("mirror_ex4", 120, 80, RT_SIZE_FULL_FRAME_BUFFER, IMAGE_FORMAT_RGBA8888);
		ITexture* tex5 = pApp->MaterialSystem()->CreateNamedRenderTargetTextureEx("mirror_ex5", 120, 80, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);

		pRenderTexture = pApp->MaterialSystem()->CreateNamedRenderTargetTextureEx("mirror_ex", 120, 80, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
		pApp->MaterialSystem()->EndRenderTargetAllocation();
	}*/
	
	if (pApp->EngineClient()->IsInGame())
	{
		if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		{
			CViewSetup myView = view;
			myView.x = 0;
			myView.y = 0;
			myView.width = 180;
			myView.height = 120;

			myView.angles.y += 180.0f;

			//nClearFlags &= ~(VIEW_CLEAR_STENCIL);
			//ITexture* pMirror = pApp->MaterialSystem()->FindTexture("mirror_ex", "RenderTargets");
			ITexture* pMirrorSafe = g_pResourceManager->GetMirror();

			IMatRenderContext* pRenderContext = pApp->MaterialSystem()->GetRenderContext();
			ITexture* pOldTarget = pRenderContext->GetRenderTarget();
			pRenderContext->SetRenderTarget(pMirrorSafe);

			m_pRenderViewFn(
				ecx,
				myView,
				hudViewSetup,
				0,
				0
			);

			pRenderContext->SetRenderTarget(pOldTarget);

			Rect_t src; src.x = 0; src.y = 0; src.width = 180; src.height = 120;
			Rect_t dst; dst.x = 150; dst.y = 150; dst.width = 180; dst.height = 120;
			pRenderContext->CopyTextureToRenderTargetEx(1, pMirrorSafe, &src, &dst);

			//pApp->BaseClient()->RenderView(myView, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_FULL_TARGET | VIEW_CLEAR_OBEY_STENCIL | VIEW_CLEAR_STENCIL, RENDERVIEW_UNSPECIFIED | RENDERVIEW_DRAWVIEWMODEL | RENDERVIEW_DRAWHUD | RENDERVIEW_SUPPRESSMONITORRENDERING);
			//pApp->BaseClient()->RenderView(backupView, 0, 0);
		}
	}
}

void __fastcall CApplication::hk_RenderSmokePostViewmodel(void* ecx, void* edx)
{
	CApplication* pApp = CApplication::Instance();

	// Idk what this is used for,
	// more or less pasta from AimTux :D
	if (pApp->Visuals()->GetNoSmoke())
		return;

	pApp->m_pRenderSmokePostViewmodel(ecx);
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

void DetachBtnClick(IControl* p)
{
	CApplication* pApp = CApplication::Instance();

	pApp->Gui()->SetEnableMouse(true);
	pApp->Detach();
}

void CApplication::Setup()
{
	// Setup console
	g_pConsole = new CConsole();

	// Resource manager
	g_pResourceManager = new CResourceManager();
	g_pResourceManager->Init(this);
	m_pResourceManager = g_pResourceManager;

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

	m_pGlobalVars = **(CGlobalVars***)((*(DWORD**)(m_pClient))[0] + OFFSET_GLOBALS);

	if (m_pEngineClient->IsInGame())
	{
		SetRecoilCompensation(atof(m_pCVar->FindVar(CXorString("`n‰≤xe⁄∞rhÍ´{Tˆ°vg‡").ToCharArray())->value));
	}

	// Create Resources
	m_pResourceManager->CreateMirror();
	m_pResourceManager->CreateTextures();
	m_pResourceManager->CreateFonts();

	// CGui initialization
	m_pGui = CGui::Instance();
	m_pGui->Setup();

	// Print classes & their properties
	FILE* pFile = fopen("C:\\Users\\Robin\\Desktop\\dump.txt", "w");
	if (pFile)
	{
		//CNetVarManager::DumpAll(pFile, m_pClient->GetAllClasses());
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_CSPlayer");
		//CNetVarManager::DumpTable(stdout, m_pClient->GetAllClasses(), "DT_BaseCombatWeapon");
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseEntity");
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BaseViewModel");
		//CNetVarManager::DumpTable(pFile, m_pClient->GetAllClasses(), "DT_BasePlayer");
		fclose(pFile);
	}

	CXorString csplayer("S_⁄ÅD[È£nn˜");
	CXorString basecombatweapon("S_⁄Ävx‡ÅxfÁ£c\\‡£gdÎ");
	CXorString weaponcsbase("S_⁄ïrjı≠yH÷Ävx‡");
	CXorString basecsgrenade("S_⁄Ävx‡ÅDL˜ßyj·ß");
	CXorString baseentity("S_⁄Ävx‡áyÏ∂n");
	CXorString baseplayer("S_⁄Ävx‡í{j¸ße");

	CNetVarManager netVarManager;
	netVarManager.AddTable(baseentity.ToCharArray());
	netVarManager.AddTable(baseplayer.ToCharArray());
	netVarManager.AddTable(csplayer.ToCharArray());
	netVarManager.AddTable(basecombatweapon.ToCharArray());
	//netVarManager.AddTable("DT_BaseAttributableItem");
	netVarManager.AddTable(weaponcsbase.ToCharArray());
	netVarManager.AddTable(basecsgrenade.ToCharArray());
	netVarManager.LoadTables(m_pClient->GetAllClasses());

	CXorString baseclass("ujˆßtg‰±d");
	CXorString localdata("{dÊ£{o‰∂v");
	Offsets::m_nModelIndex = netVarManager.GetOffset(1, baseentity.ToCharArray(), CXorString("zTÎèxo‡Æ^e·ßo").ToCharArray());
	Offsets::m_hMyWeapons = netVarManager.GetOffset(2, baseplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÌèn\\‡£gdÎ±").ToCharArray());
	Offsets::m_hViewModel = netVarManager.GetOffset(1, baseplayer.ToCharArray(), CXorString("zTÌî~nÚèxo‡ÆL;ÿ").ToCharArray());
	Offsets::m_vecOrigin = netVarManager.GetOffset(7, csplayer.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÛßtD˜´pbÎ").ToCharArray());
	Offsets::m_vecViewOffset = netVarManager.GetOffset(3, csplayer.ToCharArray(), baseclass.ToCharArray(), localdata.ToCharArray(), CXorString("zTÛßt]Ïß`D„§dnÒô'V").ToCharArray());
	Offsets::m_angEyeAngles = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zT‰¨pN¸ßVe‚Ærx").ToCharArray());
	Offsets::m_flLowerBodyYawTarget = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zT„Æ[dÚßeIÍ¶nR‰µCj˜•r").ToCharArray());
	Offsets::m_vecVelocity = netVarManager.GetOffset(3, csplayer.ToCharArray(), baseclass.ToCharArray(), localdata.ToCharArray(), CXorString("zTÛßt]‡ÆxhÏ∂nPµü").ToCharArray());
	Offsets::m_iTeamNum = netVarManager.GetOffset(7, csplayer.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÏñrjËåbf").ToCharArray());
	Offsets::m_lifeState = netVarManager.GetOffset(2, csplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÈ´qn÷∂v‡").ToCharArray());
	Offsets::m_iHealth = netVarManager.GetOffset(2, csplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÏärjÈ∂").ToCharArray());
	Offsets::m_ArmorValue = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zTƒ∞zd˜îvgß").ToCharArray());
	Offsets::m_bHasHelmet = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zTÁävxÕß{f‡∂").ToCharArray());
	Offsets::m_fFlags = netVarManager.GetOffset(2, csplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("zT„Ñ{j‚±").ToCharArray());
	Offsets::m_bSpotted = netVarManager.GetOffset(7, csplayer.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÁëgdÒ∂ro").ToCharArray());
	Offsets::m_bGunGameImmunity = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zTÁÖbe¬£znÃØz~Î´cr").ToCharArray());
	Offsets::m_flFlashMaxAlpha = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zT„ÆQg‰±F‰∫Vgı™v").ToCharArray());
	//Offsets::m_bMoveType = netVarManager.GetOffset(1, "DT_BaseEntity", "m_bMoveType");
	Offsets::m_bIsScoped = netVarManager.GetOffset(1, csplayer.ToCharArray(), CXorString("zTÁãdXÊ≠gn·").ToCharArray());
	Offsets::m_iShotsFired = netVarManager.GetOffset(2, csplayer.ToCharArray(), CXorString("txÈ≠tjÈ¶v‰").ToCharArray(), CXorString("zTÏëdÒ±Qb˜ßs").ToCharArray());
	Offsets::m_hActiveWeapon = netVarManager.GetOffset(3, csplayer.ToCharArray(), baseclass.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÌÉtÏ¥r\\‡£gdÎ").ToCharArray());
	Offsets::m_hObserverTarget = netVarManager.GetOffset(2, csplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("zTÌçux‡∞an˜ñvy‚ßc").ToCharArray());
	Offsets::m_deadflag = netVarManager.GetOffset(3, csplayer.ToCharArray(), baseclass.ToCharArray(), CXorString("gg").ToCharArray(), CXorString("sn‰¶qg‰•").ToCharArray());
	Offsets::m_nTickBase = netVarManager.GetOffset(3, csplayer.ToCharArray(), baseclass.ToCharArray(), localdata.ToCharArray(), CXorString("zTÎñ~hÓÄvx‡").ToCharArray());

	//Offsets::m_AttributeManager = netVarManager.GetOffset(2, basecombatweapon.ToCharArray(), baseclass.ToCharArray(), "m_AttributeManager");
	//Offsets::m_Item = netVarManager.GetOffset(3, basecombatweapon.ToCharArray(), baseclass.ToCharArray(), "m_AttributeManager", "m_Item");
	Offsets::m_iItemDefinitionIndex = netVarManager.GetOffset(4, basecombatweapon.ToCharArray(), baseclass.ToCharArray(), CXorString("zTƒ∂cyÏ†b‡ève‰•ry").ToCharArray(), CXorString("zTÃ∂rf").ToCharArray(), CXorString("zTÏãcnËÜrmÏ¨~Ï≠yBÎ¶rs").ToCharArray());
	//Offsets::m_iItemDefinitionIndex = netVarManager.GetOffset(1, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	Offsets::m_iClip1 = netVarManager.GetOffset(1, basecombatweapon.ToCharArray(), CXorString("zTÏÅ{bıÛ").ToCharArray());
	Offsets::m_iClip2 = netVarManager.GetOffset(1, basecombatweapon.ToCharArray(), CXorString("zTÏÅ{bı").ToCharArray());
	Offsets::m_flNextPrimaryAttack = netVarManager.GetOffset(2, basecombatweapon.ToCharArray(), CXorString("[dÊ£{JÊ∂~}‡ïrjı≠yO‰∂v").ToCharArray(), CXorString("zT„ÆYn˝∂GyÏØvy¸Éc‰°|").ToCharArray());

	Offsets::m_fAccuracyPenalty = netVarManager.GetOffset(1, weaponcsbase.ToCharArray(), CXorString("zT„Éth∞vh¸íre‰Æcr").ToCharArray());
	Offsets::m_flPostponeFireReadyTime = netVarManager.GetOffset(1, weaponcsbase.ToCharArray(), CXorString("zT„ÆGdˆ∂gdÎßQb˜ßEn‰¶n_ÏØr").ToCharArray());

	Offsets::m_fThrowTime = netVarManager.GetOffset(1, basecsgrenade.ToCharArray(), CXorString("zT„ñyÍµCbËß").ToCharArray());

	// Target Selector
	this->m_targetSelector.Setup(this);
	this->m_targetSelector.SetMultipoint(false);
	this->m_targetSelector.SetVisibleMode(VISIBLEMODE_CANHIT);
	for (int i = 0; i < TARGET_HITBOX_COUNT; i++)
	{
		this->m_targetSelector.SetCheckHitbox(i, true);
	}

	// Setups
	this->m_ragebot.Setup();
	this->m_triggerbot.Setup();
	this->m_antiAim.Setup();
	this->m_bhop.Setup();
	this->m_esp.Setup();
	this->m_chams.Setup();
	this->m_misc.Setup();
	this->m_skinchanger.Setup();
	this->m_visuals.Setup();

	// Aimbot
	this->m_ragebot.SetEnabled(true);
	this->m_ragebot.SetAutoshoot(true);
	this->m_ragebot.SetAutoscope(true);
	this->m_ragebot.SetSilentAim(true);
	this->m_ragebot.SetAutoReload(true);
	this->m_ragebot.SetTargetCriteria(TARGETCRITERIA_VIEWANGLES);

	// Triggerbot
	this->m_triggerbot.SetEnabled(false);

	// Antiaim
	this->m_antiAim.SetEnabled(true);
	this->m_antiAim.SetPitchSetting(PITCHANTIAIM_DOWN);
	this->m_antiAim.SetYawSetting(YAWANTIAIM_REALRIGHTFAKELEFT);

	// Bhop
	this->m_bhop.SetEnabled(true);

	// ESP
	this->m_esp.SetEnabled(true);
	this->m_esp.SetDrawBoundingBox(true);
	this->m_esp.SetDrawNames(true);
	this->m_esp.SetDrawHealthBar(true);
	this->m_esp.SetDrawHealthNumber(true);
	this->m_esp.SetDrawArmorBar(false);
	this->m_esp.SetDrawOwnTeam(false);
	this->m_esp.SetDrawOwnModel(true);
	this->m_esp.SetDrawOnlySpotted(false);
	this->m_esp.SetDrawOutline(true);

	// Chams
	this->m_chams.SetEnabled(true);
	this->m_chams.SetRenderTeam(false);
	this->m_chams.SetRenderLocalplayer(true);
	this->m_chams.SetIgnoreZIndex(true);
	this->m_chams.SetFlatModels(false);

	// Misc
	this->m_misc.SetEnabled(true);
	this->m_misc.SetNoRecoil(true);
	this->m_misc.SetFakelag(false);
	this->m_misc.SetAutoStrafe(true);
	this->m_misc.SetNoScope(true);
	this->m_misc.SetAutoPistol(true);
	this->m_misc.SetShowSpectators(false);
	this->m_misc.SetShowOnlyMySpectators(false);
	this->m_misc.SetShowOnlyMyTeamSpectators(false);
	this->m_misc.SetDisablePostProcessing(true);
	this->m_misc.SetJumpScout(true);
	this->m_misc.SetSpamNameFix(false);

	// SkinChanger
	this->m_skinchanger.SetEnabled(true);
	// TODO: Config und sowas
	this->LoadSkinChangerConfig();

	// Visuals
	this->m_visuals.SetEnabled(true);

	this->m_visuals.SetCrosshair(true);
	this->m_visuals.SetHitmarker(true);
	this->m_visuals.SetNoSmoke(true);
	this->m_visuals.SetHandsDrawStyle(HANDSDRAWSTYLE_WIREFRAME);
	this->m_visuals.SetNoVisualRecoil(true);
	this->m_visuals.SetDrawLbyIndicator(true);

	this->m_visuals.SetNoFlash(true);
	this->m_visuals.SetFlashPercentage(0.f);

	this->m_visuals.SetThirdperson(false);
	this->m_visuals.SetThirdpersonDistance(150);

	this->m_visuals.SetFovChange(true);
	this->m_visuals.SetFovValue(110);
	this->m_visuals.SetFovChangeScoped(false);

	// Register Event Handlers
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("pjËßHe‡µzjı").ToCharArray(), false); // game_newmap
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("tx⁄•vf‡ùsbˆ°xeÎßt‡¶").ToCharArray(), false); // cs_game_disconnected
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("d|Ï∂tc⁄∂rjË").ToCharArray(), false); // switch_team
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄™byÒ").ToCharArray(), false); // player_hurt
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("gg‰ªry⁄¶rjÒ™").ToCharArray(), false); // player_death
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sTˆ∂vyÒ").ToCharArray(), false); // round_start
	m_pGameEventManager->AddListener(&m_gameEventListener, CXorString("ed¨sT‡¨s").ToCharArray(), false); // round_end

	// Initialize InputHandler
	m_inputHandler.RegisterKey(VK_DELETE, EVENT_BTN_DETACH);
	m_inputHandler.RegisterKey(VK_INSERT, EVENT_BTN_TOGGLEMENU);
	m_inputHandler.RegisterKey(VK_NUMPAD0, EVENT_BTN_THIRDPERSON);

	// Create GUI (Window + all controls)
	CButton* pBtn = new CButton(16, 64, 120, 45, "Detach");
	pBtn->SetButtonClickEventHandler(DetachBtnClick);

	CCheckbox* pAimbot = new CCheckbox(16, 16, 120, 32, "Aimbot", m_ragebot.GetEnabled());
	pAimbot->SetEventHandler(std::bind(&CRagebot::SetEnabled, &m_ragebot, std::placeholders::_1));

	CCheckbox* pSilentAim = new CCheckbox(16, 64, 120, 32, "Silent Aim", m_ragebot.GetSilentAim());
	pSilentAim->SetEventHandler(std::bind(&CRagebot::SetSilentAim, &m_ragebot, std::placeholders::_1));

	CCheckbox* pCheck = new CCheckbox(16, 112, 120, 32, "Autoshoot", m_ragebot.GetAutoshoot());
	pCheck->SetEventHandler(std::bind(&CRagebot::SetAutoshoot, &m_ragebot, std::placeholders::_1));

	CCheckbox* pAutoScope = new CCheckbox(16, 160, 120, 32, "Autoscope", m_ragebot.GetAutoscope());
	pAutoScope->SetEventHandler(std::bind(&CRagebot::SetAutoscope, &m_ragebot, std::placeholders::_1));

	CCheckbox* pCheck2 = new CCheckbox(160, 112, 120, 32, "Thirdperson", m_visuals.GetThirdperson());
	pCheck2->SetEventHandler(std::bind(&CVisuals::SetThirdperson, &m_visuals, std::placeholders::_1));
	m_pGuiThirdpersonCheckbox = pCheck2;

	CSelectbox* pSelectbox = new CSelectbox(16, 16, 100, 32);
	pSelectbox->AddOption(0, "Unspecified");
	pSelectbox->AddOption(1, "NoHands");
	pSelectbox->AddOption(2, "lol");
	pSelectbox->AddOption(3, "Just testing");
	pSelectbox->AddOption(4, "this is fun");
	pSelectbox->AddOption(5, "nicht");
	pSelectbox->AddOption(6, "SIEG");

	CCheckbox* pNoName = new CCheckbox(16, 160, 120, 32, "NoName", m_misc.GetNoName());
	pNoName->SetEventHandler(std::bind(&CMisc::NoName, &m_misc, std::placeholders::_1));

	// TODO: Groupbox -> "Antiaim" :D
	CSelectbox* pSelectPitchAntiaim = new CSelectbox(16, 64, 100, 32, "Pitch", this->AntiAim()->GetPitchSetting());
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_NONE, "None");
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_DOWN, "Down");
	pSelectPitchAntiaim->AddOption(PITCHANTIAIM_UP, "Up");
	pSelectPitchAntiaim->SetEventHandler(std::bind(&CAntiAim::SetPitchSetting, &m_antiAim, std::placeholders::_1));

	CSelectbox* pSelectYawAntiaim = new CSelectbox(16, 112, 100, 32, "Yaw", this->AntiAim()->GetYawSetting());
	pSelectYawAntiaim->AddOption(YAWANTIAIM_NONE, "None");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_REALLEFTFAKERIGHT, "REAL LEFT FAKE RIGHT");
	pSelectYawAntiaim->AddOption(YAWANTIAIM_REALRIGHTFAKELEFT, "REAL RIGHT FAKE LEFT");
	pSelectYawAntiaim->SetEventHandler(std::bind(&CAntiAim::SetYawSetting, &m_antiAim, std::placeholders::_1));

	CSelectbox* pSelectbox2 = new CSelectbox(304, 16, 128, 32);
	pSelectbox2->AddOption(HANDSDRAWSTYLE_NONE, "None");
	pSelectbox2->AddOption(HANDSDRAWSTYLE_NOHANDS, "NoHands");
	pSelectbox2->AddOption(HANDSDRAWSTYLE_WIREFRAME, "Wireframe");
	pSelectbox2->SetSelection(m_visuals.GetHandsDrawStyle());
	pSelectbox2->SetEventHandler(std::bind(&CVisuals::SetHandsDrawStyle, &m_visuals, std::placeholders::_1));

	CGroupbox* pGroupbox = new CGroupbox(16, 16, 292, 312, "ESP");
	CCheckbox* pDrawBoundingBox = new CCheckbox(4, 4, 128, 32, "Bounding Box", m_esp.GetDrawBoundingBox());
	CCheckbox* pDrawHealthbar = new CCheckbox(4, 52, 128, 32, "Health bar", m_esp.GetDrawHealthBar());
	CCheckbox* pDrawHealthnumber = new CCheckbox(4, 100, 128, 32, "Health number", m_esp.GetDrawHealthNumber());
	CCheckbox* pDrawArmorbar = new CCheckbox(4, 160, 128, 32, "Armor bar", m_esp.GetDrawArmorBar());
	CCheckbox* pDrawOwnTeam = new CCheckbox(4, 208, 128, 32, "Own team", m_esp.GetDrawOwnTeam());
	CCheckbox* pDrawOwnModel = new CCheckbox(4, 256, 128, 32, "Own model (3rd person)", m_esp.GetDrawOwnModel());
	CCheckbox* pDrawOnlySpotted = new CCheckbox(4, 304, 128, 32, "Only spotted", m_esp.GetDrawOnlySpotted());
	CCheckbox* pDrawOutline = new CCheckbox(136, 16, 128, 32, "Outlines", m_esp.GetDrawOutline());
	CCheckbox* pDrawNames = new CCheckbox(136, 64, 128, 32, "Names", m_esp.GetDrawNames());

	pDrawBoundingBox->SetEventHandler(std::bind(&CEsp::SetDrawBoundingBox, &m_esp, std::placeholders::_1));
	pDrawHealthbar->SetEventHandler(std::bind(&CEsp::SetDrawHealthBar, &m_esp, std::placeholders::_1));
	pDrawHealthnumber->SetEventHandler(std::bind(&CEsp::SetDrawHealthNumber, &m_esp, std::placeholders::_1));
	pDrawArmorbar->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, &m_esp, std::placeholders::_1));
	pDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, &m_esp, std::placeholders::_1));
	pDrawOwnModel->SetEventHandler(std::bind(&CEsp::SetDrawOwnModel, &m_esp, std::placeholders::_1));
	pDrawOnlySpotted->SetEventHandler(std::bind(&CEsp::SetDrawOnlySpotted, &m_esp, std::placeholders::_1));
	pDrawOutline->SetEventHandler(std::bind(&CEsp::SetDrawOutline, &m_esp, std::placeholders::_1));
	pDrawNames->SetEventHandler(std::bind(&CEsp::SetDrawNames, &m_esp, std::placeholders::_1));

	pGroupbox->AddChild(pDrawBoundingBox);
	pGroupbox->AddChild(pDrawHealthbar);
	pGroupbox->AddChild(pDrawHealthnumber);
	pGroupbox->AddChild(pDrawArmorbar);
	pGroupbox->AddChild(pDrawOwnTeam);
	pGroupbox->AddChild(pDrawOwnModel);
	pGroupbox->AddChild(pDrawOnlySpotted);
	pGroupbox->AddChild(pDrawOutline);
	pGroupbox->AddChild(pDrawNames);

	CCheckbox* pChamsDrawOwnTeam = new CCheckbox(304, 112, 128, 32, "Chams Own Team", m_chams.GetRenderTeam());
	CCheckbox* pChamsDrawOwnModel = new CCheckbox(304, 160, 128, 32, "Chams Own Model", m_chams.GetRenderLocalplayer());
	CCheckbox* pChamsIgnoreZ = new CCheckbox(304, 208, 128, 32, "Chams Ignore Z", m_chams.GetIgnoreZIndex());
	CCheckbox* pChamsFlatModels = new CCheckbox(304, 256, 128, 32, "Chams Flat Models", m_chams.GetFlatModels());

	pChamsDrawOwnTeam->SetEventHandler(std::bind(&CChams::SetRenderTeam, &m_chams, std::placeholders::_1));
	pChamsDrawOwnModel->SetEventHandler(std::bind(&CChams::SetRenderLocalplayer, &m_chams, std::placeholders::_1));
	pChamsIgnoreZ->SetEventHandler(std::bind(&CChams::SetIgnoreZIndex, &m_chams, std::placeholders::_1));
	pChamsFlatModels->SetEventHandler(std::bind(&CChams::SetFlatModels, &m_chams, std::placeholders::_1));

	CSlider* pSlider = new CSlider(16, 125, 200, 20);
	//pSlider->SetEventHandler(std::bind(&SliderUpdateValue, std::placeholders::_1));

	CSlider* pSlider2 = new CSlider(232, 16, 20, 200, 0.0f, SLIDER_ORIENTATION_VERTICAL, true);
	//pSlider2->SetEventHandler(std::bind(&SliderUpdateValue, std::placeholders::_1));

	CSlider* pSliderFlashAmnt = new CSlider(304, 64, 128, 32, m_visuals.GetFlashPercentage());
	pSliderFlashAmnt->SetEventHandler(std::bind(&CVisuals::SetFlashPercentage, &m_visuals, std::placeholders::_1));

	CSlider* pSliderFov = new CSlider(563, 5, 32, 350, m_visuals.GetFovValue(), SLIDER_ORIENTATION_VERTICAL, true, 1.0f, 180.0f);
	pSliderFov->SetEventHandler(std::bind(&CVisuals::SetFovValue, &m_visuals, std::placeholders::_1));

	CLabel* pLabelWip = new CLabel(0, 0, 600, 400, "[WIP]", RM_FONT_HEADER, LABEL_ORIENTATION_CENTER, Color(255, 255, 0, 0));

	CGroupbox* groupBox = new CGroupbox(16, 16, 400, 300, "This is a CGroupbox");
	CButton* pGroupBtn = new CButton();
	groupBox->AddChild(pGroupBtn);

	CTabContainer* pContainer = new CTabContainer();
	CTabPage* pPage1 = new CTabPage("Rage");
	CTabPage* pPage2 = new CTabPage("Legit");
	CTabPage* pPage3 = new CTabPage("Visuals");
	CTabPage* pPage4 = new CTabPage("Misc");
	CTabPage* pPage5 = new CTabPage("Skin Changer");
	CTabPage* pPage6 = new CTabPage("Config");

	pPage1->AddChild(pAimbot);
	pPage1->AddChild(pSilentAim);
	pPage1->AddChild(pCheck);
	pPage1->AddChild(pAutoScope);

	pPage2->AddChild(pLabelWip);

	/*pPage3->AddChild(pDrawBoundingBox);
	pPage3->AddChild(pDrawHealthbar);
	pPage3->AddChild(pDrawHealthnumber);
	pPage3->AddChild(pDrawArmorbar);
	pPage3->AddChild(pDrawOwnTeam);
	pPage3->AddChild(pDrawOwnModel);
	pPage3->AddChild(pDrawOnlySpotted);
	pPage3->AddChild(pDrawOutline);
	pPage3->AddChild(pDrawNames);*/
	pPage3->AddChild(pGroupbox);
	pPage3->AddChild(pCheck2);
	pPage3->AddChild(pSelectbox2);
	pPage3->AddChild(pSliderFlashAmnt);
	pPage3->AddChild(pSliderFov);
	pPage3->AddChild(pChamsDrawOwnTeam);
	pPage3->AddChild(pChamsDrawOwnModel);
	pPage3->AddChild(pChamsIgnoreZ);
	pPage3->AddChild(pChamsFlatModels);

	pPage4->AddChild(pSelectbox);
	pPage4->AddChild(pSelectPitchAntiaim);
	pPage4->AddChild(pSelectYawAntiaim);
	pPage4->AddChild(pNoName);

	pPage5->AddChild(groupBox);

	pPage6->AddChild(pBtn);
	pPage6->AddChild(new CColorPicker());
	pPage6->AddChild(pSlider);
	pPage6->AddChild(pSlider2);

	pContainer->AddChild(pPage1);
	pContainer->AddChild(pPage2);
	pContainer->AddChild(pPage3);
	pContainer->AddChild(pPage4);
	pContainer->AddChild(pPage5);
	pContainer->AddChild(pPage6);
	pContainer->SelectTab(0);

	m_pWindow = new CWindow(30, 30, 600, 400, ".mechanics");
	m_pWindow->AddChild(pContainer);

	// Wait for the game to be ingame before hooking
	while (!m_pEngineClient->IsInGame()) {
		Sleep(100);
	}
}

void CApplication::Hook()
{
	// IClientMode
	DWORD dwClientMode = (DWORD)(**(DWORD***)((*(DWORD**)(m_pClient))[10] + 0x5));
	// CInput
	this->m_pInput = *(CInput**)((*(DWORD**)(m_pClient))[15] + 0x1);

	// ClientState
	this->m_pClientState = **(IClientState***)(CPattern::FindPattern(
		(BYTE*)this->EngineDll(),
		ENGINEDLL_SIZE,
		(BYTE*)"\xA1\x00\x00\x00\x00\x8B\x80\x00\x00\x00\x00\xC3",
		"a----gh----e"
	) + 1);

	// IViewRender
	m_pViewRender = **(IViewRender***)(CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\xFF\x75\x10\x8B\x0D\x00\x00\x00\x00\xFF\x75\x0C",
		"abcdefgh----ijk"
	) + 0x08);

	// KeyValues::Init
	DWORD dwInitKeyValuesTemp = (CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56",
		"a----bbb----ccccccc-----ddd"
	) + 7);
	m_pInitKeyValues = (InitKeyValues_t)(dwInitKeyValuesTemp + (*(DWORD_PTR*)(dwInitKeyValuesTemp + 1)) + 5);

	// KeyValues::LoadFromBuffer
	DWORD dwLoadFromBufferTemp = CPattern::FindPattern(
		(BYTE*)this->m_dwClientDll,
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89\x4C\x24\x04",
		"abcdefghijjjjjjjq"
	);
	m_pLoadFromBuffer = (LoadFromBuffer_t)dwLoadFromBufferTemp;

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
	m_pRenderSmokePostViewmodel = (RenderSmokePostViewmodel_t)m_pViewRenderHook->Hook(41, (DWORD*)hk_RenderSmokePostViewmodel);

	CXorString xorBaseViewModel("TI‰±r]Ïß`FÍ¶rg");
	CXorString xorSequence("zTÎërzßyh‡");
	// Search for the 'CBaseViewModel' class.
	for (ClientClass* pClass = this->m_pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
		if (!strcmp(pClass->m_pNetworkName, xorBaseViewModel.ToCharArray())) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, xorSequence.ToCharArray()))
					continue;

				// Store the original proxy function.
				this->m_pSequenceProxy = (RecvVarProxy_t)pProp->m_ProxyFn;

				// Replace the proxy function with our sequence changer.
				m_pProxyProp = pProp;
				m_pProxyProp->m_ProxyFn = (RecvVarProxy_t)CApplication::hk_SetViewModelSequence;

				break;
			}

			break;
		}
	}

	m_misc.SetClanTag(".mechanics");

	this->m_bIsHooked = true;
	this->m_bInitialHookDone = true;
}

// Singleton
CApplication::CApplication()
{
	// TODO: Der konstruktor muss *ALLE* pointer auf NULL setzen, der destruktor
	//		 alle ptr != NULL lˆschen
	m_pWindow = NULL;

	m_pClientModeHook = NULL;
	m_pModelRenderHook = NULL;
	m_pClientHook = NULL;
	m_pPanelHook = NULL;
	m_pGameEventManagerHook = NULL;

	m_bGotSendPackets = false;

	m_bInitialHookDone = false;
	m_bIsHooked = false;
}

CApplication::CApplication(CApplication const&)
{
}

CApplication::~CApplication()
{
	if (m_pGameEventManagerHook)
		delete m_pGameEventManagerHook;

	if (m_pPanelHook)
		delete m_pPanelHook;

	if (m_pClientHook)
		delete m_pClientHook;

	if (m_pModelRenderHook)
		delete m_pModelRenderHook;

	if (m_pClientModeHook)
		delete m_pClientModeHook;

	if (m_pWindow)
		delete m_pWindow;
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

	vecViewForward.Normalize();
	vecViewRight.Normalize();
	vecViewUp.Normalize();

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