#include "Menu.h"
#include "../Application.h"

CMenu::CMenu()
	: m_pInputHandler(CInputHandler::Instance())
{
	m_pWindow = NULL;
	m_pWindowMirror = NULL;
}

CMenu::~CMenu()
{
	if (m_pWindowMirror)
		delete m_pWindowMirror;

	if (m_pWindow)
		delete m_pWindow;
}

CMenu* CMenu::Instance()
{
	static CMenu inst;
	return &inst;
}

void CMenu::Init(CApplication* pApp)
{
	m_pApp = pApp;

	// Initialize InputHandler
	m_pInputHandler->RegisterKey(VK_DELETE, EVENT_BTN_DETACH);
	m_pInputHandler->RegisterKey(VK_INSERT, EVENT_BTN_TOGGLEMENU);
	m_pInputHandler->RegisterKey(VK_NUMPAD0, EVENT_BTN_THIRDPERSON);
	m_pInputHandler->RegisterKey(VK_XBUTTON2, EVENT_BTN_SWITCHREALFAKE);

	m_pInputHandler->RegisterKey(VK_XBUTTON1, EVENT_BTN_TRIGGERKEY);
}

void CMenu::CreateMenu()
{
	this->CreateRageTab();
	this->CreateLegitTab();
	this->CreateVisualsTab();
	this->CreateMiscTab();
	this->CreateSkinChangerTab();
	this->CreateConfigTab();

	m_pMainContainer = new CTabContainer();
	m_pMainContainer->AddChild(m_pRageTab);
	m_pMainContainer->AddChild(m_pLegitTab);
	m_pMainContainer->AddChild(m_pVisualsTab);
	m_pMainContainer->AddChild(m_pMiscTab);
	m_pMainContainer->AddChild(m_pSkinChangerTab);
	m_pMainContainer->AddChild(m_pConfigTab);
	//m_pMainContainer->SelectTab(0);

	m_pWindow = new CWindow(30, 30, 692, 400, ".mechanics");
	m_pWindow->AddChild(m_pMainContainer);

	m_pWindowMirror = new CWindow(100, 100, MIRROR_WIDTH, MIRROR_HEIGHT, "Mirror");
	m_pWindowMirror->SetVisible(false);
}

void CMenu::ApplySettings()
{
	m_pAimbotEnabled->SetChecked(m_pApp->Ragebot()->GetEnabled());
	m_pAimbotSilentAim->SetChecked(m_pApp->Ragebot()->GetSilentAim());
	m_pAimbotAutoshoot->SetChecked(m_pApp->Ragebot()->GetAutoshoot());
	m_pAimbotAutoscope->SetChecked(m_pApp->Ragebot()->GetAutoscope());
	m_pAimbotAutoReload->SetChecked(m_pApp->Ragebot()->GetAutoReload());
	m_pAimbotMultipoint->SetChecked(m_pApp->Ragebot()->GetMultipoint());
	m_pAimbotMultipointScale->SetValue(m_pApp->Ragebot()->GetMultipointScale());
	m_pAimbotHitchanceEnabled->SetChecked(m_pApp->Ragebot()->GetCalculateHitchance());
	m_pAimbotHitchanceSlider->SetValue(m_pApp->Ragebot()->GetHitchance());
	m_pAimbotTargetCriteria->SetValue(m_pApp->Ragebot()->GetTargetCriteria());
	m_pAimbotVisibleMode->SetValue(m_pApp->Ragebot()->GetVisibleMode());

	m_pRageNoRecoilEnabled->SetChecked(m_pApp->GunAccuracy()->GetNoRecoil());
	m_pRageNoSpreadEnabled->SetChecked(m_pApp->GunAccuracy()->GetNoSpread());

	m_pRageHitboxHead->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_HEAD));
	m_pRageHitboxChest->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_CHEST));
	m_pRageHitboxPelvis->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
	m_pRageHitboxRForearm->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
	m_pRageHitboxLForearm->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
	m_pRageHitboxRCalf->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
	m_pRageHitboxLCalf->SetChecked(m_pApp->Ragebot()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

	m_pRageOthersAutoZeusEnabled->SetChecked(m_pApp->Ragebot()->GetAutoZeus());
	m_pRageOthersAutoRevolverEnabled->SetChecked(m_pApp->Ragebot()->GetAutoRevolver());
	m_pRageOthersLagCompensationEnabled->SetChecked(m_pApp->LagCompensation()->GetRageLagCompensationEnabled());
	m_pRageOthersResolverType->SetValue(m_pApp->Resolver()->GetResolverType());

	m_pAntiaimEnabled->SetChecked(m_pApp->AntiAim()->GetEnabled());
	// Standing
	m_pAntiaimStandingPitch->SetValue(m_pApp->AntiAim()->GetPitchSettingStanding());
	m_pAntiaimStandingYaw->SetValue(m_pApp->AntiAim()->GetYawSettingStanding());
	m_pAntiaimStandingYawOffset->SetValue(m_pApp->AntiAim()->GetYawOffsetStanding());
	m_pAntiaimStandingYawFake->SetSelection(m_pApp->AntiAim()->GetYawFakeSettingStanding());
	m_pAntiaimYawStandingFakeOffset->SetValue(m_pApp->AntiAim()->GetYawFakeOffsetStanding());
	// Moving
	m_pAntiaimMovingPitch->SetValue(m_pApp->AntiAim()->GetPitchSettingMoving());
	m_pAntiaimMovingYaw->SetValue(m_pApp->AntiAim()->GetYawSettingMoving());
	m_pAntiaimMovingYawOffset->SetValue(m_pApp->AntiAim()->GetYawOffsetMoving());
	m_pAntiaimMovingYawFake->SetSelection(m_pApp->AntiAim()->GetYawFakeSettingMoving());
	m_pAntiaimYawMovingFakeOffset->SetValue(m_pApp->AntiAim()->GetYawFakeOffsetMoving());

	m_pAntiaimLbyIndicator->SetChecked(m_pApp->AntiAim()->GetDrawLbyIndicator());
	m_pAntiaimStandingLbyBreaker->SetChecked(m_pApp->AntiAim()->GetLbyBreaker());

	m_pAntiaimEdgeAntiaimEnabled->SetChecked(m_pApp->AntiAim()->GetDoEdgeAntiAim());
	m_pAntiaimEdgeAntiaimCheckOffset->SetValue(m_pApp->AntiAim()->GetEdgeAntiAimCheckPointsAmount());
	m_pAntiaimEdgeAntiaimDrawPoints->SetChecked(m_pApp->AntiAim()->GetDrawEdgeAntiAimPoints());
	m_pAntiaimEdgeAntiaimDrawLines->SetChecked(m_pApp->AntiAim()->GetDrawEdgeAntiAimLines());

	// Legit
	// Legitbot
	m_pLegitbotEnabled->SetChecked(m_pApp->Legitbot()->GetEnabled());
	m_pLegitbotAutoshoot->SetChecked(m_pApp->Legitbot()->GetAutoshoot());
	m_pLegitbotHelpAfterShots->SetValue(m_pApp->Legitbot()->GetHelpAfterShots());
	m_pLegitbotTimeToAim->SetValue(m_pApp->Legitbot()->GetTimeToAim());
	m_pLegitbotCurve->SetValue(m_pApp->Legitbot()->GetCurve());
	m_pLegitbotFov->SetValue(m_pApp->Legitbot()->GetFieldOfView());
	m_pLegitbotPointScale->SetValue(m_pApp->Legitbot()->GetPointScale());
	m_pLegitbotDrawPath->SetChecked(m_pApp->Legitbot()->GetDrawPath());
	m_pLegitbotPathColor->SetValue(m_pApp->Legitbot()->GetPathColor());

	// Legitbot hitboxes
	m_pLegitHitboxHead->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_HEAD));
	m_pLegitHitboxChest->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_CHEST));
	m_pLegitHitboxPelvis->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
	m_pLegitHitboxRForearm->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
	m_pLegitHitboxLForearm->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
	m_pLegitHitboxRCalf->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
	m_pLegitHitboxLCalf->SetChecked(m_pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

	// Triggerbot
	m_pTriggerbotEnabled->SetChecked(m_pApp->Triggerbot()->GetEnabled());
	m_pTriggerbotKey->SetKey(m_pApp->Triggerbot()->GetTriggerKey());
	m_pTriggerbotDelayValue->SetValue(m_pApp->Triggerbot()->GetShootDelay());
	m_pTriggerbotDelayJitterValue->SetValue(m_pApp->Triggerbot()->GetShootDelayJitter());
	m_pTriggerbotBurst->SetChecked(m_pApp->Triggerbot()->GetTriggerBurst());
	m_pTriggerbotMinShots->SetValue(m_pApp->Triggerbot()->GetMinShots());
	m_pTriggerbotMaxShots->SetValue(m_pApp->Triggerbot()->GetMaxShots());

	// Legit Lag Compensation
	m_pLegitLagCompensationEnabled->SetChecked(m_pApp->LagCompensation()->GetLegitLagCompensationEnabled());
	m_pLegitLagCompensationDuration->SetValue(m_pApp->LagCompensation()->GetLegitLagCompensationDuration());

	// Esp
	m_pEspEnabled->SetChecked(m_pApp->Esp()->GetEnabled());
	m_pEspDrawBoundingBox->SetSelection(m_pApp->Esp()->GetDrawBoundingBox());
	m_pEspDrawFilledBox->SetChecked(m_pApp->Esp()->GetFillBoundingBox());
	m_pEspDrawOutline->SetChecked(m_pApp->Esp()->GetDrawOutline());
	m_pEspDrawHealthbar->SetChecked(m_pApp->Esp()->GetDrawHealthBar());
	m_pEspDrawHealthnumber->SetChecked(m_pApp->Esp()->GetDrawHealthNumber());
	m_pEspDrawAmmonumber->SetChecked(m_pApp->Esp()->GetDrawAmmoNumber());
	m_pEspDrawAmmobar->SetChecked(m_pApp->Esp()->GetDrawAmmoBar());
	m_pEspDrawArmorbar->SetChecked(m_pApp->Esp()->GetDrawArmorBar());
	m_pEspDrawSkeletonEnabled->SetChecked(m_pApp->Esp()->GetDrawSkeleton());
	m_pEspDrawOwnTeam->SetChecked(m_pApp->Esp()->GetDrawOwnTeam());
	m_pEspDrawOwnModel->SetChecked(m_pApp->Esp()->GetDrawOwnModel());
	m_pEspFadeoutEnabled->SetChecked(m_pApp->Esp()->GetFadeoutEnabled());
	m_pEspFadeoutValue->SetValue(m_pApp->Esp()->GetFadeoutTime());
	m_pEspDrawOnlyVisible->SetChecked(m_pApp->Esp()->GetDrawOnlyVisible());
	m_pEspDrawOnlySpotted->SetChecked(m_pApp->Esp()->GetDrawOnlySpotted());
	m_pEspDrawActiveWeapon->SetChecked(m_pApp->Esp()->GetDrawActiveWeapon());
	m_pEspDrawNames->SetChecked(m_pApp->Esp()->GetDrawNames());
	m_pEspBarrelEnabled->SetChecked(m_pApp->Esp()->GetDrawViewangles());
	m_pEspBarrelValue->SetValue(m_pApp->Esp()->GetViewanglesLength());

	// WeaponEsp
	m_pWeaponEspEnabled->SetChecked(m_pApp->WeaponEsp()->GetEnabled());
	m_pWeaponEspWeaponNameEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawWeaponName());
	m_pWeaponEspWeaponBoundingBoxEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawWeaponBoundingBox());
	m_pWeaponEspGrenadeNameEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawGrenadeName());
	m_pWeaponEspGrenadeBoundingBoxEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawGrenadeBoundingBox());
	m_pWeaponEspBombNameEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawBombName());
	m_pWeaponEspBombBoundingBoxEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawBombBoundingBox());
	m_pWeaponEspBombTimerEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawBombTimer());
	m_pWeaponEspBombDefuseTimerEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawBombDefuseTimer());
	m_pWeaponEspBombDamageIndicatorEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawBombDamageIndicator());

	// Chams
	m_pChamsEnabled->SetChecked(m_pApp->Chams()->GetEnabled());
	m_pChamsStyle->SetSelection(m_pApp->Chams()->GetModelStyle());
	m_pChamsDrawOwnTeam->SetChecked(m_pApp->Chams()->GetRenderTeam());
	m_pChamsDrawOwnModel->SetChecked(m_pApp->Chams()->GetRenderLocalplayer());
	m_pChamsIgnoreZ->SetChecked(m_pApp->Chams()->GetOnlyVisible());
	m_pChamsFakeAngle->SetChecked(m_pApp->Chams()->GetRenderFakeAngle());
	m_pChamsWeaponChams->SetSelection(m_pApp->Chams()->GetWeaponChamsStyle());
	m_pChamsColorVisibleCT->SetValue(m_pApp->Chams()->GetColorVisibleCT());
	m_pChamsColorHiddenCT->SetValue(m_pApp->Chams()->GetColorHiddenCT());
	m_pChamsColorVisibleT->SetValue(m_pApp->Chams()->GetColorVisibleT());
	m_pChamsColorHiddenT->SetValue(m_pApp->Chams()->GetColorHiddenT());

	// Sound ESP
	m_pSoundEspEnabled->SetChecked(m_pApp->SoundEsp()->GetEnabled());
	m_pSoundEspShowTime->SetValue(m_pApp->SoundEsp()->GetShowTime());
	m_pSoundEspFadeoutEnabled->SetChecked(m_pApp->SoundEsp()->GetFadeoutEnabled());
	m_pSoundEspFadeoutTime->SetValue(m_pApp->SoundEsp()->GetFadeoutTime());
	m_pSoundEspDrawOwnTeam->SetChecked(m_pApp->SoundEsp()->GetDrawOwnTeam());
	m_pSoundEspOnlyNotVisible->SetChecked(m_pApp->SoundEsp()->GetDrawOnlyNotVisible());

	// LC
	m_pDrawLagCompensationStyle->SetSelection(m_pApp->LagCompensation()->GetDrawStyle());
	m_pDrawLagCompensationFrequency->SetValue(m_pApp->LagCompensation()->GetDrawFrequency());
	m_pDrawLagCompensationOnlyVisible->SetChecked(m_pApp->LagCompensation()->GetDrawOnlyVisible());

	m_pEffectsNoVisualRecoil->SetChecked(m_pApp->Visuals()->GetNoVisualRecoil());
	m_pEffectsNoSmoke->SetChecked(m_pApp->Visuals()->GetNoSmoke());
	m_pEffectsNoScope->SetChecked(m_pApp->Visuals()->GetNoScope());
	m_pEffectsDisablePostprocessing->SetChecked(m_pApp->Visuals()->GetDisablePostProcessing());
	m_pEffectsNoFlash->SetChecked(m_pApp->Visuals()->GetNoFlash());
	m_pEffectsNoFlashValue->SetValue(m_pApp->Visuals()->GetFlashPercentage());

	m_pVisualsOthersHandsDrawStyle->SetValue(m_pApp->Visuals()->GetHandsDrawStyle());
	m_pVisualsOthersHitmarkerEnabled->SetChecked(m_pApp->GunHud()->GetHitmarker());
	m_pVisualsOthersHitmarkerSoundEnabled->SetChecked(m_pApp->GunHud()->GetHitmarkerSound());
	m_pVisualsOthersHitmarkerHitpointEnabled->SetChecked(m_pApp->GunHud()->GetHitmarkerHitpoint());
	m_pVisualsOthersCrosshairEnabled->SetChecked(m_pApp->GunHud()->GetCrosshair());
	m_pVisualsOthersRecoilCrosshairEnabled->SetChecked(m_pApp->GunHud()->GetCrosshairShowRecoil());
	m_pVisualsOthersSpreadConeEnabled->SetChecked(m_pApp->GunHud()->GetSpreadCone());
	m_pVisualsOthersRecoilSpreadConeEnabled->SetChecked(m_pApp->GunHud()->GetSpreadConeShowRecoil());
	m_pVisualsOtherSpreadConeStyle->SetSelection(m_pApp->GunHud()->GetSpreadConeStyle());
	m_pVisualsOtherSpreadConeColor->SetValue(m_pApp->GunHud()->GetSpreadConeColor());

	m_pVisualsOthersNightmode->SetValue(m_pApp->MaterialVisuals()->GetNightmodeValue());
	m_pVisualsOthersAsuswalls->SetValue(m_pApp->MaterialVisuals()->GetAsuswallsValue());
	m_pVisualsOthersSkychanger->SetSelection(m_pApp->MaterialVisuals()->GetSkychangerValue());

	m_pVisualsOthersThirdperson->SetChecked(m_pApp->Visuals()->GetThirdperson());
	m_pVisualsOthersThirdpersonDistance->SetValue(m_pApp->Visuals()->GetThirdpersonDistance());
	m_pVisualsOthersMirror->SetChecked(m_pApp->Mirror()->GetEnabled());

	m_pFovChangerFovEnabled->SetChecked(m_pApp->Visuals()->GetFovChange());
	m_pFovChangerFovScopeEnabled->SetChecked(m_pApp->Visuals()->GetFovChangeScoped());
	m_pFovChangerFovValue->SetValue(m_pApp->Visuals()->GetFovValue());
	m_pFovChangerViewmodelFovEnabled->SetChecked(m_pApp->Visuals()->GetViewmodelFov());
	m_pFovChangerViewmodelFovValue->SetValue(m_pApp->Visuals()->GetViewmodelFovValue());

	m_pBulletTracerEnabled->SetChecked(m_pApp->Visuals()->GetBulletTracer());
	m_pBulletTracerSelf->SetChecked(m_pApp->Visuals()->GetBulletTracerSelf());
	m_pBulletTracerTeam->SetChecked(m_pApp->Visuals()->GetBulletTracerTeam());

	// Misc
	m_pFakelagEnabled->SetChecked(m_pApp->Fakelag()->GetEnabled());
	m_pFakelagOnlyInAir->SetChecked(m_pApp->Fakelag()->GetOnlyInAir());
	m_pFakelagChokeAmount->SetValue(m_pApp->Fakelag()->GetChokeAmount());
	m_pFakelagType->SetValue(m_pApp->Fakelag()->GetLagType());

	m_pMiscOthersNoRecoilEnabled->SetChecked(m_pApp->Misc()->GetNoRecoil());
	m_pMiscOthersAutoPistolEnabled->SetChecked(m_pApp->Misc()->GetAutoPistol());
	m_pMiscOthersJumpScoutEnabled->SetChecked(m_pApp->Misc()->GetJumpScout());
	m_pMiscOthersAutoAcceptEnabled->SetChecked(m_pApp->Misc()->GetAutoAccept());
	m_pMiscOthersNoNameEnabled->SetChecked(m_pApp->Misc()->GetNoName());

	m_pMiscBhopEnabled->SetChecked(m_pApp->Bhop()->GetEnabled());
	m_pMiscBhopAutoStrafeMode->SetSelection(m_pApp->Bhop()->GetAutoStrafeMode());
	m_pMiscBhopCircleStrafeEnabled->SetChecked(m_pApp->Bhop()->GetCircleStrafe());
	m_pMiscBhopCircleStrafeDirection->SetSelection(m_pApp->Bhop()->GetCircleStrafeStartDirection());

	// TODO
	// SkinChanger
	m_pSkinChangerOnlyMyWeapon->SetChecked(m_pApp->SkinChanger()->GetOnlyMyWeapons());
	m_pSkinChangerKnife->SetSelection(0);
	m_pSkinChangerWeapon->SetSelection(0);
	m_pSkinChangerSkin->SetSelection(0);
	m_pSkinChangerGlove->SetSelection(0);
	m_pSkinChangerWeaponName->SetText("");
	m_pSkinChangerWeaponStattrakCount->SetValue(-1);
	m_pSkinChangerWeaponSeed->SetValue(0);
	m_pSkinChangerWeaponWear->SetValue(0.01f);
}

void CMenu::HandleInput()
{
	// Input handling
	bool bVis = m_pWindow->GetVisible();
	while (m_pInputHandler->GetInput(&m_inputEvent, bVis))
	{
		if (m_inputEvent.eventType == EVENT_TYPE_KEYBOARD)
		{
			// Implement key sensitive features here (aimkey, triggerkey, etc)
			if (!bVis)
			{
				if (m_inputEvent.button == EVENT_BTN_AIMKEY)
				{
					// TODO
				}
				else if (m_inputEvent.button == EVENT_BTN_TRIGGERKEY)
				{
					if (m_inputEvent.buttonDirection == EVENT_BTNDIR_DOWN)
						m_pApp->Triggerbot()->TriggerKeyDown();
					else
						m_pApp->Triggerbot()->TriggerKeyUp();
				}
			}

			if (m_inputEvent.button == EVENT_BTN_TOGGLEMENU &&
				m_inputEvent.buttonDirection == EVENT_BTNDIR_DOWN)
			{
				m_pWindow->SetVisible(!bVis);

				// Input & mouse stuff
				m_pApp->Gui()->SetEnableGameInput(bVis);
				m_pApp->Gui()->SetDrawMouse(!bVis);
			}
			else if (m_inputEvent.button == EVENT_BTN_DETACH &&
				m_inputEvent.buttonDirection == EVENT_BTNDIR_DOWN)
			{
				m_pApp->Detach();
			}
			else if (m_inputEvent.button == EVENT_BTN_THIRDPERSON &&
				m_inputEvent.buttonDirection == EVENT_BTNDIR_UP)
			{
				bool bNewValue = !m_pApp->Visuals()->GetThirdperson();

				m_pVisualsOthersThirdperson->SetChecked(bNewValue);
			}
			else if (m_inputEvent.button == EVENT_BTN_SWITCHREALFAKE &&
				m_inputEvent.buttonDirection == EVENT_BTNDIR_UP)
			{
				float fOffsetSwap = m_pApp->AntiAim()->GetYawOffsetStanding();

				m_pAntiaimStandingYawOffset->SetValue(m_pApp->AntiAim()->GetYawFakeOffsetStanding());
				m_pAntiaimYawStandingFakeOffset->SetValue(fOffsetSwap);
			}
		}

		if (m_pWindow->GetVisible())
		{
			m_pWindow->ProcessEvent(&m_inputEvent);
			m_pWindowMirror->ProcessEvent(&m_inputEvent);
		}
	}
}

void CMenu::Draw(ISurface* pSurface)
{
	m_pWindow->Draw(pSurface);
	m_pApp->Gui()->DrawMouse(pSurface);
}

void CMenu::CreateRageTab()
{
	m_pAimbotEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pAimbotEnabled->SetEventHandler(std::bind(&CRagebot::SetEnabled, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotEnabled->SetTooltipText("Enables/Disables the Aimbot entirely");

	m_pAimbotSilentAim = new CCheckbox(4, 20, 128, 16, "Silent Aim");
	m_pAimbotSilentAim->SetEventHandler(std::bind(&CRagebot::SetSilentAim, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotSilentAim->SetTooltipText("Toggles wether or not the movement of the aimbot is visible to you");
	m_pAimbotSilentAim->AddDependency(m_pAimbotEnabled);

	m_pAimbotAutoshoot = new CCheckbox(4, 40, 128, 16, "Auto Shoot");
	m_pAimbotAutoshoot->SetEventHandler(std::bind(&CRagebot::SetAutoshoot, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotAutoshoot->SetTooltipText("Automatically shoot as soon as possible (by configuration)");
	m_pAimbotAutoshoot->AddDependency(m_pAimbotEnabled);

	m_pAimbotAutoscope = new CCheckbox(4, 60, 128, 16, "Auto Scope");
	m_pAimbotAutoscope->SetEventHandler(std::bind(&CRagebot::SetAutoscope, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotAutoscope->SetTooltipText("Scope with sniper rifles before shooting");
	m_pAimbotAutoscope->AddDependency(m_pAimbotEnabled);

	m_pAimbotAutoReload = new CCheckbox(4, 80, 128, 16, "Auto Reload");
	m_pAimbotAutoReload->SetEventHandler(std::bind(&CRagebot::SetAutoReload, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotAutoReload->SetTooltipText("Automatically reload as soon as you're out of ammo");
	m_pAimbotAutoReload->AddDependency(m_pAimbotEnabled);

	m_pAimbotMultipoint = new CCheckbox(4, 100, 128, 16, "Multipoint");
	m_pAimbotMultipoint->SetEventHandler(std::bind(&CRagebot::SetMultipoint, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotMultipoint->SetTooltipText("If checked, the aimbot will check 9 different points of the hitbox, instead of the middle");
	m_pAimbotMultipoint->AddDependency(m_pAimbotEnabled);

	m_pAimbotMultipointScale = new CSlider(4, 122, 128, 16, 0.01f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 1.0f);
	m_pAimbotMultipointScale->SetEventHandler(std::bind(&CRagebot::SetMultipointScale, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotMultipointScale->SetTooltipText("Defines how far the points are away from the middle (0 = middle, 1 = most outter)");
	m_pAimbotMultipointScale->AddDependency(m_pAimbotEnabled);
	m_pAimbotMultipointScale->AddDependency(m_pAimbotMultipoint);

	m_pRageNoRecoilEnabled = new CCheckbox(4, 142, 64, 16, "Remove Recoil");
	m_pRageNoRecoilEnabled->SetEventHandler(std::bind(&CGunAccuracy::SetNoRecoil, m_pApp->GunAccuracy(), std::placeholders::_1));
	m_pRageNoRecoilEnabled->SetTooltipText("Removes gun recoil (not visible to you)");

	m_pRageNoSpreadEnabled = new CCheckbox(72, 142, 64, 16, "Remove Spread");
	m_pRageNoSpreadEnabled->SetEventHandler(std::bind(&CGunAccuracy::SetNoSpread, m_pApp->GunAccuracy(), std::placeholders::_1));
	m_pRageNoSpreadEnabled->SetTooltipText("Removes gun spread (not visible to you), but only on nospread servers!");

	m_pAimbotHitchanceEnabled = new CCheckbox(4, 162, 128, 16, "Hitchance");
	m_pAimbotHitchanceEnabled->SetEventHandler(std::bind(&CRagebot::SetCalculateHitchance, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotHitchanceEnabled->SetTooltipText("Check how high the percentage is of hitting your enemy");
	m_pAimbotHitchanceEnabled->AddDependency(m_pAimbotEnabled);

	m_pAimbotHitchanceSlider = new CSlider(4, 184, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f);
	m_pAimbotHitchanceSlider->SetEventHandler(std::bind(&CRagebot::SetHitchance, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotHitchanceSlider->SetTooltipText("Define at which minimum percentage to shoot");
	m_pAimbotHitchanceSlider->AddDependency(m_pAimbotEnabled);
	m_pAimbotHitchanceSlider->AddDependency(m_pAimbotHitchanceEnabled);

	m_pAimbotTargetCriteria = new CSelectbox(4, 214, 128, 20, "Target criteria");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_UNSPECIFIED, "First found");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_ORIGIN, "Closest to position");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_VIEWANGLES, "Closest to crosshair");
	m_pAimbotTargetCriteria->SetEventHandler(std::bind(&CRagebot::SetTargetCriteria, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotTargetCriteria->SetTooltipText("Specify how the aimbot target is selected");
	m_pAimbotTargetCriteria->AddDependency(m_pAimbotEnabled);

	m_pAimbotVisibleMode = new CSelectbox(4, 252, 128, 20, "Visible mode");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_IGNORE, "Ignore");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_CANHIT, "Can Hit (Autowall)");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_FULLVISIBLE, "Full Visible");
	m_pAimbotVisibleMode->SetEventHandler(std::bind(&CRagebot::SetVisibleMode, m_pApp->Ragebot(), std::placeholders::_1));
	m_pAimbotVisibleMode->SetTooltipText("Additional filter for targets (shoot whenever, as soon as you can do damage, etc)");
	m_pAimbotVisibleMode->AddDependency(m_pAimbotEnabled);

	m_pAimbotGroup = new CGroupbox(16, 16, 152, 308, "Aimbot");
	m_pAimbotGroup->AddChild(m_pAimbotEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotSilentAim);
	m_pAimbotGroup->AddChild(m_pAimbotAutoshoot);
	m_pAimbotGroup->AddChild(m_pAimbotAutoscope);
	m_pAimbotGroup->AddChild(m_pAimbotAutoReload);
	m_pAimbotGroup->AddChild(m_pAimbotMultipoint);
	m_pAimbotGroup->AddChild(m_pAimbotMultipointScale);
	m_pAimbotGroup->AddChild(m_pRageNoRecoilEnabled);
	m_pAimbotGroup->AddChild(m_pRageNoSpreadEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotHitchanceEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotHitchanceSlider);
	m_pAimbotGroup->AddChild(m_pAimbotTargetCriteria);
	m_pAimbotGroup->AddChild(m_pAimbotVisibleMode);

	m_pRageHitboxHead = new CCheckbox(4, 0, 128, 16, "Head");
	m_pRageHitboxHead->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_HEAD, std::placeholders::_1));
	m_pRageHitboxHead->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxChest = new CCheckbox(4, 20, 128, 16, "Chest");
	m_pRageHitboxChest->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_CHEST, std::placeholders::_1));
	m_pRageHitboxChest->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxPelvis = new CCheckbox(4, 40, 128, 16, "Pelvis");
	m_pRageHitboxPelvis->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_PELVIS, std::placeholders::_1));
	m_pRageHitboxPelvis->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxRForearm = new CCheckbox(4, 60, 128, 16, "Right forearm");
	m_pRageHitboxRForearm->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_RIGHT_FOREARM, std::placeholders::_1));
	m_pRageHitboxRForearm->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxLForearm = new CCheckbox(4, 80, 128, 16, "Left forearm");
	m_pRageHitboxLForearm->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_LEFT_FOREARM, std::placeholders::_1));
	m_pRageHitboxLForearm->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxRCalf = new CCheckbox(4, 100, 128, 16, "Right calf");
	m_pRageHitboxRCalf->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_RIGHT_CALF, std::placeholders::_1));
	m_pRageHitboxRCalf->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxLCalf = new CCheckbox(4, 120, 128, 16, "Left calf");
	m_pRageHitboxLCalf->SetEventHandler(std::bind(&CRagebot::SetCheckHitbox, m_pApp->Ragebot(), TARGET_HITBOX_LEFT_CALF, std::placeholders::_1));
	m_pRageHitboxLCalf->AddDependency(m_pAimbotEnabled);

	m_pRageHitboxGroup = new CGroupbox(184, 16, 152, 154, "Hitboxes");
	m_pRageHitboxGroup->AddChild(m_pRageHitboxHead);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxChest);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxPelvis);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxRForearm);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxLForearm);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxRCalf);
	m_pRageHitboxGroup->AddChild(m_pRageHitboxLCalf);

	m_pRageOthersAutoZeusEnabled = new CCheckbox(4, 0, 128, 16, "Auto Zeus");
	m_pRageOthersAutoZeusEnabled->SetEventHandler(std::bind(&CRagebot::SetAutoZeus, m_pApp->Ragebot(), std::placeholders::_1));
	m_pRageOthersAutoZeusEnabled->AddDependency(m_pAimbotEnabled);

	m_pRageOthersAutoRevolverEnabled = new CCheckbox(4, 20, 128, 16, "Auto Revolver");
	m_pRageOthersAutoRevolverEnabled->SetEventHandler(std::bind(&CRagebot::SetAutoRevolver, m_pApp->Ragebot(), std::placeholders::_1));
	m_pRageOthersAutoRevolverEnabled->AddDependency(m_pAimbotEnabled);

	m_pRageOthersLagCompensationEnabled = new CCheckbox(4, 40, 128, 16, "Lag Compensation");
	m_pRageOthersLagCompensationEnabled->SetEventHandler(std::bind(&CLagCompensation::SetRageLagCompensationEnabled, m_pApp->LagCompensation(), std::placeholders::_1));
	m_pRageOthersLagCompensationEnabled->AddDependency(m_pAimbotEnabled);

	m_pRageOthersResolverType = new CSelectbox(4, 70, 128, 20, "Resolver Type");
	m_pRageOthersResolverType->AddOption(RESOLVERTYPE_NONE, "None");
	m_pRageOthersResolverType->AddOption(RESOLVERTYPE_LBY, "LBY");
	m_pRageOthersResolverType->AddOption(RESOLVERTYPE_AUTOMATIC, "Automatic");
	m_pRageOthersResolverType->SetEventHandler(std::bind(&CResolver::SetResolverType, m_pApp->Resolver(), std::placeholders::_1));
	m_pRageOthersResolverType->AddDependency(m_pAimbotEnabled);

	m_pRageOthersGroup = new CGroupbox(184, 186, 152, 138, "Others");
	m_pRageOthersGroup->AddChild(m_pRageOthersAutoZeusEnabled);
	m_pRageOthersGroup->AddChild(m_pRageOthersAutoRevolverEnabled);
	m_pRageOthersGroup->AddChild(m_pRageOthersLagCompensationEnabled);
	m_pRageOthersGroup->AddChild(m_pRageOthersResolverType);

	// AntiAim
	m_pAntiaimEnabled = new CCheckbox(4, 0, 60, 16, "Enabled");
	m_pAntiaimEnabled->SetEventHandler(std::bind(&CAntiAim::SetEnabled, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingPitch = new CSelectbox(4, 10, 128, 20, "Pitch");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_NONE, "None");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_DOWN, "Down");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_UP, "Up");
	m_pAntiaimMovingPitch->SetEventHandler(std::bind(&CAntiAim::SetPitchSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimMovingPitch->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimMovingYaw = new CSelectbox(4, 46, 128, 20, "Yaw");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_NONE, "None");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_ADAPTIVE, "Adaptive [WIP]");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_STATIC, "Static");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Static Jitter Backwards");
	m_pAntiaimMovingYaw->SetEventHandler(std::bind(&CAntiAim::SetYawSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimMovingYaw->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimMovingYawOffset = new CSlider(4, 76, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimMovingYawOffset->SetEventHandler(std::bind(&CAntiAim::SetYawOffsetMoving, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimMovingYawOffset->AddDependency(m_pAntiaimEnabled);
	m_pAntiaimMovingYawOffset->AddDependency(m_pAntiaimMovingYaw, (void*)YAWANTIAIM_NONE, true);

	m_pAntiaimMovingYawFake = new CSelectbox(4, 108, 128, 20, "Yaw Fake");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_NONE, "None");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_ADAPTIVE, "Adaptive [WIP]");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_STATIC, "Static");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_STATICJITTER, "Static Jitter");
	m_pAntiaimMovingYawFake->SetEventHandler(std::bind(&CAntiAim::SetYawFakeSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimMovingYawFake->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimYawMovingFakeOffset = new CSlider(4, 138, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimYawMovingFakeOffset->SetEventHandler(std::bind(&CAntiAim::SetYawFakeOffsetMoving, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimYawMovingFakeOffset->AddDependency(m_pAntiaimEnabled);
	m_pAntiaimYawMovingFakeOffset->AddDependency(m_pAntiaimMovingYawFake, (void*)YAWANTIAIM_NONE, true);


	//Standing
	m_pAntiaimStandingPitch = new CSelectbox(4, 10, 128, 20, "Pitch");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_NONE, "None");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_DOWN, "Down");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_UP, "Up");
	m_pAntiaimStandingPitch->SetEventHandler(std::bind(&CAntiAim::SetPitchSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimStandingPitch->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimStandingYaw = new CSelectbox(4, 46, 128, 20, "Yaw");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_NONE, "None");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_ADAPTIVE, "Adaptive [WIP]");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_STATIC, "Static");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	m_pAntiaimStandingYaw->SetEventHandler(std::bind(&CAntiAim::SetYawSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimStandingYaw->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimStandingYawOffset = new CSlider(4, 76, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimStandingYawOffset->SetEventHandler(std::bind(&CAntiAim::SetYawOffsetStanding, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimStandingYawOffset->AddDependency(m_pAntiaimEnabled);
	m_pAntiaimStandingYawOffset->AddDependency(m_pAntiaimStandingYaw, (void*)YAWANTIAIM_NONE, true);

	m_pAntiaimStandingYawFake = new CSelectbox(4, 108, 128, 20, "Yaw Fake");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_NONE, "None");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_ADAPTIVE, "Adaptive [WIP]");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_STATIC, "Static");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_STATICJITTER, "Static Jitter");
	m_pAntiaimStandingYawFake->SetEventHandler(std::bind(&CAntiAim::SetYawFakeSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimStandingYawFake->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimYawStandingFakeOffset = new CSlider(4, 138, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimYawStandingFakeOffset->SetEventHandler(std::bind(&CAntiAim::SetYawFakeOffsetStanding, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimYawStandingFakeOffset->AddDependency(m_pAntiaimEnabled);
	m_pAntiaimYawStandingFakeOffset->AddDependency(m_pAntiaimStandingYawFake, (void*)YAWANTIAIM_NONE, true);

	m_pAntiaimStandingLbyBreaker = new CCheckbox(4, 159, 128, 16, "LBY Breaker");
	m_pAntiaimStandingLbyBreaker->SetEventHandler(std::bind(&CAntiAim::SetLbyBreaker, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimStandingLbyBreaker->AddDependency(m_pAntiaimEnabled);
	m_pAntiaimStandingLbyBreaker->AddDependency(m_pAntiaimStandingYaw, (void*)YAWANTIAIM_NONE, true);
	m_pAntiaimStandingLbyBreaker->AddDependency(m_pAntiaimStandingYawFake, (void*)YAWANTIAIM_NONE, true);

	m_pAntiaimLbyIndicator = new CCheckbox(62, 0, 128, 16, "LBY Indicator");
	m_pAntiaimLbyIndicator->SetEventHandler(std::bind(&CAntiAim::SetDrawLbyIndicator, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimLbyIndicator->AddDependency(m_pAntiaimEnabled);

	m_pAntiaimStandingGroup = new CGroupbox(4, 20, 152, 190, "Standing");
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingPitch);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYaw);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYawOffset);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYawFake);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimYawStandingFakeOffset);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingLbyBreaker);

	m_pAntiaimMovingGroup = new CGroupbox(160, 20, 152, 190, "Moving");
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingPitch);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYaw);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYawOffset);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYawFake);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimYawMovingFakeOffset);


	m_pAntiaimEdgeAntiaimEnabled = new CCheckbox(4, 0, 60, 16, "Enabled");
	m_pAntiaimEdgeAntiaimEnabled->SetEventHandler(std::bind(&CAntiAim::SetDoEdgeAntiAim, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimEdgeAntiaimCheckOffsetLabel = new CLabel(4, 14, 128, 16, "Amount of Checkpoints", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pAntiaimEdgeAntiaimCheckOffsetLabel->AddDependency(m_pAntiaimEdgeAntiaimEnabled);

	m_pAntiaimEdgeAntiaimCheckOffset = new CSlider(4, 34, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 2.0f, 8.0f);
	m_pAntiaimEdgeAntiaimCheckOffset->SetEventHandler(std::bind(&CAntiAim::SetEdgeAntiAimCheckPointsAmount, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimEdgeAntiaimCheckOffset->AddDependency(m_pAntiaimEdgeAntiaimEnabled);

	m_pAntiaimEdgeAntiaimDrawPoints = new CCheckbox(164, 0, 128, 16, "Draw Points");
	m_pAntiaimEdgeAntiaimDrawPoints->SetEventHandler(std::bind(&CAntiAim::SetDrawEdgeAntiAimPoints, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimEdgeAntiaimDrawPoints->AddDependency(m_pAntiaimEdgeAntiaimEnabled);

	m_pAntiaimEdgeAntiaimDrawLines = new CCheckbox(164, 20, 128, 16, "Draw Lines");
	m_pAntiaimEdgeAntiaimDrawLines->SetEventHandler(std::bind(&CAntiAim::SetDrawEdgeAntiAimLines, m_pApp->AntiAim(), std::placeholders::_1));
	m_pAntiaimEdgeAntiaimDrawLines->AddDependency(m_pAntiaimEdgeAntiaimEnabled);

	m_pAntiaimEdgeAntiaimGroup = new CGroupbox(4, 222, 308, 65, "Edge Antiaim");
	m_pAntiaimEdgeAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimEnabled);
	m_pAntiaimEdgeAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimCheckOffsetLabel);
	m_pAntiaimEdgeAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimCheckOffset);
	m_pAntiaimEdgeAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimDrawPoints);
	m_pAntiaimEdgeAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimDrawLines);

	m_pAntiaimGroup = new CGroupbox(352, 16, 324, 308, "AntiAim");
	m_pAntiaimGroup->AddChild(m_pAntiaimEnabled);
	m_pAntiaimGroup->AddChild(m_pAntiaimLbyIndicator);
	m_pAntiaimGroup->AddChild(m_pAntiaimStandingGroup);
	m_pAntiaimGroup->AddChild(m_pAntiaimMovingGroup);
	m_pAntiaimGroup->AddChild(m_pAntiaimEdgeAntiaimGroup);

	m_pRageTab = new CTabPage("Rage");
	m_pRageTab->AddChild(m_pAimbotGroup);
	m_pRageTab->AddChild(m_pRageHitboxGroup);
	m_pRageTab->AddChild(m_pRageOthersGroup);
	m_pRageTab->AddChild(m_pAntiaimGroup);
}

void CMenu::CreateLegitTab()
{
	m_pLegitbotEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pLegitbotEnabled->SetEventHandler(std::bind(&CLegitbot::SetEnabled, m_pApp->Legitbot(), std::placeholders::_1));

	m_pLegitbotAutoshoot = new CCheckbox(4, 20, 128, 16, "Autoshoot");
	m_pLegitbotAutoshoot->SetEventHandler(std::bind(&CLegitbot::SetAutoshoot, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotAutoshoot->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotHelpAfterShots = new CSlider(4, 60, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 10.0f, "Help after shots");
	m_pLegitbotHelpAfterShots->SetEventHandler(std::bind(&CLegitbot::SetHelpAfterShots, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotHelpAfterShots->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotTimeToAim = new CSlider(4, 100, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 1.0f, "Time to Aim");
	m_pLegitbotTimeToAim->SetEventHandler(std::bind(&CLegitbot::SetTimeToAim, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotTimeToAim->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotCurve = new CSlider(4, 140, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 1.5f, "Curve");
	m_pLegitbotCurve->SetEventHandler(std::bind(&CLegitbot::SetCurve, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotCurve->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotFov = new CSlider(4, 180, 128, 16, 0.1f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 30.0f, "FOV");
	m_pLegitbotFov->SetEventHandler(std::bind(&CLegitbot::SetFieldOfView, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotFov->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotPointScale = new CSlider(4, 220, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 3.0f, "Point Scale");
	m_pLegitbotPointScale->SetEventHandler(std::bind(&CLegitbot::SetPointScale, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotPointScale->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotDrawPath = new CCheckbox(4, 260, 80, 16, "Draw Path");
	m_pLegitbotDrawPath->SetEventHandler(std::bind(&CLegitbot::SetDrawPath, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotDrawPath->AddDependency(m_pLegitbotEnabled);
	
	m_pLegitbotPathColor = new CColorPicker(100, 260, 16, 16);
	m_pLegitbotPathColor->SetEventHandler(std::bind(&CLegitbot::SetPathColor, m_pApp->Legitbot(), std::placeholders::_1));
	m_pLegitbotPathColor->AddDependency(m_pLegitbotEnabled);

	m_pLegitbotGroup = new CGroupbox(16, 16, 152, 308, "Aimbot");
	m_pLegitbotGroup->AddChild(m_pLegitbotEnabled);
	m_pLegitbotGroup->AddChild(m_pLegitbotAutoshoot);
	m_pLegitbotGroup->AddChild(m_pLegitbotHelpAfterShots);
	m_pLegitbotGroup->AddChild(m_pLegitbotTimeToAim);
	m_pLegitbotGroup->AddChild(m_pLegitbotCurve);
	m_pLegitbotGroup->AddChild(m_pLegitbotFov);
	m_pLegitbotGroup->AddChild(m_pLegitbotPointScale);
	m_pLegitbotGroup->AddChild(m_pLegitbotDrawPath);
	m_pLegitbotGroup->AddChild(m_pLegitbotPathColor);

	m_pLegitHitboxHead = new CCheckbox(4, 0, 128, 16, "Head");
	m_pLegitHitboxHead->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_HEAD, std::placeholders::_1));
	m_pLegitHitboxHead->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxChest = new CCheckbox(4, 20, 128, 16, "Chest");
	m_pLegitHitboxChest->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_CHEST, std::placeholders::_1));
	m_pLegitHitboxChest->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxPelvis = new CCheckbox(4, 40, 128, 16, "Pelvis");
	m_pLegitHitboxPelvis->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_PELVIS, std::placeholders::_1));
	m_pLegitHitboxPelvis->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxRForearm = new CCheckbox(4, 60, 128, 16, "Right forearm");
	m_pLegitHitboxRForearm->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_RIGHT_FOREARM, std::placeholders::_1));
	m_pLegitHitboxRForearm->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxLForearm = new CCheckbox(4, 80, 128, 16, "Left forearm");
	m_pLegitHitboxLForearm->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_LEFT_FOREARM, std::placeholders::_1));
	m_pLegitHitboxLForearm->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxRCalf = new CCheckbox(4, 100, 128, 16, "Right calf");
	m_pLegitHitboxRCalf->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_RIGHT_CALF, std::placeholders::_1));
	m_pLegitHitboxRCalf->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxLCalf = new CCheckbox(4, 120, 128, 16, "Left calf");
	m_pLegitHitboxLCalf->SetEventHandler(std::bind(&CLegitbot::SetCheckHitbox, m_pApp->Legitbot(), TARGET_HITBOX_LEFT_CALF, std::placeholders::_1));
	m_pLegitHitboxLCalf->AddDependency(m_pLegitbotEnabled);

	m_pLegitHitboxGroup = new CGroupbox(184, 16, 152, 154, "Hitboxes");
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxHead);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxChest);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxPelvis);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxRForearm);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxLForearm);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxRCalf);
	m_pLegitHitboxGroup->AddChild(m_pLegitHitboxLCalf);

	m_pLegitLagCompensationEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pLegitLagCompensationEnabled->SetEventHandler(std::bind(&CLagCompensation::SetLegitLagCompensationEnabled, m_pApp->LagCompensation(), std::placeholders::_1));

	m_pLegitLagCompensationDuration = new CSlider(4, 44, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 200.0f, "Duration");
	m_pLegitLagCompensationDuration->SetEventHandler(std::bind(&CLagCompensation::SetLegitLagCompensationDuration, m_pApp->LagCompensation(), std::placeholders::_1));
	m_pLegitLagCompensationDuration->AddDependency(m_pLegitLagCompensationEnabled);

	m_pLegitLagCompensationGroup = new CGroupbox(184, 186, 152, 138, "Lag Compensation");
	m_pLegitLagCompensationGroup->AddChild(m_pLegitLagCompensationEnabled);
	m_pLegitLagCompensationGroup->AddChild(m_pLegitLagCompensationDuration);

	m_pTriggerbotEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pTriggerbotEnabled->SetEventHandler(std::bind(&CTriggerbot::SetEnabled, m_pApp->Triggerbot(), std::placeholders::_1));

	m_pTriggerbotKey = new CKeyBinder(4, 20, 128, 16, EVENT_BTN_TRIGGERKEY);
	m_pTriggerbotKey->SetEventHandler(std::bind(&CTriggerbot::SetTriggerKey, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotKey->AddDependency(m_pTriggerbotEnabled);

	m_pTriggerbotDelayValue = new CSlider(4, 60, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 40.0f, "Delay (milliseconds)");
	m_pTriggerbotDelayValue->SetEventHandler(std::bind(&CTriggerbot::SetShootDelay, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotDelayValue->AddDependency(m_pTriggerbotEnabled);

	m_pTriggerbotDelayJitterValue = new CSlider(4, 100, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 30.0f, "Delay Jitter (milliseconds)");
	m_pTriggerbotDelayJitterValue->SetEventHandler(std::bind(&CTriggerbot::SetShootDelayJitter, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotDelayJitterValue->AddDependency(m_pTriggerbotEnabled);

	m_pTriggerbotBurst = new CCheckbox(4, 120, 128, 16, "Burstfire");
	m_pTriggerbotBurst->SetEventHandler(std::bind(&CTriggerbot::SetTriggerBurst, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotBurst->AddDependency(m_pTriggerbotEnabled);

	m_pTriggerbotMinShots = new CSlider(4, 160, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 5.0f, "Min shots");
	m_pTriggerbotMinShots->SetEventHandler(std::bind(&CTriggerbot::SetMinShots, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotMinShots->AddDependency(m_pTriggerbotEnabled);
	m_pTriggerbotMinShots->AddDependency(m_pTriggerbotBurst);

	m_pTriggerbotMaxShots = new CSlider(4, 200, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 5.0f, "Max shots");
	m_pTriggerbotMaxShots->SetEventHandler(std::bind(&CTriggerbot::SetMaxShots, m_pApp->Triggerbot(), std::placeholders::_1));
	m_pTriggerbotMaxShots->AddDependency(m_pTriggerbotEnabled);
	m_pTriggerbotMaxShots->AddDependency(m_pTriggerbotBurst);

	m_pTriggerbotGroup = new CGroupbox(352, 16, 152, 308, "Triggerbot");
	m_pTriggerbotGroup->AddChild(m_pTriggerbotEnabled);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotKey);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayValue);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayJitterValue);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotBurst);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotMinShots);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotMaxShots);

	m_pLegitTab = new CTabPage("Legit");
	m_pLegitTab->AddChild(m_pLegitbotGroup);
	m_pLegitTab->AddChild(m_pLegitHitboxGroup);
	m_pLegitTab->AddChild(m_pLegitLagCompensationGroup);
	m_pLegitTab->AddChild(m_pTriggerbotGroup);
}

void CMenu::CreateVisualsTab()
{
	// ==================================================================================================================
	m_pEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pEspEnabled->SetEventHandler(std::bind(&CEsp::SetEnabled, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawBoundingBox = new CSelectbox(4, 36, 128, 20, "Bounding Box");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_NONE, "None");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_EDGE, "Edge");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_FULL, "Full");
	m_pEspDrawBoundingBox->SetEventHandler(std::bind(&CEsp::SetDrawBoundingBox, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawBoundingBox->AddDependency(m_pEspEnabled);

	m_pEspDrawFilledBox = new CCheckbox(4, 56, 128, 16, "Fill Bounding Box");
	m_pEspDrawFilledBox->SetEventHandler(std::bind(&CEsp::SetFillBoundingBox, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawFilledBox->AddDependency(m_pEspEnabled);
	m_pEspDrawFilledBox->AddDependency(m_pEspDrawBoundingBox, (void*)ESP_STYLE_NONE, true);

	m_pEspDrawOutline = new CCheckbox(4, 76, 128, 16, "Outlines");
	m_pEspDrawOutline->SetEventHandler(std::bind(&CEsp::SetDrawOutline, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawOutline->AddDependency(m_pEspEnabled);
	m_pEspDrawOutline->AddDependency(m_pEspDrawBoundingBox, (void*)ESP_STYLE_NONE, true);

	m_pEspDrawHealthbar = new CCheckbox(4, 104, 128, 16, "Health Bar");
	m_pEspDrawHealthbar->SetEventHandler(std::bind(&CEsp::SetDrawHealthBar, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawHealthbar->AddDependency(m_pEspEnabled);

	m_pEspDrawHealthnumber = new CCheckbox(4, 124, 128, 16, "Health Number");
	m_pEspDrawHealthnumber->SetEventHandler(std::bind(&CEsp::SetDrawHealthNumber, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawHealthnumber->AddDependency(m_pEspEnabled);

	m_pEspDrawArmorbar = new CCheckbox(4, 144, 128, 16, "Armor Bar");
	m_pEspDrawArmorbar->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawArmorbar->AddDependency(m_pEspEnabled);

	m_pEspDrawArmornumber = new CCheckbox(4, 164, 128, 16, "Armor Number (WIP)");
	//m_pEspDrawArmornumber->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, m_pApp->Esp(), std::placeholders::_1)); //todo EVENTHANDLER !!!!!
	m_pEspDrawArmornumber->AddDependency(m_pEspEnabled);

	m_pEspDrawAmmobar = new CCheckbox(4, 184, 128, 16, "Ammo Bar");
	m_pEspDrawAmmobar->SetEventHandler(std::bind(&CEsp::SetDrawAmmoBar, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawAmmobar->AddDependency(m_pEspEnabled);

	m_pEspDrawAmmonumber = new CCheckbox(4, 204, 128, 16, "Ammo Number");
	m_pEspDrawAmmonumber->SetEventHandler(std::bind(&CEsp::SetDrawAmmoNumber, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawAmmonumber->AddDependency(m_pEspEnabled);

	m_pEspFadeoutEnabled = new CCheckbox(4, 232, 128, 16, "Fadeout");
	m_pEspFadeoutEnabled->SetEventHandler(std::bind(&CEsp::SetFadeoutEnabled, m_pApp->Esp(), std::placeholders::_1));
	m_pEspFadeoutEnabled->AddDependency(m_pEspEnabled);

	m_pEspFadeoutLabel = new CLabel(4, 246, 128, 16, "Fadeout time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pEspFadeoutLabel->AddDependency(m_pEspEnabled);
	m_pEspFadeoutLabel->AddDependency(m_pEspFadeoutEnabled);

	m_pEspFadeoutValue = new CSlider(4, 270, 128, 16, 0.1f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pEspFadeoutValue->SetEventHandler(std::bind(&CEsp::SetFadeoutTime, m_pApp->Esp(), std::placeholders::_1));
	m_pEspFadeoutValue->AddDependency(m_pEspEnabled);
	m_pEspFadeoutValue->AddDependency(m_pEspFadeoutEnabled);

	// Second column
	m_pEspBarrelEnabled = new CCheckbox(156, 232, 128, 16, "Show Viewangle");
	m_pEspBarrelEnabled->SetEventHandler(std::bind(&CEsp::SetDrawViewangles, m_pApp->Esp(), std::placeholders::_1));
	m_pEspBarrelEnabled->AddDependency(m_pEspEnabled);

	m_pEspBarrelLabel = new CLabel(156, 246, 128, 16, "Viewangle Length", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pEspBarrelLabel->AddDependency(m_pEspEnabled);
	m_pEspBarrelLabel->AddDependency(m_pEspBarrelEnabled);

	m_pEspBarrelValue = new CSlider(156, 270, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 500.0f);
	m_pEspBarrelValue->SetEventHandler(std::bind(&CEsp::SetViewanglesLength, m_pApp->Esp(), std::placeholders::_1));
	m_pEspBarrelValue->AddDependency(m_pEspEnabled);
	m_pEspBarrelValue->AddDependency(m_pEspBarrelEnabled);

	m_pEspDrawGlowEnabled = new CCheckbox(156, 56, 128, 16, "Glow (WIP)");
	//m_pEspDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, m_pApp->Esp(), std::placeholders::_1)); // TODO: EVENTHANDLER !!!!!
	m_pEspDrawGlowEnabled->AddDependency(m_pEspEnabled);

	m_pEspDrawSkeletonEnabled = new CCheckbox(156, 76, 128, 16, "Skeleton");
	m_pEspDrawSkeletonEnabled->SetEventHandler(std::bind(&CEsp::SetDrawSkeleton, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawSkeletonEnabled->AddDependency(m_pEspEnabled);

	m_pEspDrawOwnTeam = new CCheckbox(156, 104, 128, 16, "Own Team");
	m_pEspDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawOwnTeam->AddDependency(m_pEspEnabled);

	m_pEspDrawOwnModel = new CCheckbox(156, 124, 128, 16, "Own Model (3rd person)");
	m_pEspDrawOwnModel->SetEventHandler(std::bind(&CEsp::SetDrawOwnModel, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawOwnModel->AddDependency(m_pEspEnabled);

	m_pEspDrawOnlyVisible = new CCheckbox(156, 164, 128, 16, "Only Visible");
	m_pEspDrawOnlyVisible->SetEventHandler(std::bind(&CEsp::SetDrawOnlyVisible, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawOnlyVisible->AddDependency(m_pEspEnabled);

	m_pEspDrawOnlySpotted = new CCheckbox(156, 184, 128, 16, "Only Spotted");
	m_pEspDrawOnlySpotted->SetEventHandler(std::bind(&CEsp::SetDrawOnlySpotted, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawOnlySpotted->AddDependency(m_pEspEnabled);

	m_pEspDrawActiveWeapon = new CCheckbox(156, 204, 128, 16, "Active Weapon");
	m_pEspDrawActiveWeapon->SetEventHandler(std::bind(&CEsp::SetDrawActiveWeapon, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawActiveWeapon->AddDependency(m_pEspEnabled);

	m_pEspDrawNames = new CCheckbox(156, 34, 128, 16, "Names");
	m_pEspDrawNames->SetEventHandler(std::bind(&CEsp::SetDrawNames, m_pApp->Esp(), std::placeholders::_1));
	m_pEspDrawNames->AddDependency(m_pEspEnabled);

	m_pEspGroup = new CGroupbox(16, 16, 304, 308, "Esp");
	m_pEspGroup->AddChild(m_pEspEnabled);
	m_pEspGroup->AddChild(m_pEspDrawBoundingBox);
	m_pEspGroup->AddChild(m_pEspDrawFilledBox);
	m_pEspGroup->AddChild(m_pEspDrawOutline);
	m_pEspGroup->AddChild(m_pEspDrawHealthbar);
	m_pEspGroup->AddChild(m_pEspDrawHealthnumber);
	m_pEspGroup->AddChild(m_pEspDrawArmorbar);
	m_pEspGroup->AddChild(m_pEspDrawArmornumber);
	m_pEspGroup->AddChild(m_pEspDrawAmmobar);
	m_pEspGroup->AddChild(m_pEspDrawAmmonumber);
	m_pEspGroup->AddChild(m_pEspFadeoutEnabled);
	m_pEspGroup->AddChild(m_pEspFadeoutLabel);
	m_pEspGroup->AddChild(m_pEspFadeoutValue);
	m_pEspGroup->AddChild(m_pEspDrawGlowEnabled);
	m_pEspGroup->AddChild(m_pEspDrawSkeletonEnabled);
	m_pEspGroup->AddChild(m_pEspDrawOwnTeam);
	m_pEspGroup->AddChild(m_pEspDrawOwnModel);
	m_pEspGroup->AddChild(m_pEspDrawActiveWeapon);
	m_pEspGroup->AddChild(m_pEspDrawOnlyVisible);
	m_pEspGroup->AddChild(m_pEspDrawOnlySpotted);
	m_pEspGroup->AddChild(m_pEspDrawNames);
	m_pEspGroup->AddChild(m_pEspBarrelEnabled);
	m_pEspGroup->AddChild(m_pEspBarrelLabel);
	m_pEspGroup->AddChild(m_pEspBarrelValue);
	// ==================================================================================================================

	// ==================================================================================================================
	m_pChamsEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pChamsEnabled->SetEventHandler(std::bind(&CChams::SetEnabled, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsStyle = new CSelectbox(4, 36, 128, 20, "Playerchams Style");
	m_pChamsStyle->AddOption(PLAYER_CHAMSSTYLE_NONE, "None");
	m_pChamsStyle->AddOption(PLAYER_CHAMSSTYLE_LIT, "Lit");
	m_pChamsStyle->AddOption(PLAYER_CHAMSSTYLE_FLAT, "Flat");
	m_pChamsStyle->SetEventHandler(std::bind(&CChams::SetModelStyle, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsStyle->AddDependency(m_pChamsEnabled);

	m_pChamsDrawOwnTeam = new CCheckbox(4, 56, 128, 16, "Own Team");
	m_pChamsDrawOwnTeam->SetEventHandler(std::bind(&CChams::SetRenderTeam, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsDrawOwnTeam->AddDependency(m_pChamsEnabled);

	m_pChamsDrawOwnModel = new CCheckbox(4, 76, 128, 16, "Own Model (3rd person)");
	m_pChamsDrawOwnModel->SetEventHandler(std::bind(&CChams::SetRenderLocalplayer, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsDrawOwnModel->AddDependency(m_pChamsEnabled);

	m_pChamsIgnoreZ = new CCheckbox(4, 104, 128, 16, "Only Visible");
	m_pChamsIgnoreZ->SetEventHandler(std::bind(&CChams::SetOnlyVisible, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsIgnoreZ->AddDependency(m_pChamsEnabled);

	m_pChamsFakeAngle = new CCheckbox(4, 132, 128, 16, "Fake Angle");
	m_pChamsFakeAngle->SetEventHandler(std::bind(&CChams::SetRenderFakeAngle, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsFakeAngle->AddDependency(m_pChamsEnabled);

	m_pChamsWeaponChams = new CSelectbox(4, 166, 128, 20, "Weaponchams Style");
	m_pChamsWeaponChams->AddOption(WEAPON_CHAMSSTYLE_NONE, "None");
	m_pChamsWeaponChams->AddOption(WEAPON_CHAMSSTYLE_GLASS, "Glass");
	m_pChamsWeaponChams->AddOption(WEAPON_CHAMSSTYLE_PLATINUM, "Platinum");
	m_pChamsWeaponChams->AddOption(WEAPON_CHAMSSTYLE_CRYSTAL, "Crystal");
	m_pChamsWeaponChams->AddOption(WEAPON_CHAMSSTYLE_GOLD, "Gold");
	m_pChamsWeaponChams->SetEventHandler(std::bind(&CChams::SetWeaponChamsStyle, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsWeaponChams->AddDependency(m_pChamsEnabled);

	m_pChamsColorVisibleCT = new CColorPicker(4, 190);
	m_pChamsColorVisibleCT->SetTooltipText("CT - Visible");
	m_pChamsColorVisibleCT->SetEventHandler(std::bind(&CChams::SetColorVisibleCT, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsColorVisibleCT->AddDependency(m_pChamsEnabled);

	m_pChamsColorHiddenCT = new CColorPicker(38, 190);
	m_pChamsColorHiddenCT->SetTooltipText("CT - Hidden");
	m_pChamsColorHiddenCT->SetEventHandler(std::bind(&CChams::SetColorHiddenCT, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsColorHiddenCT->AddDependency(m_pChamsEnabled);
	m_pChamsColorHiddenCT->AddDependency(m_pChamsIgnoreZ, nullptr, true);

	m_pChamsColorVisibleT = new CColorPicker(72, 190);
	m_pChamsColorVisibleT->SetTooltipText("T - Visible");
	m_pChamsColorVisibleT->SetEventHandler(std::bind(&CChams::SetColorVisibleT, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsColorVisibleT->AddDependency(m_pChamsEnabled);

	m_pChamsColorHiddenT = new CColorPicker(106, 190);
	m_pChamsColorHiddenT->SetTooltipText("T - Hidden");
	m_pChamsColorHiddenT->SetEventHandler(std::bind(&CChams::SetColorHiddenT, m_pApp->Chams(), std::placeholders::_1));
	m_pChamsColorHiddenT->AddDependency(m_pChamsEnabled);
	m_pChamsColorHiddenT->AddDependency(m_pChamsIgnoreZ, nullptr, true);

	m_pChamsGroup = new CGroupbox(336, 16, 152, 308, "Chams");
	m_pChamsGroup->AddChild(m_pChamsEnabled);
	m_pChamsGroup->AddChild(m_pChamsStyle);
	m_pChamsGroup->AddChild(m_pChamsDrawOwnTeam);
	m_pChamsGroup->AddChild(m_pChamsDrawOwnModel);
	m_pChamsGroup->AddChild(m_pChamsIgnoreZ);
	m_pChamsGroup->AddChild(m_pChamsFakeAngle);
	m_pChamsGroup->AddChild(m_pChamsWeaponChams);
	m_pChamsGroup->AddChild(m_pChamsColorVisibleCT);
	m_pChamsGroup->AddChild(m_pChamsColorHiddenCT);
	m_pChamsGroup->AddChild(m_pChamsColorVisibleT);
	m_pChamsGroup->AddChild(m_pChamsColorHiddenT);
	// ==================================================================================================================

	// ==================================================================================================================
	// Lag Compensation
	m_pDrawLagCompensationStyle = new CSelectbox(4, 10, 128, 20, "Draw Style");
	m_pDrawLagCompensationStyle->AddOption(LC_DRAWSTYLE_NONE, "None");
	m_pDrawLagCompensationStyle->AddOption(LC_DRAWSTYLE_CROSS, "Cross");
	m_pDrawLagCompensationStyle->AddOption(LC_DRAWSTYLE_BONES, "Bones");
	m_pDrawLagCompensationStyle->SetEventHandler(std::bind(&CLagCompensation::SetDrawStyle, m_pApp->LagCompensation(), std::placeholders::_1));

	m_pDrawLagCompensationFrequencyLabel = new CLabel(4, 28, 128, 20, "Draw Frequency");
	m_pDrawLagCompensationFrequencyLabel->AddDependency(m_pDrawLagCompensationStyle, (void*)LC_DRAWSTYLE_NONE, true);

	m_pDrawLagCompensationFrequency = new CSlider(4, 54, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 5.0f);
	m_pDrawLagCompensationFrequency->SetEventHandler(std::bind(&CLagCompensation::SetDrawFrequency, m_pApp->LagCompensation(), std::placeholders::_1));
	m_pDrawLagCompensationFrequency->AddDependency(m_pDrawLagCompensationStyle, (void*)LC_DRAWSTYLE_NONE, true);

	m_pDrawLagCompensationOnlyVisible = new CCheckbox(4, 68, 128, 16, "Only Visible");
	m_pDrawLagCompensationOnlyVisible->SetEventHandler(std::bind(&CLagCompensation::SetDrawOnlyVisible, m_pApp->LagCompensation(), std::placeholders::_1));
	m_pDrawLagCompensationOnlyVisible->AddDependency(m_pDrawLagCompensationStyle, (void*)LC_DRAWSTYLE_NONE, true);

	m_pDrawLagCompensationGroup = new CGroupbox(16, 16, 152, 120, "Lag Compensation");
	m_pDrawLagCompensationGroup->AddChild(m_pDrawLagCompensationStyle);
	m_pDrawLagCompensationGroup->AddChild(m_pDrawLagCompensationFrequencyLabel);
	m_pDrawLagCompensationGroup->AddChild(m_pDrawLagCompensationFrequency);
	m_pDrawLagCompensationGroup->AddChild(m_pDrawLagCompensationOnlyVisible);
	// ==================================================================================================================

	// ==================================================================================================================
	// Bullet Tracer
	m_pBulletTracerEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pBulletTracerEnabled->SetEventHandler(std::bind(&CVisuals::SetBulletTracer, m_pApp->Visuals(), std::placeholders::_1));

	m_pBulletTracerSelf = new CCheckbox(4, 20, 128, 16, "Show mine");
	m_pBulletTracerSelf->SetEventHandler(std::bind(&CVisuals::SetBulletTracerSelf, m_pApp->Visuals(), std::placeholders::_1));
	m_pBulletTracerSelf->AddDependency(m_pBulletTracerEnabled);

	m_pBulletTracerTeam = new CCheckbox(4, 40, 128, 16, "Own Team");
	m_pBulletTracerTeam->SetEventHandler(std::bind(&CVisuals::SetBulletTracerTeam, m_pApp->Visuals(), std::placeholders::_1));
	m_pBulletTracerTeam->AddDependency(m_pBulletTracerEnabled);

	m_pBulletTracerGroup = new CGroupbox(16, 152, 152, 172, "Bullet Tracer");
	m_pBulletTracerGroup->AddChild(m_pBulletTracerEnabled);
	m_pBulletTracerGroup->AddChild(m_pBulletTracerSelf);;
	m_pBulletTracerGroup->AddChild(m_pBulletTracerTeam);
	// ==================================================================================================================

	// ==================================================================================================================
	m_pWeaponEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pWeaponEspEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetEnabled, m_pApp->WeaponEsp(), std::placeholders::_1));

	m_pWeaponEspWeaponLabel = new CLabel(4, 20, 128, 16, "Weapon");
	m_pWeaponEspWeaponLabel->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspWeaponNameEnabled = new CCheckbox(4, 34, 128, 16, "Names");
	m_pWeaponEspWeaponNameEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawWeaponName, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspWeaponNameEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspWeaponBoundingBoxEnabled = new CCheckbox(4, 54, 128, 16, "Bounding Box");
	m_pWeaponEspWeaponBoundingBoxEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawWeaponBoundingBox, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspWeaponBoundingBoxEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspGrenadeLabel = new CLabel(4, 74, 128, 16, "Grenade");
	m_pWeaponEspGrenadeLabel->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspGrenadeNameEnabled = new CCheckbox(4, 88, 128, 16, "Names");
	m_pWeaponEspGrenadeNameEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawGrenadeName, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspGrenadeNameEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspGrenadeBoundingBoxEnabled = new CCheckbox(4, 108, 128, 16, "Bounding Box");
	m_pWeaponEspGrenadeBoundingBoxEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawGrenadeBoundingBox, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspGrenadeBoundingBoxEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombLabel = new CLabel(4, 128, 128, 16, "Bomb");
	m_pWeaponEspBombLabel->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombNameEnabled = new CCheckbox(4, 142, 128, 16, "Name");
	m_pWeaponEspBombNameEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawBombName, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspBombNameEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombBoundingBoxEnabled = new CCheckbox(4, 162, 128, 16, "Bounding Box");
	m_pWeaponEspBombBoundingBoxEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawBombBoundingBox, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspBombBoundingBoxEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombTimerEnabled = new CCheckbox(4, 182, 128, 16, "Timer");
	m_pWeaponEspBombTimerEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawBombTimer, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspBombTimerEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombDefuseTimerEnabled = new CCheckbox(4, 202, 128, 16, "Defuse Timer");
	m_pWeaponEspBombDefuseTimerEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawBombDefuseTimer, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspBombDefuseTimerEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspBombDamageIndicatorEnabled = new CCheckbox(4, 222, 128, 16, "Damage Indicator");
	m_pWeaponEspBombDamageIndicatorEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawBombDamageIndicator, m_pApp->WeaponEsp(), std::placeholders::_1));
	m_pWeaponEspBombDamageIndicatorEnabled->AddDependency(m_pWeaponEspEnabled);

	m_pWeaponEspGroup = new CGroupbox(184, 16, 152, 308, "Weapon Esp");
	m_pWeaponEspGroup->AddChild(m_pWeaponEspEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspWeaponLabel);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspWeaponBoundingBoxEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspWeaponNameEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspGrenadeLabel);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspGrenadeBoundingBoxEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspGrenadeNameEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombLabel);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombNameEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombBoundingBoxEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombTimerEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombDefuseTimerEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspBombDamageIndicatorEnabled);
	// ==================================================================================================================

	// ==================================================================================================================
	m_pSoundEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pSoundEspEnabled->SetEventHandler(std::bind(&CSoundEsp::SetEnabled, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspShowTimeLabel = new CLabel(4, 20, 128, 16, "Show time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pSoundEspShowTimeLabel->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspShowTime = new CSlider(4, 42, 128, 16, 0.1f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pSoundEspShowTime->SetEventHandler(std::bind(&CSoundEsp::SetShowTime, m_pApp->SoundEsp(), std::placeholders::_1));
	m_pSoundEspShowTime->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspFadeoutEnabled = new CCheckbox(4, 56, 128, 16, "Fadeout");
	m_pSoundEspFadeoutEnabled->SetEventHandler(std::bind(&CSoundEsp::SetFadeoutEnabled, m_pApp->SoundEsp(), std::placeholders::_1));
	m_pSoundEspFadeoutEnabled->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspFadeoutTimeLabel = new CLabel(4, 70, 128, 16, "Fadeout time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pSoundEspFadeoutTimeLabel->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspFadeoutTime = new CSlider(4, 92, 128, 16, 0.1f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pSoundEspFadeoutTime->SetEventHandler(std::bind(&CSoundEsp::SetFadeoutTime, m_pApp->SoundEsp(), std::placeholders::_1));
	m_pSoundEspFadeoutTime->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspDrawOwnTeam = new CCheckbox(4, 124, 128, 16, "Own Team");
	m_pSoundEspDrawOwnTeam->SetEventHandler(std::bind(&CSoundEsp::SetDrawOwnTeam, m_pApp->SoundEsp(), std::placeholders::_1));
	m_pSoundEspDrawOwnTeam->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspOnlyNotVisible = new CCheckbox(4, 144, 128, 16, "Only Not Visible");
	m_pSoundEspOnlyNotVisible->SetEventHandler(std::bind(&CSoundEsp::SetDrawOnlyNotVisible, m_pApp->SoundEsp(), std::placeholders::_1));
	m_pSoundEspOnlyNotVisible->AddDependency(m_pSoundEspEnabled);

	m_pSoundEspGroup = new CGroupbox(352, 16, 152, 308, "Sound Esp");
	m_pSoundEspGroup->AddChild(m_pSoundEspEnabled);
	m_pSoundEspGroup->AddChild(m_pSoundEspShowTimeLabel);
	m_pSoundEspGroup->AddChild(m_pSoundEspShowTime);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutEnabled);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutTimeLabel);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutTime);
	m_pSoundEspGroup->AddChild(m_pSoundEspDrawOwnTeam);
	m_pSoundEspGroup->AddChild(m_pSoundEspOnlyNotVisible);
	// ==================================================================================================================

	// ==================================================================================================================
	// OthersGroup
	m_pVisualsOthersHandsDrawStyle = new CSelectbox(4, 12, 128, 16, "Hands Drawstyle");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NONE, "None");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NOHANDS, "NoHands");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_WIREFRAME, "Wireframe");
	m_pVisualsOthersHandsDrawStyle->SetEventHandler(std::bind(&CVisuals::SetHandsDrawStyle, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersHitmarkerEnabled = new CCheckbox(4, 40, 128, 16, "Hitmarker Crosshair");
	m_pVisualsOthersHitmarkerEnabled->SetEventHandler(std::bind(&CGunHud::SetHitmarker, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersHitmarkerSoundEnabled = new CCheckbox(4, 60, 128, 16, "Hitmarker Sound");
	m_pVisualsOthersHitmarkerSoundEnabled->SetEventHandler(std::bind(&CGunHud::SetHitmarkerSound, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersHitmarkerHitpointEnabled = new CCheckbox(4, 80, 128, 16, "Hitmarker Hitpoint");
	m_pVisualsOthersHitmarkerHitpointEnabled->SetEventHandler(std::bind(&CGunHud::SetHitmarkerHitpoint, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersCrosshairEnabled = new CCheckbox(4, 108, 128, 16, "Custom Crosshair");
	m_pVisualsOthersCrosshairEnabled->SetEventHandler(std::bind(&CGunHud::SetCrosshair, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersRecoilCrosshairEnabled = new CCheckbox(4, 128, 128, 16, "Show Recoil Crosshair");
	m_pVisualsOthersRecoilCrosshairEnabled->SetEventHandler(std::bind(&CGunHud::SetCrosshairShowRecoil, m_pApp->GunHud(), std::placeholders::_1));
	m_pVisualsOthersRecoilCrosshairEnabled->AddDependency(m_pVisualsOthersCrosshairEnabled);

	m_pVisualsOthersSpreadConeEnabled = new CCheckbox(4, 156, 128, 16, "Show Spread Cone");
	m_pVisualsOthersSpreadConeEnabled->SetEventHandler(std::bind(&CGunHud::SetSpreadCone, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersRecoilSpreadConeEnabled = new CCheckbox(4, 176, 128, 16, "Show Recoil Spread Cone");
	m_pVisualsOthersRecoilSpreadConeEnabled->SetEventHandler(std::bind(&CGunHud::SetSpreadConeShowRecoil, m_pApp->GunHud(), std::placeholders::_1));
	m_pVisualsOthersRecoilSpreadConeEnabled->AddDependency(m_pVisualsOthersSpreadConeEnabled);

	m_pVisualsOtherSpreadConeStyle = new CSelectbox(4, 206, 128, 16, "Spread Cone Style");
	m_pVisualsOtherSpreadConeStyle->AddOption(SPREADCONE_STYLE_OUTLINE, "Outline");
	m_pVisualsOtherSpreadConeStyle->AddOption(SPREADCONE_STYLE_FILLED, "Filled");
	m_pVisualsOtherSpreadConeStyle->SetEventHandler(std::bind(&CGunHud::SetSpreadConeStyle, m_pApp->GunHud(), std::placeholders::_1));
	m_pVisualsOtherSpreadConeStyle->AddDependency(m_pVisualsOthersSpreadConeEnabled);

	m_pVisualsOtherSpreadConeColor = new CColorPicker(136, 206, 16, 16);
	m_pVisualsOtherSpreadConeColor->SetEventHandler(std::bind(&CGunHud::SetSpreadConeColor, m_pApp->GunHud(), std::placeholders::_1));

	m_pVisualsOthersNightmode = new CSlider(156, 110, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f, "Nightmode");
	m_pVisualsOthersNightmode->SetEventHandler(std::bind(&CMaterialVisuals::Nightmode, m_pApp->MaterialVisuals(), std::placeholders::_1));

	m_pVisualsOthersAsuswalls = new CSlider(156, 140, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f, "Asuswalls");
	m_pVisualsOthersAsuswalls->SetEventHandler(std::bind(&CMaterialVisuals::Asuswalls, m_pApp->MaterialVisuals(), std::placeholders::_1));

	m_pVisualsOthersSkychanger = new CSelectbox(156, 170, 128, 16, "Skychanger");
	m_pVisualsOthersSkychanger->AddOption(SKY_NOCHANGE, "No Change");
	m_pVisualsOthersSkychanger->AddOption(SKY_BAGGAGE, "cs_baggage_skybox_");
	m_pVisualsOthersSkychanger->AddOption(SKY_TIBET, "cs_tibet");
	m_pVisualsOthersSkychanger->AddOption(SKY_EMBASSY, "embassy");
	m_pVisualsOthersSkychanger->AddOption(SKY_ITALY, "italy");
	m_pVisualsOthersSkychanger->AddOption(SKY_JUNGLE, "jungle");
	m_pVisualsOthersSkychanger->AddOption(SKY_OFFICE, "office");
	m_pVisualsOthersSkychanger->AddOption(SKY_DAYLIGHT1, "sky_cs15_daylight01_hdr");
	m_pVisualsOthersSkychanger->AddOption(SKY_DAYLIGHT2, "sky_cs15_daylight02_hdr");
	m_pVisualsOthersSkychanger->AddOption(SKY_DAYLIGHT3, "sky_cs15_daylight03_hdr");
	m_pVisualsOthersSkychanger->AddOption(SKY_DAYLIGHT4, "sky_cs15_daylight04_hdr");
	m_pVisualsOthersSkychanger->AddOption(SKY_NUKEBLANK, "nukeblank");
	m_pVisualsOthersSkychanger->AddOption(SKY_VENICE, "sky_venice");
	m_pVisualsOthersSkychanger->AddOption(SKY_CLOUDY, "sky_csgo_cloudy01");
	m_pVisualsOthersSkychanger->AddOption(SKY_NIGHT, "sky_csgo_night02");
	m_pVisualsOthersSkychanger->AddOption(SKY_NIGHTB, "sky_csgo_night02b");
	m_pVisualsOthersSkychanger->AddOption(SKY_VERTIGO, "vertigo");
	m_pVisualsOthersSkychanger->AddOption(SKY_VERTIGOBLUE, "vertigoblue_hdr");
	m_pVisualsOthersSkychanger->AddOption(SKY_DUST, "sky_dust");
	m_pVisualsOthersSkychanger->AddOption(SKY_VIETNAM, "vietnam");
	m_pVisualsOthersSkychanger->SetEventHandler(std::bind(&CMaterialVisuals::Skychanger, m_pApp->MaterialVisuals(), std::placeholders::_1));


	m_pVisualsOthersThirdperson = new CCheckbox(156, 0, 128, 16, "Thirdperson");
	m_pVisualsOthersThirdperson->SetEventHandler(std::bind(&CVisuals::SetThirdperson, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersThirdpersonLabel = new CLabel(156, 18, 128, 16, "Thirdperson Distance", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pVisualsOthersThirdpersonLabel->AddDependency(m_pVisualsOthersThirdperson);

	m_pVisualsOthersThirdpersonDistance = new CSlider(156, 40, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 30.0f, 300.0f);
	m_pVisualsOthersThirdpersonDistance->SetEventHandler(std::bind(&CVisuals::SetThirdpersonDistance, m_pApp->Visuals(), std::placeholders::_1));
	m_pVisualsOthersThirdpersonDistance->AddDependency(m_pVisualsOthersThirdperson);

	m_pVisualsOthersMirror = new CCheckbox(156, 64, 128, 16, "Mirror");
	m_pVisualsOthersMirror->SetEventHandler(std::bind(&CMirror::SetEnabled, m_pApp->Mirror(), std::placeholders::_1));

	m_pVisualsOthersGroup = new CGroupbox(16, 16, 304, 308, "Others");
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHandsDrawStyle);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHitmarkerEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHitmarkerSoundEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHitmarkerHitpointEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersCrosshairEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersRecoilCrosshairEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersSpreadConeEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersRecoilSpreadConeEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOtherSpreadConeStyle);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOtherSpreadConeColor);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersNightmode);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersAsuswalls);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersSkychanger);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdperson);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdpersonLabel);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdpersonDistance);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersMirror);
	// ==================================================================================================================

	// ==================================================================================================================
	// EffectsGroup
	m_pEffectsNoVisualRecoil = new CCheckbox(4, 0, 128, 16, "Remove Visual Recoil");
	m_pEffectsNoVisualRecoil->SetEventHandler(std::bind(&CVisuals::SetNoVisualRecoil, m_pApp->Visuals(), std::placeholders::_1));

	m_pEffectsNoSmoke = new CCheckbox(4, 20, 128, 16, "Remove Smoke");
	m_pEffectsNoSmoke->SetEventHandler(std::bind(&CVisuals::NoSmoke, m_pApp->Visuals(), std::placeholders::_1));

	m_pEffectsNoScope = new CCheckbox(4, 40, 128, 16, "Remove Scope");
	m_pEffectsNoScope->SetEventHandler(std::bind(&CVisuals::SetNoScope, m_pApp->Visuals(), std::placeholders::_1));

	m_pEffectsDisablePostprocessing = new CCheckbox(4, 60, 128, 16, "Disable PostProcessing");
	m_pEffectsDisablePostprocessing->SetEventHandler(std::bind(&CVisuals::DisablePostProcessing, m_pApp->Visuals(), std::placeholders::_1));

	m_pEffectsNoFlash = new CCheckbox(4, 88, 128, 16, "Remove Flash");
	m_pEffectsNoFlash->SetEventHandler(std::bind(&CVisuals::SetNoFlash, m_pApp->Visuals(), std::placeholders::_1));

	m_pEffectsNoFlashLabel = new CLabel(4, 102, 128, 16, "Flash Percentage", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pEffectsNoFlashLabel->AddDependency(m_pEffectsNoFlash);

	m_pEffectsNoFlashValue = new CSlider(4, 124, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f);
	m_pEffectsNoFlashValue->SetEventHandler(std::bind(&CVisuals::NoFlash, m_pApp->Visuals(), std::placeholders::_1));
	m_pEffectsNoFlashValue->AddDependency(m_pEffectsNoFlash);

	m_pEffectsGroup = new CGroupbox(336, 16, 152, 308, "Effects");
	m_pEffectsGroup->AddChild(m_pEffectsNoVisualRecoil);
	m_pEffectsGroup->AddChild(m_pEffectsNoSmoke);
	m_pEffectsGroup->AddChild(m_pEffectsNoScope);
	m_pEffectsGroup->AddChild(m_pEffectsDisablePostprocessing);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlash);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlashLabel);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlashValue);
	// ==================================================================================================================

	// ==================================================================================================================
	// FovChangerGroup
	m_pFovChangerFovEnabled = new CCheckbox(4, 0, 128, 16, "FOV");
	m_pFovChangerFovEnabled->SetEventHandler(std::bind(&CVisuals::SetFovChange, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerFovScopeEnabled = new CCheckbox(4, 20, 128, 16, "Scoping FOV");
	m_pFovChangerFovScopeEnabled->SetEventHandler(std::bind(&CVisuals::SetFovChangeScoped, m_pApp->Visuals(), std::placeholders::_1));
	m_pFovChangerFovScopeEnabled->AddDependency(m_pFovChangerFovEnabled);

	m_pFovChangerFovLabel = new CLabel(4, 34, 128, 16, "FOV Value", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pFovChangerFovLabel->AddDependency(m_pFovChangerFovEnabled);

	m_pFovChangerFovValue = new CSlider(4, 56, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 170.0f);
	m_pFovChangerFovValue->SetEventHandler(std::bind(&CVisuals::SetFovValue, m_pApp->Visuals(), std::placeholders::_1));
	m_pFovChangerFovValue->AddDependency(m_pFovChangerFovEnabled);

	m_pFovChangerViewmodelFovEnabled = new CCheckbox(4, 68, 128, 16, "Viewmodel-FOV");
	m_pFovChangerViewmodelFovEnabled->SetEventHandler(std::bind(&CVisuals::SetViewmodelFov, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerViewmodelFovLabel = new CLabel(4, 82, 128, 16, "FOV Viewmodel Value", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pFovChangerViewmodelFovLabel->AddDependency(m_pFovChangerViewmodelFovEnabled);

	m_pFovChangerViewmodelFovValue = new CSlider(4, 106, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 150.0f);
	m_pFovChangerViewmodelFovValue->SetEventHandler(std::bind(&CVisuals::SetViewmodelFovValue, m_pApp->Visuals(), std::placeholders::_1));
	m_pFovChangerViewmodelFovValue->AddDependency(m_pFovChangerViewmodelFovEnabled);

	m_pFovChangerGroup = new CGroupbox(504, 16, 152, 308, "Fov Changer");
	m_pFovChangerGroup->AddChild(m_pFovChangerFovEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovScopeEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovLabel);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovValue);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovLabel);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovValue);
	// ==================================================================================================================

	m_pVisualsOneTab = new CTabPage("Visuals I");
	m_pVisualsOneTab->AddChild(m_pEspGroup);
	m_pVisualsOneTab->AddChild(m_pChamsGroup);
	// TODO: GlowGroup?
	
	m_pVisualsTwoTab = new CTabPage("Visuals II");
	m_pVisualsTwoTab->AddChild(m_pDrawLagCompensationGroup);
	m_pVisualsTwoTab->AddChild(m_pBulletTracerGroup);
	m_pVisualsTwoTab->AddChild(m_pWeaponEspGroup);
	m_pVisualsTwoTab->AddChild(m_pSoundEspGroup);

	m_pVisualsThreeTab = new CTabPage("Visuals III");
	m_pVisualsThreeTab->AddChild(m_pVisualsOthersGroup);
	m_pVisualsThreeTab->AddChild(m_pEffectsGroup);
	m_pVisualsThreeTab->AddChild(m_pFovChangerGroup);

	m_pVisualsTabContainer = new CTabContainer();
	m_pVisualsTabContainer->AddChild(m_pVisualsOneTab);
	m_pVisualsTabContainer->AddChild(m_pVisualsTwoTab);
	m_pVisualsTabContainer->AddChild(m_pVisualsThreeTab);
	m_pVisualsTabContainer->SelectTab(0);

	m_pVisualsTab = new CTabPage("Visuals");
	m_pVisualsTab->AddChild(m_pVisualsTabContainer);
}

void CMenu::CreateMiscTab()
{
	m_pMiscOthersNoRecoilEnabled = new CCheckbox(4, 0, 128, 16, "Remove Recoil");
	m_pMiscOthersNoRecoilEnabled->SetEventHandler(std::bind(&CMisc::SetNoRecoil, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscOthersAutoPistolEnabled = new CCheckbox(4, 20, 128, 16, "Auto Pistol");
	m_pMiscOthersAutoPistolEnabled->SetEventHandler(std::bind(&CMisc::SetAutoPistol, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscOthersJumpScoutEnabled = new CCheckbox(4, 40, 128, 16, "Scout Jump Shots");
	m_pMiscOthersJumpScoutEnabled->SetEventHandler(std::bind(&CMisc::SetJumpScout, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscOthersAutoAcceptEnabled = new CCheckbox(4, 60, 128, 16, "Auto Accept Matchmaking");
	m_pMiscOthersAutoAcceptEnabled->SetEventHandler(std::bind(&CMisc::SetAutoAccept, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscOthersNoNameEnabled = new CCheckbox(4, 80, 128, 16, "Remove Name");
	m_pMiscOthersNoNameEnabled->SetEventHandler(std::bind(&CMisc::SetNoName, m_pApp->Misc(), std::placeholders::_1));

	m_pFakelagEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pFakelagEnabled->SetEventHandler(std::bind(&CFakelag::SetEnabled, m_pApp->Fakelag(), std::placeholders::_1));

	m_pFakelagOnlyInAir = new CCheckbox(4, 20, 128, 16, "Only In Air");
	m_pFakelagOnlyInAir->SetEventHandler(std::bind(&CFakelag::SetOnlyInAir, m_pApp->Fakelag(), std::placeholders::_1));
	m_pFakelagOnlyInAir->AddDependency(m_pFakelagEnabled);

	m_pFakelagLabel = new CLabel(4, 40, 128, 16, "Packets choking", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pFakelagLabel->AddDependency(m_pFakelagEnabled);

	m_pFakelagChokeAmount = new CSlider(4, 62, 128, 16, 1.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, (float)MAX_CHOKE_PACKETS);
	m_pFakelagChokeAmount->SetEventHandler(std::bind(&CFakelag::SetChokeAmount, m_pApp->Fakelag(), std::placeholders::_1));
	m_pFakelagChokeAmount->AddDependency(m_pFakelagEnabled);

	m_pFakelagType = new CSelectbox(4, 86, 128, 16, "Lag Type");
	m_pFakelagType->AddOption(FAKELAG_TYPE_FACTOR, "Factor");
	m_pFakelagType->AddOption(FAKELAG_TYPE_ADAPTIVE, "Adaptive");
	m_pFakelagType->SetEventHandler(std::bind(&CFakelag::SetLagType, m_pApp->Fakelag(), std::placeholders::_1));
	m_pFakelagType->AddDependency(m_pFakelagEnabled);

	m_pMiscOthersGroup = new CGroupbox(16, 16, 152, 308, "Others");
	m_pMiscOthersGroup->AddChild(m_pMiscOthersNoRecoilEnabled);
	m_pMiscOthersGroup->AddChild(m_pMiscOthersAutoPistolEnabled);
	m_pMiscOthersGroup->AddChild(m_pMiscOthersJumpScoutEnabled);
	m_pMiscOthersGroup->AddChild(m_pMiscOthersAutoAcceptEnabled);
	m_pMiscOthersGroup->AddChild(m_pMiscOthersNoNameEnabled);

	m_pMiscBhopEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pMiscBhopEnabled->SetEventHandler(std::bind(&CBhop::SetEnabled, m_pApp->Bhop(), std::placeholders::_1));

	m_pMiscBhopAutoStrafeMode = new CSelectbox(4, 16, 124, 16, "Auto Strafe Mode");
	m_pMiscBhopAutoStrafeMode->AddOption(AUTOSTRAFEMODE_NONE, "None");
	m_pMiscBhopAutoStrafeMode->AddOption(AUTOSTRAFEMODE_LEGIT, "Legit");
	m_pMiscBhopAutoStrafeMode->AddOption(AUTOSTRAFEMODE_RAGE, "Rage");
	m_pMiscBhopAutoStrafeMode->SetEventHandler(std::bind(&CBhop::SetAutoStrafeMode, m_pApp->Bhop(), std::placeholders::_1));

	m_pFakelagGroup = new CGroupbox(184, 16, 152, 308, "Fakelag");
	m_pFakelagGroup->AddChild(m_pFakelagEnabled);
	m_pFakelagGroup->AddChild(m_pFakelagOnlyInAir);
	m_pFakelagGroup->AddChild(m_pFakelagLabel);
	m_pFakelagGroup->AddChild(m_pFakelagChokeAmount);
	m_pFakelagGroup->AddChild(m_pFakelagType);

	m_pMiscBhopCircleStrafeEnabled = new CCheckbox(4, 0, 128, 16, "Circle Strafe");
	m_pMiscBhopCircleStrafeEnabled->SetEventHandler(std::bind(&CBhop::SetCircleStrafe, m_pApp->Bhop(), std::placeholders::_1));

	m_pMiscBhopCircleStrafeDirection = new CSelectbox(4, 32, 124, 16, "Circle Strafe Direction");
	m_pMiscBhopCircleStrafeDirection->AddOption(CIRCLESTRAFEDIRECTION_RIGHT, "Right");
	m_pMiscBhopCircleStrafeDirection->AddOption(CIRCLESTRAFEDIRECTION_LEFT, "Left");
	m_pMiscBhopCircleStrafeDirection->SetEventHandler(std::bind(&CBhop::SetCircleStrafeStartDirection, m_pApp->Bhop(), std::placeholders::_1));
	m_pMiscBhopCircleStrafeDirection->AddDependency(m_pMiscBhopCircleStrafeEnabled);

	m_pMiscBhopCircleStrafeLabel = new CLabel(4, 46, 128, 16, "On: MENU");

	m_pMiscBhopCircleStrafeGroup = new CGroupbox(4, 80, 136, 80, "Circle Strafe");
	m_pMiscBhopCircleStrafeGroup->AddChild(m_pMiscBhopCircleStrafeEnabled);
	m_pMiscBhopCircleStrafeGroup->AddChild(m_pMiscBhopCircleStrafeDirection);
	m_pMiscBhopCircleStrafeGroup->AddChild(m_pMiscBhopCircleStrafeLabel);

	CSelectbox* m_pMiscBhopCircleStrafeDirection;

	m_pMiscBhopAutoStrafeGroup = new CGroupbox(4, 20, 136, 50, "Auto Strafe");
	m_pMiscBhopAutoStrafeGroup->AddChild(m_pMiscBhopAutoStrafeMode);

	m_pMiscBhopGroup = new CGroupbox(352, 16, 152, 308, "Bhop");
	m_pMiscBhopGroup->AddChild(m_pMiscBhopEnabled);
	m_pMiscBhopGroup->AddChild(m_pMiscBhopAutoStrafeGroup);
	m_pMiscBhopGroup->AddChild(m_pMiscBhopCircleStrafeGroup);

	m_pMiscTab = new CTabPage("Misc");
	m_pMiscTab->AddChild(m_pMiscOthersGroup);
	m_pMiscTab->AddChild(m_pFakelagGroup);
	m_pMiscTab->AddChild(m_pMiscBhopGroup);
}

void CMenu::CreateSkinChangerTab()
{
	// Knife
	m_pSkinChangerKnife = new CSelectbox(4, 4, 100, 16, "Knife");
	m_pSkinChangerKnife->AddOption(-1, "No Change");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE, "Default-CT");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_T, "Default-T");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_BAYONET, "Bayonet");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_BUTTERFLY, "Butterfly");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_FALCHION, "Falchion");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_FLIP, "Flip");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_GUT, "Gut");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_TACTICAL, "Huntsman");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_KARAMBIT, "Karambit");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet");
	m_pSkinChangerKnife->AddOption(WEAPON_KNIFE_PUSH, "Shadow Dagger");
	m_pSkinChangerKnife->SetEventHandler(std::bind(&CMenu::AddKnifeToWeapons, this, std::placeholders::_1));

	// Glove
	m_pSkinChangerGlove = new CSelectbox(108, 4, 100, 16, "Glove");
	m_pSkinChangerGlove->AddOption(GLOVE_NONE, "None");
	m_pSkinChangerGlove->AddOption(GLOVE_BLOODHOUND, "Bloodhound");
	m_pSkinChangerGlove->AddOption(GLOVE_DRIVER, "Driver");
	m_pSkinChangerGlove->AddOption(GLOVE_HANDWRAPS, "Hand Wraps");
	m_pSkinChangerGlove->AddOption(GLOVE_MOTO, "Moto");
	m_pSkinChangerGlove->AddOption(GLOVE_SPECIALIST, "Specialist");
	m_pSkinChangerGlove->AddOption(GLOVE_SPORT, "Sport");

	//Weapon
	m_pSkinChangerWeapon = new CSelectbox(4, 34, 128, 16, "Weapon");
	m_pSkinChangerWeapon->AddOption(0, "None");
	m_pSkinChangerWeapon->AddOption(-1, "Knife");
	std::vector<WeaponMetadata_t>* m_mapWeaponIds = m_pApp->SkinChanger()->GetWeapons();
	for (std::vector<WeaponMetadata_t>::iterator it = m_mapWeaponIds->begin(); it != m_mapWeaponIds->end(); it++)
	{
		m_pSkinChangerWeapon->AddOption(it->id, it->readableName);
	}
	m_pSkinChangerWeapon->SetEventHandler(std::bind(&CMenu::FillSkinIds, this, std::placeholders::_1));

	// Skins
	m_pSkinChangerApplyT = new CButton(132, 54, 60, 20, "T");
	m_pSkinChangerApplyT->SetEventHandler(std::bind(&CMenu::ApplySkin, this, TEAMNUM_T));
	m_pSkinChangerApplyCt = new CButton(68, 54, 60, 20, "CT");
	m_pSkinChangerApplyCt->SetEventHandler(std::bind(&CMenu::ApplySkin, this, TEAMNUM_CT));
	m_pSkinChangerApplyBoth = new CButton(4, 54, 60, 20, "Both");
	m_pSkinChangerApplyBoth->SetEventHandler(std::bind(&CMenu::ApplySkinsBothTeams, this));


	m_pSkinChangerWeaponName = new CTextbox(278, 4, 128, 16, "Name");

	m_pSkinChangerWeaponStattrakCount = new CNumericUpDown(278, 34, 128, 16, "Stattrak Count");

	m_pSkinChangerWeaponSeed = new CNumericUpDown(278, 64, 128, 16, "Seed");

	m_pSkinChangerWeaponWear = new CSlider(278, 104, 128, 16, 0.01, SLIDER_ORIENTATION_HORIZONTAL, false, 0.01f, 1.0f, "Wear");

	m_pSkinChangerOnlyMyWeapon = new CCheckbox(278, 120, 128, 16, "Only my weapons");
	m_pSkinChangerOnlyMyWeapon->SetEventHandler(std::bind(&CSkinChanger::SetOnlyMyWeapons, m_pApp->SkinChanger(), std::placeholders::_1));

	m_pSkinChangerSkin = new CSelectbox(140, 34, 128, 16, "Skin");
	m_pSkinChangerSkin->AddOption(0, "None");

	m_pSkinChangerLoadCfg = new CButton(400, 4, 128, 20, "Load Cfg");
	m_pSkinChangerLoadCfg->SetEventHandler(std::bind(&CSkinChanger::LoadFromConfig, m_pApp->SkinChanger(), "skins.cfg"));

	m_pSkinChangerSaveCfg = new CButton(400, 28, 128, 20, "Save Cfg");
	m_pSkinChangerSaveCfg->SetEventHandler(std::bind(&CSkinChanger::WriteToConfig, m_pApp->SkinChanger(), "skins.cfg"));

	m_pSkinChangerSkinsGroup = new CGroupbox(4, 10, 684, 300, "Skins");
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerKnife);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerGlove);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerWeapon);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerSkin);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerWeaponName);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerWeaponStattrakCount);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerWeaponSeed);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerWeaponWear);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerOnlyMyWeapon);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerApplyBoth);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerApplyCt);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerApplyT);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerLoadCfg);
	m_pSkinChangerSkinsGroup->AddChild(m_pSkinChangerSaveCfg);

	m_pSkinChangerTab = new CTabPage("Skin Changer");
	m_pSkinChangerTab->AddChild(m_pSkinChangerSkinsGroup);
}

void CMenu::CreateConfigTab()
{
	m_pConfigLoadables = new CSelectbox(4, 10, 100, 16, "Loadable Configs");
	FillSelectboxWithFiles(m_pConfigLoadables, CONFIG_FOLDER);

	m_pConfigLoadConfigButton = new CButton(4, 32, 128, 16, "Load Config");
	m_pConfigLoadConfigButton->SetEventHandler(std::bind(&CMenu::LoadConfig, this));

	m_pConfigReloadLoadables = new CButton(4, 52, 128, 16, "Reload Configs");
	m_pConfigReloadLoadables->SetEventHandler(std::bind(&CMenu::FillSelectboxWithFiles, this, m_pConfigLoadables, CONFIG_FOLDER));

	m_pConfigSaveConfigName = new CTextbox(4, 87, 128, 16, "Configname");

	m_pConfigSaveConfigButton = new CButton(4, 107, 128, 16, "Save Config");
	m_pConfigSaveConfigButton->SetEventHandler(std::bind(&CMenu::SaveConfig, this));

	m_pConfigGroup = new CGroupbox(4, 4, 152, 308, "Configs");
	m_pConfigGroup->AddChild(m_pConfigLoadables);
	m_pConfigGroup->AddChild(m_pConfigLoadConfigButton);
	m_pConfigGroup->AddChild(m_pConfigReloadLoadables);
	m_pConfigGroup->AddChild(m_pConfigSaveConfigName);
	m_pConfigGroup->AddChild(m_pConfigSaveConfigButton);

	m_pDetachBtn = new CButton(170, 32, 120, 45, "Detach");
	m_pDetachBtn->SetEventHandler(std::bind(&CApplication::Detach, m_pApp));

	m_pForceFullUpdate = new CButton(170, 90, 120, 45, "Force Full Update");
	m_pForceFullUpdate->SetEventHandler(std::bind(&IClientState::ForceFullUpdate, m_pApp->ClientState()));

	m_pColorPicker = new CColorPicker(300, 30);
	m_pColorPicker->SetEventHandler(std::bind(&CGunHud::SetSpreadConeColor, m_pApp->GunHud(), std::placeholders::_1));

	m_pNumericUpDown = new CNumericUpDown(300, 90, 128, 16, "Numeric Up Down :D");

	m_pConfigPrintValue = new CButton(300, 120, 128, 16, "Print NumUpDown Value");
	m_pConfigPrintValue->SetEventHandler(std::bind(&CMenu::PrintNumUpDownValue, this));

	m_pConfigTab = new CTabPage("Config");
	m_pConfigTab->AddChild(m_pConfigGroup);
	m_pConfigTab->AddChild(m_pDetachBtn);
	m_pConfigTab->AddChild(m_pForceFullUpdate);
	m_pConfigTab->AddChild(m_pColorPicker);
	m_pConfigTab->AddChild(m_pNumericUpDown);
	m_pConfigTab->AddChild(m_pConfigPrintValue);
}

void CMenu::AddKnifeToWeapons(int iWeaponId)
{
	m_pSkinChangerWeapon->ReplaceIdOfOption("Knife", iWeaponId);
	m_pSkinChangerWeapon->SetSelectionById(iWeaponId);
}

void CMenu::FillSkinIds(int iWeaponId)
{
	m_pSkinChangerSkin->ClearOptions();
	m_pSkinChangerSkin->AddOption(0, "None");

	if (iWeaponId <= 0)
	{
		m_pSkinChangerSkin->SetSelection(0);
		return;
	}

	std::unordered_map<int, std::vector<SkinMetadata_t>>* m_pSkins = m_pApp->SkinChanger()->GetSkins();
	std::unordered_map<int, std::vector<SkinMetadata_t>>::iterator it = m_pSkins->find(iWeaponId);
	if (it == m_pSkins->end())
	{
		m_pSkinChangerSkin->SetSelection(0);
		return;
	}

	for (std::vector<SkinMetadata_t>::iterator skinIt = it->second.begin(); skinIt != it->second.end(); skinIt++)
	{
		m_pSkinChangerSkin->AddOptionW(skinIt->id, skinIt->name);
	}

	CSkinMetadata* pSkin = m_pApp->SkinChanger()->GetSkinMetadataForWeapon(iWeaponId);
	if (pSkin)
	{
		m_pSkinChangerSkin->SetSelectionById(pSkin->m_iFallbackPaintKit);
		m_pSkinChangerWeaponName->SetText(pSkin->m_pCustomName);
		m_pSkinChangerWeaponStattrakCount->SetValue(pSkin->m_iFallbackStatTrak);
		m_pSkinChangerWeaponSeed->SetValue(pSkin->m_iFallbackSeed);
		m_pSkinChangerWeaponWear->SetValue(pSkin->m_fFallbackWear);
	}
	else
	{
		m_pSkinChangerSkin->SetSelection(0);
	}
}

void CMenu::ApplySkin(int iTeamNum)
{
	int iSkinId = m_pSkinChangerSkin->GetValue();
	int iWeaponId = m_pSkinChangerWeapon->GetValue();

	int iWeaponSeed = 0;
	const char* pWeaponSeed = m_pSkinChangerWeaponSeed->GetText();
	if (!(pWeaponSeed != NULL && pWeaponSeed[0] == '\0'))
	{
		iWeaponSeed = atoi(pWeaponSeed);
	}

	int iWeaponStattrakCount = -1;
	const char* pWeaponStattrak = m_pSkinChangerWeaponStattrakCount->GetText();
	if (!(pWeaponStattrak != NULL && pWeaponStattrak[0] == '\0'))
	{
		int iTemp = atoi(pWeaponStattrak);
		if (iTemp >= -1)
		{
			iWeaponStattrakCount = iTemp;
		}
	}

	const char* pWeaponName = m_pSkinChangerWeaponName->GetText();
	float fWeaponWear = m_pSkinChangerWeaponWear->GetValue();

	if (iWeaponId == WEAPON_KNIFE ||
		iWeaponId == WEAPON_KNIFE_BAYONET ||
		iWeaponId == WEAPON_KNIFE_FLIP ||
		iWeaponId == WEAPON_KNIFE_GUT ||
		iWeaponId == WEAPON_KNIFE_KARAMBIT ||
		iWeaponId == WEAPON_KNIFE_M9_BAYONET ||
		iWeaponId == WEAPON_KNIFE_BUTTERFLY ||
		iWeaponId == WEAPON_KNIFE_FALCHION ||
		iWeaponId == WEAPON_KNIFE_PUSH ||
		iWeaponId == WEAPON_KNIFE_SURVIVAL_BOWIE ||
		iWeaponId == WEAPON_KNIFE_T ||
		iWeaponId == WEAPON_KNIFE_TACTICAL ||
		iWeaponId == -1)
	{
		m_pApp->SkinChanger()->ApplyDesiredKnife(iTeamNum, iWeaponId, true, iSkinId, iWeaponSeed, iWeaponStattrakCount, pWeaponName, fWeaponWear);
	}
	else
	{
		m_pApp->SkinChanger()->AddSkinReplacement(
			iTeamNum,
			iWeaponId,
			new CSkinMetadata(
				iWeaponId,
				iSkinId,
				iWeaponSeed,
				iWeaponStattrakCount,
				iWeaponStattrakCount > 0 ? 4 : 0,
				pWeaponName,
				fWeaponWear
			)
		);

		m_pApp->SkinChanger()->SetForceFullUpdate();
	}
}

void CMenu::ApplySkinsBothTeams()
{
	this->ApplySkin(TEAMNUM_CT);
	this->ApplySkin(TEAMNUM_T);
}

void CMenu::FillSelectboxWithFiles(CSelectbox* pSelectbox, const char* pPath)
{
	pSelectbox->ClearOptions();

	int iCount = 0;
	WIN32_FIND_DATA ffd;
	char pSearchQuery[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	sprintf(pSearchQuery, "%s%s*", m_pApp->GetWorkingDirectory(), pPath);

	hFind = FindFirstFile(pSearchQuery, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			pSelectbox->AddOption(iCount++, ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}

void CMenu::LoadConfig()
{
	const char* pConfigName = m_pConfigLoadables->GetSelectedText();
	if (!pConfigName)
		return;

	CConfig loadConfig;
	loadConfig.Init(CApplication::Instance());
	if (loadConfig.LoadFile(pConfigName))
	{
		ConfigHelper::ConfigToFeatures(&loadConfig);
		this->ApplySettings();
	}
}

void CMenu::SaveConfig()
{
	const char* pConfigName = m_pConfigSaveConfigName->GetText();
	if (!pConfigName)
		return;

	CConfig saveConfig;
	saveConfig.Init(CApplication::Instance());
	ConfigHelper::FeaturesToConfig(&saveConfig);

	saveConfig.SaveFile(pConfigName);
	this->FillSelectboxWithFiles(m_pConfigLoadables, CONFIG_FOLDER);
}

void CMenu::PrintNumUpDownValue()
{
	g_pConsole->Write(LOGLEVEL_WARNING, "m_pNumericUpDown->GetValue() = %d\n", m_pNumericUpDown->GetValue());
}