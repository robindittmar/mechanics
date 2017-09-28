#include "Menu.h"
#include "Application.h"

CMenu::CMenu()
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
	m_inputHandler.RegisterKey(VK_DELETE, EVENT_BTN_DETACH);
	m_inputHandler.RegisterKey(VK_INSERT, EVENT_BTN_TOGGLEMENU);
	m_inputHandler.RegisterKey(VK_NUMPAD0, EVENT_BTN_THIRDPERSON);
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
	m_pMainContainer->SelectTab(0);

	m_pWindow = new CWindow(30, 30, 1020, 550, ".mechanics");
	m_pWindow->AddChild(m_pMainContainer);

	m_pWindowMirror = new CWindow(100, 100, MIRROR_WIDTH, MIRROR_HEIGHT, "Mirror");
	m_pWindowMirror->SetVisible(true);
}

void CMenu::ApplySettings()
{
	m_pAimbotEnabled->SetChecked(m_pApp->Ragebot()->GetEnabled());
	m_pAimbotSilentAim->SetChecked(m_pApp->Ragebot()->GetSilentAim());
	m_pAimbotAutoshoot->SetChecked(m_pApp->Ragebot()->GetAutoshoot());
	m_pAimbotAutoscope->SetChecked(m_pApp->Ragebot()->GetAutoscope());
	m_pAimbotAutoReload->SetChecked(m_pApp->Ragebot()->GetAutoReload());
	m_pAimbotMultipoint->SetChecked(m_pApp->TargetSelector()->GetMultipoint());
	m_pAimbotNoSpreadEnabled->SetChecked(m_pApp->Ragebot()->GetNoSpread());
	m_pAimbotHitchanceEnabled->SetChecked(m_pApp->Ragebot()->GetCalculateHitchance());
	m_pAimbotHitchanceSlider->SetDisplayValue(m_pApp->Ragebot()->GetHitchance());
	m_pAimbotTargetCriteria->SetSelectionByValue(m_pApp->Ragebot()->GetTargetCriteria());
	m_pAimbotVisibleMode->SetSelectionByValue(m_pApp->TargetSelector()->GetVisibleMode());

	m_pHitboxHead->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_HEAD));
	m_pHitboxChest->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_CHEST));
	m_pHitboxPelvis->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
	m_pHitboxRForearm->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
	m_pHitboxLForearm->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
	m_pHitboxRCalf->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
	m_pHitboxLCalf->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

	m_pRageOthersAutoZeusEnabled->SetChecked(m_pApp->Ragebot()->GetAutoZeus());
	m_pRageOthersAutoRevolverEnabled->SetChecked(m_pApp->Ragebot()->GetAutoRevolver());

	m_pAntiaimEnabled->SetChecked(m_pApp->AntiAim()->GetEnabled());
	// Standing
	m_pAntiaimStandingPitch->SetSelectionByValue(m_pApp->AntiAim()->GetPitchSettingStanding());
	m_pAntiaimStandingPitchOffset->SetDisplayValue(m_pApp->AntiAim()->GetPitchOffsetStanding());
	m_pAntiaimStandingYaw->SetSelectionByValue(m_pApp->AntiAim()->GetYawSettingStanding());
	m_pAntiaimStandingYawOffset->SetDisplayValue(m_pApp->AntiAim()->GetYawOffsetStanding());
	m_pAntiaimStandingYawFake->SetSelection(m_pApp->AntiAim()->GetYawFakeSettingStanding());
	m_pAntiaimYawStandingFakeOffset->SetDisplayValue(m_pApp->AntiAim()->GetYawFakeOffsetStanding());
	// Moving
	m_pAntiaimMovingPitch->SetSelectionByValue(m_pApp->AntiAim()->GetPitchSettingMoving());
	m_pAntiaimMovingPitchOffset->SetDisplayValue(m_pApp->AntiAim()->GetPitchOffsetMoving());
	m_pAntiaimMovingYaw->SetSelectionByValue(m_pApp->AntiAim()->GetYawSettingMoving());
	m_pAntiaimMovingYawOffset->SetDisplayValue(m_pApp->AntiAim()->GetYawOffsetMoving());
	m_pAntiaimMovingYawFake->SetSelection(m_pApp->AntiAim()->GetYawFakeSettingMoving());
	m_pAntiaimYawMovingFakeOffset->SetDisplayValue(m_pApp->AntiAim()->GetYawFakeOffsetMoving());

	m_pAntiaimLbyIndicator->SetChecked(m_pApp->AntiAim()->GetDrawLbyIndicator());
	m_pAntiaimStandingLbyBreaker->SetChecked(m_pApp->AntiAim()->GetLbyBreaker());

	// Legit
	m_pTriggerbotEnabled->SetChecked(m_pApp->Triggerbot()->GetEnabled());
	m_pTriggerbotDelayValue->SetDisplayValue(m_pApp->Triggerbot()->GetShootDelay());
	m_pTriggerbotDelayJitterValue->SetDisplayValue(m_pApp->Triggerbot()->GetShootDelayJitter());

	// Esp
	m_pEspEnabled->SetChecked(m_pApp->Esp()->GetEnabled());
	m_pEspDrawBoundingBox->SetSelection(m_pApp->Esp()->GetDrawBoundingBox());
	m_pEspDrawFilledBox->SetChecked(m_pApp->Esp()->GetFillBoundingBox());
	m_pEspDrawOutline->SetChecked(m_pApp->Esp()->GetDrawOutline());
	m_pEspDrawHealthbar->SetChecked(m_pApp->Esp()->GetDrawHealthBar());
	m_pEspDrawHealthnumber->SetChecked(m_pApp->Esp()->GetDrawHealthNumber());
	m_pEspDrawArmorbar->SetChecked(m_pApp->Esp()->GetDrawArmorBar());
	m_pEspDrawOwnTeam->SetChecked(m_pApp->Esp()->GetDrawOwnTeam());
	m_pEspDrawOwnModel->SetChecked(m_pApp->Esp()->GetDrawOwnModel());
	m_pEspFadeoutEnabled->SetChecked(m_pApp->Esp()->GetFadeoutEnabled());
	m_pEspFadeoutValue->SetDisplayValue(m_pApp->Esp()->GetFadeoutTime());
	m_pEspDrawOnlyVisible->SetChecked(m_pApp->Esp()->GetDrawOnlyVisible());
	m_pEspDrawOnlySpotted->SetChecked(m_pApp->Esp()->GetDrawOnlySpotted());
	m_pEspDrawNames->SetChecked(m_pApp->Esp()->GetDrawNames());
	m_pEspBarrelEnabled->SetChecked(m_pApp->Esp()->GetDrawViewangles());
	m_pEspBarrelValue->SetDisplayValue(m_pApp->Esp()->GetViewanglesLength());

	// WeaponEsp
	m_pWeaponEspEnabled->SetChecked(m_pApp->WeaponEsp()->GetEnabled());
	m_pWeaponEspWeaponBoundingBoxEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawWeaponBoundingBox());
	m_pWeaponEspWeaponNameEnabled->SetChecked(m_pApp->WeaponEsp()->GetDrawWeaponName());

	m_pChamsEnabled->SetChecked(m_pApp->Chams()->GetEnabled());
	m_pChamsStyle->SetSelection(m_pApp->Chams()->GetFlatModels());
	m_pChamsDrawOwnTeam->SetChecked(m_pApp->Chams()->GetRenderTeam());
	m_pChamsDrawOwnModel->SetChecked(m_pApp->Chams()->GetRenderLocalplayer());
	m_pChamsIgnoreZ->SetChecked(m_pApp->Chams()->GetIgnoreZIndex());

	m_pSoundEspEnabled->SetChecked(m_pApp->SoundEsp()->GetEnabled());
	m_pSoundEspShowTime->SetDisplayValue(m_pApp->SoundEsp()->GetShowTime());
	m_pSoundEspFadeoutEnabled->SetChecked(m_pApp->SoundEsp()->GetFadeoutEnabled());
	m_pSoundEspFadeoutTime->SetDisplayValue(m_pApp->SoundEsp()->GetFadeTime());
	m_pSoundEspDrawOwnTeam->SetChecked(m_pApp->SoundEsp()->GetDrawOwnTeam());
	m_pSoundEspOnlyNotVisible->SetChecked(m_pApp->SoundEsp()->GetDrawVisible());

	m_pEffectsNoVisualRecoil->SetChecked(m_pApp->Visuals()->GetNoVisualRecoil());
	m_pEffectsNoSmoke->SetChecked(m_pApp->Visuals()->GetNoSmoke());
	m_pEffectsNoScope->SetChecked(m_pApp->Visuals()->GetNoScope());
	m_pEffectsDisablePostprocessing->SetChecked(m_pApp->Visuals()->GetDisablePostProcessing());
	m_pEffectsNoFlash->SetChecked(m_pApp->Visuals()->GetNoFlash());
	m_pEffectsNoFlashValue->SetDisplayValue(m_pApp->Visuals()->GetFlashPercentage());

	m_pVisualsOthersHandsDrawStyle->SetSelectionByValue(m_pApp->Visuals()->GetHandsDrawStyle());
	m_pVisualsOthersHitmarkerEnabled->SetChecked(m_pApp->Visuals()->GetHitmarker());
	m_pVisualsOthersCrosshairEnabled->SetChecked(m_pApp->Visuals()->GetCrosshair());
	m_pVisualsOthersRecoilCrosshairEnabled->SetChecked(m_pApp->Visuals()->GetCrosshairShowRecoil());
	m_pVisualsOthersThirdperson->SetChecked(m_pApp->Visuals()->GetThirdperson());
	m_pVisualsOthersThirdpersonDistance->SetDisplayValue(m_pApp->Visuals()->GetThirdpersonDistance());
	m_pVisualsOthersMirror->SetChecked(m_pApp->Mirror()->GetEnabled());

	m_pFovChangerFovEnabled->SetChecked(m_pApp->Visuals()->GetFovChange());
	m_pFovChangerFovScopeEnabled->SetChecked(m_pApp->Visuals()->GetFovChangeScoped());
	m_pFovChangerFovValue->SetDisplayValue(m_pApp->Visuals()->GetFovValue());
	m_pFovChangerViewmodelFovEnabled->SetChecked(m_pApp->Visuals()->GetViewmodelFov());
	m_pFovChangerViewmodelFovValue->SetDisplayValue(m_pApp->Visuals()->GetViewmodelFovValue());

	// Misc
	m_pFakelagEnabled->SetChecked(m_pApp->Misc()->GetFakelag());
	m_pFakelagChokeAmount->SetDisplayValue(m_pApp->Misc()->GetFakelagChokeAmount());

	m_pMiscOthersNoRecoilEnabled->SetChecked(m_pApp->Misc()->GetNoRecoil());
	m_pMiscOthersAutoPistolEnabled->SetChecked(m_pApp->Misc()->GetAutoPistol());
	m_pMiscOthersJumpScoutEnabled->SetChecked(m_pApp->Misc()->GetJumpScout());
	m_pMiscOthersAutoAcceptEnabled->SetChecked(m_pApp->Misc()->GetAutoAccept());
	m_pMiscOthersNoNameEnabled->SetChecked(m_pApp->Misc()->GetNoName());

	m_pMiscBhopEnabled->SetChecked(m_pApp->Bhop()->GetEnabled());
	m_pMiscBhopAutoStrafeMode->SetSelection(m_pApp->Misc()->GetAutoStrafeMode());
}

void CMenu::HandleInput()
{
	// Input handling
	m_inputEvent.Clear();
	m_inputHandler.CreateInput(&m_inputEvent, m_pWindow->GetVisible());
	if (m_inputEvent.eventType == EVENT_TYPE_KEYBOARD)
	{
		if (m_inputEvent.buttons & EVENT_BTN_TOGGLEMENU &&
			m_inputEvent.buttonProperties & EVENT_BTN_TOGGLEMENU)
		{
			bool bVis = !m_pWindow->GetVisible();
			m_pWindow->SetVisible(bVis);

			// Mouse stuff
			m_pApp->Gui()->SetEnableMouse(!bVis);
			m_pApp->Gui()->SetDrawMouse(bVis);
		}
		else if (m_inputEvent.buttons & EVENT_BTN_DETACH &&
			m_inputEvent.buttonProperties & EVENT_BTN_DETACH)
		{
			m_pApp->Detach();
		}
		else if (m_inputEvent.buttons & EVENT_BTN_THIRDPERSON &&
			!(m_inputEvent.buttonProperties & EVENT_BTN_THIRDPERSON))
		{
			bool bNewValue = !m_pApp->Visuals()->GetThirdperson();

			m_pVisualsOthersThirdperson->SetChecked(bNewValue);
			m_pApp->Visuals()->SetThirdperson(bNewValue);
		}
	}

	if (m_pWindow->GetVisible())
	{
		m_pWindow->ProcessEvent(&m_inputEvent);
		m_pWindowMirror->ProcessEvent(&m_inputEvent);
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

	m_pAimbotSilentAim = new CCheckbox(4, 20, 128, 16, "Silent Aim");
	m_pAimbotSilentAim->SetEventHandler(std::bind(&CRagebot::SetSilentAim, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotAutoshoot = new CCheckbox(4, 40, 128, 16, "Auto Shoot");
	m_pAimbotAutoshoot->SetEventHandler(std::bind(&CRagebot::SetAutoshoot, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotAutoscope = new CCheckbox(4, 60, 128, 16, "Auto Scope");
	m_pAimbotAutoscope->SetEventHandler(std::bind(&CRagebot::SetAutoscope, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotAutoReload = new CCheckbox(4, 80, 128, 16, "Auto Reload");
	m_pAimbotAutoReload->SetEventHandler(std::bind(&CRagebot::SetAutoReload, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotMultipoint = new CCheckbox(4, 100, 128, 16, "Multipoint");
	m_pAimbotMultipoint->SetEventHandler(std::bind(&CTargetSelector::SetMultipoint, m_pApp->TargetSelector(), std::placeholders::_1));

	m_pAimbotNoSpreadEnabled = new CCheckbox(4, 120, 128, 16, "Remove Spread (if possible)");
	m_pAimbotNoSpreadEnabled->SetEventHandler(std::bind(&CRagebot::SetNoSpread, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotHitchanceEnabled = new CCheckbox(4, 140, 128, 16, "Hitchance");
	m_pAimbotHitchanceEnabled->SetEventHandler(std::bind(&CRagebot::SetCalculateHitchance, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotHitchanceSlider = new CSlider(4, 162, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f);
	m_pAimbotHitchanceSlider->SetEventHandler(std::bind(&CRagebot::SetHitchance, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotTargetCriteria = new CSelectbox(4, 192, 128, 20, "Target criteria");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_UNSPECIFIED, "First found");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_ORIGIN, "Closest to position");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_VIEWANGLES, "Closest to crosshair");
	m_pAimbotTargetCriteria->SetEventHandler(std::bind(&CRagebot::SetTargetCriteria, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotVisibleMode = new CSelectbox(4, 228, 128, 20, "Visible mode");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_IGNORE, "Ignore");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_CANHIT, "Can Hit (Autowall)");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_FULLVISIBLE, "Full Visible");
	m_pAimbotVisibleMode->SetEventHandler(std::bind(&CTargetSelector::SetVisibleMode, m_pApp->TargetSelector(), std::placeholders::_1));

	m_pAimbotGroup = new CGroupbox(16, 16, 152, 268, "Aimbot");
	m_pAimbotGroup->AddChild(m_pAimbotEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotSilentAim);
	m_pAimbotGroup->AddChild(m_pAimbotAutoshoot);
	m_pAimbotGroup->AddChild(m_pAimbotAutoscope);
	m_pAimbotGroup->AddChild(m_pAimbotAutoReload);
	m_pAimbotGroup->AddChild(m_pAimbotMultipoint);
	m_pAimbotGroup->AddChild(m_pAimbotNoSpreadEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotHitchanceEnabled);
	m_pAimbotGroup->AddChild(m_pAimbotHitchanceSlider);
	m_pAimbotGroup->AddChild(m_pAimbotTargetCriteria);
	m_pAimbotGroup->AddChild(m_pAimbotVisibleMode);


	m_pHitboxHead = new CCheckbox(4, 0, 128, 16, "Head");
	m_pHitboxHead->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_HEAD, std::placeholders::_1));

	m_pHitboxChest = new CCheckbox(4, 20, 128, 16, "Chest");
	m_pHitboxChest->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_CHEST, std::placeholders::_1));

	m_pHitboxPelvis = new CCheckbox(4, 40, 128, 16, "Pelvis");
	m_pHitboxPelvis->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_PELVIS, std::placeholders::_1));

	m_pHitboxRForearm = new CCheckbox(4, 60, 128, 16, "Right forearm");
	m_pHitboxRForearm->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_RIGHT_FOREARM, std::placeholders::_1));

	m_pHitboxLForearm = new CCheckbox(4, 80, 128, 16, "Left forearm");
	m_pHitboxLForearm->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_LEFT_FOREARM, std::placeholders::_1));

	m_pHitboxRCalf = new CCheckbox(4, 100, 128, 16, "Right calf");
	m_pHitboxRCalf->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_RIGHT_CALF, std::placeholders::_1));

	m_pHitboxLCalf = new CCheckbox(4, 120, 128, 16, "Left calf");
	m_pHitboxLCalf->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_LEFT_CALF, std::placeholders::_1));

	m_pHitboxGroup = new CGroupbox(184, 16, 152, 268, "Hitboxes");
	m_pHitboxGroup->AddChild(m_pHitboxHead);
	m_pHitboxGroup->AddChild(m_pHitboxChest);
	m_pHitboxGroup->AddChild(m_pHitboxPelvis);
	m_pHitboxGroup->AddChild(m_pHitboxRForearm);
	m_pHitboxGroup->AddChild(m_pHitboxLForearm);
	m_pHitboxGroup->AddChild(m_pHitboxRCalf);
	m_pHitboxGroup->AddChild(m_pHitboxLCalf);

	m_pRageOthersAutoZeusEnabled = new CCheckbox(4, 0, 128, 16, "Auto Zeus");
	m_pRageOthersAutoZeusEnabled->SetEventHandler(std::bind(&CRagebot::SetAutoZeus, m_pApp->Ragebot(), std::placeholders::_1));

	m_pRageOthersAutoRevolverEnabled = new CCheckbox(4, 20, 128, 16, "Auto Revolver");
	m_pRageOthersAutoRevolverEnabled->SetEventHandler(std::bind(&CRagebot::SetAutoRevolver, m_pApp->Ragebot(), std::placeholders::_1));


	m_pAntiaimEnabled = new CCheckbox(4, 0, 60, 16, "Enabled");
	m_pAntiaimEnabled->SetEventHandler(std::bind(&CAntiAim::SetEnabled, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingPitch = new CSelectbox(4, 10, 128, 20, "Pitch");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_NONE, "None");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_DOWN, "Down");
	m_pAntiaimMovingPitch->AddOption(PITCHANTIAIM_UP, "Up");
	m_pAntiaimMovingPitch->SetEventHandler(std::bind(&CAntiAim::SetPitchSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingPitchOffset = new CSlider(4, 42, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -90.0f, 90.0f);
	m_pAntiaimMovingPitchOffset->SetEventHandler(std::bind(&CAntiAim::SetPitchOffsetMoving, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingYaw = new CSelectbox(4, 72, 128, 20, "Yaw");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_NONE, "None");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_STATIC, "Static");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	m_pAntiaimMovingYaw->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	m_pAntiaimMovingYaw->SetEventHandler(std::bind(&CAntiAim::SetYawSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingYawOffset = new CSlider(4, 104, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimMovingYawOffset->SetEventHandler(std::bind(&CAntiAim::SetPitchOffsetMoving, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimMovingYawFake = new CSelectbox(4, 134, 128, 20, "Yaw Fake");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_NONE, "None");
	m_pAntiaimMovingYawFake->AddOption(FAKEYAWANTIAIM_STATIC, "Static");
	m_pAntiaimMovingYawFake->SetEventHandler(std::bind(&CAntiAim::SetYawFakeSettingMoving, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimYawMovingFakeOffset = new CSlider(4, 166, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimYawMovingFakeOffset->SetEventHandler(std::bind(&CAntiAim::SetYawFakeOffsetMoving, m_pApp->AntiAim(), std::placeholders::_1));

	//Standing
	m_pAntiaimStandingPitch = new CSelectbox(4, 10, 128, 20, "Pitch");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_NONE, "None");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_DOWN, "Down");
	m_pAntiaimStandingPitch->AddOption(PITCHANTIAIM_UP, "Up");
	m_pAntiaimStandingPitch->SetEventHandler(std::bind(&CAntiAim::SetPitchSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingPitchOffset = new CSlider(4, 42, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -90.0f, 90.0f);
	m_pAntiaimStandingPitchOffset->SetEventHandler(std::bind(&CAntiAim::SetPitchOffsetStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingYaw = new CSelectbox(4, 72, 128, 20, "Yaw");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_NONE, "None");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_STATIC, "Static");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	m_pAntiaimStandingYaw->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	m_pAntiaimStandingYaw->SetEventHandler(std::bind(&CAntiAim::SetYawSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingYawOffset = new CSlider(4, 104, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimStandingYawOffset->SetEventHandler(std::bind(&CAntiAim::SetYawOffsetStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingYawFake = new CSelectbox(4, 134, 128, 20, "Yaw Fake");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_NONE, "None");
	m_pAntiaimStandingYawFake->AddOption(FAKEYAWANTIAIM_STATIC, "Static");
	m_pAntiaimStandingYawFake->SetEventHandler(std::bind(&CAntiAim::SetYawFakeSettingStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimYawStandingFakeOffset = new CSlider(4, 166, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, true, -180.0f, 180.0f);
	m_pAntiaimYawStandingFakeOffset->SetEventHandler(std::bind(&CAntiAim::SetYawFakeOffsetStanding, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingLbyBreaker = new CCheckbox(4, 185, 128, 16, "LBY Breaker");
	m_pAntiaimStandingLbyBreaker->SetEventHandler(std::bind(&CAntiAim::SetLbyBreaker, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimLbyIndicator = new CCheckbox(62, 0, 128, 16, "LBY Indicator");
	m_pAntiaimLbyIndicator->SetEventHandler(std::bind(&CAntiAim::SetDrawLbyIndicator, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimStandingGroup = new CGroupbox(4, 20, 152, 230, "Standing");
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingPitch);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingPitchOffset);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYaw);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYawOffset);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingYawFake);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimYawStandingFakeOffset);
	m_pAntiaimStandingGroup->AddChild(m_pAntiaimStandingLbyBreaker);

	m_pAntiaimMovingGroup = new CGroupbox(160, 20, 152, 230, "Moving");
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingPitch);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingPitchOffset);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYaw);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYawOffset);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimMovingYawFake);
	m_pAntiaimMovingGroup->AddChild(m_pAntiaimYawMovingFakeOffset);

	m_pRageOthersGroup = new CGroupbox(352, 16, 152, 268, "AntiAim");
	m_pRageOthersGroup->AddChild(m_pRageOthersAutoZeusEnabled);
	m_pRageOthersGroup->AddChild(m_pRageOthersAutoRevolverEnabled);

	m_pAntiaimGroup = new CGroupbox(520, 16, 324, 268, "AntiAim");
	m_pAntiaimGroup->AddChild(m_pAntiaimEnabled);
	m_pAntiaimGroup->AddChild(m_pAntiaimLbyIndicator);
	m_pAntiaimGroup->AddChild(m_pAntiaimStandingGroup);
	m_pAntiaimGroup->AddChild(m_pAntiaimMovingGroup);

	m_pRageTab = new CTabPage("Rage");
	m_pRageTab->AddChild(m_pAimbotGroup);
	m_pRageTab->AddChild(m_pHitboxGroup);
	m_pRageTab->AddChild(m_pRageOthersGroup);
	m_pRageTab->AddChild(m_pAntiaimGroup);
}

void CMenu::CreateLegitTab()
{
	m_pTriggerbotEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pTriggerbotEnabled->SetEventHandler(std::bind(&CTriggerbot::SetEnabled, m_pApp->Triggerbot(), std::placeholders::_1));

	m_pTriggerbotDelayLabel = new CLabel(4, 24, 128, 16, "Delay (milliseconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pTriggerbotDelayValue = new CSlider(4, 48, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 750.0f);
	m_pTriggerbotDelayValue->SetEventHandler(std::bind(&CTriggerbot::SetShootDelay, m_pApp->Triggerbot(), std::placeholders::_1));

	m_pTriggerbotDelayJitterLabel = new CLabel(4, 60, 128, 16, "Delay Jitter (milliseconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pTriggerbotDelayJitterValue = new CSlider(4, 84, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 20.0f);
	m_pTriggerbotDelayJitterValue->SetEventHandler(std::bind(&CTriggerbot::SetShootDelayJitter, m_pApp->Triggerbot(), std::placeholders::_1));

	m_pTriggerbotGroup = new CGroupbox(16, 16, 152, 268, "Triggerbot");
	m_pTriggerbotGroup->AddChild(m_pTriggerbotEnabled);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayLabel);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayValue);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayJitterLabel);
	m_pTriggerbotGroup->AddChild(m_pTriggerbotDelayJitterValue);

	m_pLegitTab = new CTabPage("Legit");
	m_pLegitTab->AddChild(m_pTriggerbotGroup);
}

void CMenu::CreateVisualsTab()
{
	m_pEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pEspEnabled->SetEventHandler(std::bind(&CEsp::SetEnabled, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawBoundingBox = new CSelectbox(4, 36, 128, 20, "Bounding Box");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_NONE, "None");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_EDGE, "Edge");
	m_pEspDrawBoundingBox->AddOption(ESP_STYLE_FULL, "Full");
	m_pEspDrawBoundingBox->SetEventHandler(std::bind(&CEsp::SetDrawBoundingBox, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawFilledBox = new CCheckbox(4, 56, 128, 16, "Fill Bounding Box");
	m_pEspDrawFilledBox->SetEventHandler(std::bind(&CEsp::SetFillBoundingBox, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOutline = new CCheckbox(4, 76, 128, 16, "Outlines");
	m_pEspDrawOutline->SetEventHandler(std::bind(&CEsp::SetDrawOutline, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawHealthbar = new CCheckbox(4, 104, 128, 16, "Health Bar");
	m_pEspDrawHealthbar->SetEventHandler(std::bind(&CEsp::SetDrawHealthBar, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawHealthnumber = new CCheckbox(4, 124, 128, 16, "Health Number");
	m_pEspDrawHealthnumber->SetEventHandler(std::bind(&CEsp::SetDrawHealthNumber, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawArmorbar = new CCheckbox(4, 144, 128, 16, "Armor Bar");
	m_pEspDrawArmorbar->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawArmornumber = new CCheckbox(4, 164, 128, 16, "Armor Number (WIP)");
	//m_pEspDrawArmornumber->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, m_pApp->Esp(), std::placeholders::_1)); //todo EVENTHANDLER !!!!!

	m_pEspFadeoutEnabled = new CCheckbox(4, 192, 128, 16, "Fadeout");
	m_pEspFadeoutEnabled->SetEventHandler(std::bind(&CEsp::SetFadeoutEnabled, m_pApp->Esp(), std::placeholders::_1));

	m_pEspFadeoutLabel = new CLabel(4, 206, 128, 16, "Fadeout time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pEspFadeoutValue = new CSlider(4, 230, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pEspFadeoutValue->SetEventHandler(std::bind(&CEsp::SetFadeoutTime, m_pApp->Esp(), std::placeholders::_1));

	// Second column
	m_pEspBarrelEnabled = new CCheckbox(156, 192, 128, 16, "Show Viewangle");
	m_pEspBarrelEnabled->SetEventHandler(std::bind(&CEsp::SetDrawViewangles, m_pApp->Esp(), std::placeholders::_1));

	m_pEspBarrelLabel = new CLabel(156, 206, 128, 16, "Viewangle Length", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pEspBarrelValue = new CSlider(156, 230, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 500.0f);
	m_pEspBarrelValue->SetEventHandler(std::bind(&CEsp::SetViewanglesLength, m_pApp->Esp(), std::placeholders::_1));


	m_pEspDrawOwnTeam = new CCheckbox(156, 56, 128, 16, "Own Team");
	m_pEspDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOwnModel = new CCheckbox(156, 76, 128, 16, "Own Model (3rd person)");
	m_pEspDrawOwnModel->SetEventHandler(std::bind(&CEsp::SetDrawOwnModel, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOnlyVisible = new CCheckbox(156, 104, 128, 16, "Only Visible");
	m_pEspDrawOnlyVisible->SetEventHandler(std::bind(&CEsp::SetDrawOnlyVisible, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOnlySpotted = new CCheckbox(156, 124, 128, 16, "Only Spotted");
	m_pEspDrawOnlySpotted->SetEventHandler(std::bind(&CEsp::SetDrawOnlySpotted, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawNames = new CCheckbox(156, 164, 208, 16, "Names");
	m_pEspDrawNames->SetEventHandler(std::bind(&CEsp::SetDrawNames, m_pApp->Esp(), std::placeholders::_1));

	m_pEspGroup = new CGroupbox(16, 16, 304, 268, "Esp");
	m_pEspGroup->AddChild(m_pEspEnabled);
	m_pEspGroup->AddChild(m_pEspDrawBoundingBox);
	m_pEspGroup->AddChild(m_pEspDrawFilledBox);
	m_pEspGroup->AddChild(m_pEspDrawOutline);
	m_pEspGroup->AddChild(m_pEspDrawHealthbar);
	m_pEspGroup->AddChild(m_pEspDrawHealthnumber);
	m_pEspGroup->AddChild(m_pEspDrawArmorbar);
	m_pEspGroup->AddChild(m_pEspDrawArmornumber);
	m_pEspGroup->AddChild(m_pEspFadeoutEnabled);
	m_pEspGroup->AddChild(m_pEspFadeoutLabel);
	m_pEspGroup->AddChild(m_pEspFadeoutValue);
	m_pEspGroup->AddChild(m_pEspDrawOwnTeam);
	m_pEspGroup->AddChild(m_pEspDrawOwnModel);
	m_pEspGroup->AddChild(m_pEspDrawOnlyVisible);
	m_pEspGroup->AddChild(m_pEspDrawOnlySpotted);
	m_pEspGroup->AddChild(m_pEspDrawNames);
	m_pEspGroup->AddChild(m_pEspBarrelEnabled);
	m_pEspGroup->AddChild(m_pEspBarrelLabel);
	m_pEspGroup->AddChild(m_pEspBarrelValue);


	m_pWeaponEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pWeaponEspEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetEnabled, m_pApp->WeaponEsp(), std::placeholders::_1));

	m_pWeaponEspWeaponBoundingBoxEnabled = new CCheckbox(4, 22, 128, 16, "Bounding Box");
	m_pWeaponEspWeaponBoundingBoxEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawWeaponBoundingBox, m_pApp->WeaponEsp(), std::placeholders::_1));

	m_pWeaponEspWeaponNameEnabled = new CCheckbox(4, 42, 128, 16, "Names");
	m_pWeaponEspWeaponNameEnabled->SetEventHandler(std::bind(&CWeaponEsp::SetDrawWeaponName, m_pApp->WeaponEsp(), std::placeholders::_1));

	m_pWeaponEspGroup = new CGroupbox(336, 16, 152, 268, "Weapon Esp");
	m_pWeaponEspGroup->AddChild(m_pWeaponEspEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspWeaponBoundingBoxEnabled);
	m_pWeaponEspGroup->AddChild(m_pWeaponEspWeaponNameEnabled);

	m_pChamsEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pChamsEnabled->SetEventHandler(std::bind(&CChams::SetEnabled, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsStyle = new CSelectbox(4, 36, 128, 20, "Chams Style");
	m_pChamsStyle->AddOption(0, "Lit");
	m_pChamsStyle->AddOption(1, "Flat");
	m_pChamsStyle->SetEventHandler(std::bind(&CChams::SetFlatModels, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsDrawOwnTeam = new CCheckbox(4, 56, 128, 16, "Own Team");
	m_pChamsDrawOwnTeam->SetEventHandler(std::bind(&CChams::SetRenderTeam, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsDrawOwnModel = new CCheckbox(4, 76, 128, 16, "Own Model (3rd person)");
	m_pChamsDrawOwnModel->SetEventHandler(std::bind(&CChams::SetRenderLocalplayer, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsIgnoreZ = new CCheckbox(4, 104, 128, 16, "Only Visible");
	m_pChamsIgnoreZ->SetEventHandler(std::bind(&CChams::SetIgnoreZIndex, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsGroup = new CGroupbox(504, 16, 152, 268, "Chams");
	m_pChamsGroup->AddChild(m_pChamsEnabled);
	m_pChamsGroup->AddChild(m_pChamsStyle);
	m_pChamsGroup->AddChild(m_pChamsDrawOwnTeam);
	m_pChamsGroup->AddChild(m_pChamsDrawOwnModel);
	m_pChamsGroup->AddChild(m_pChamsIgnoreZ);

	m_pSoundEspEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pSoundEspEnabled->SetEventHandler(std::bind(&CSoundEsp::SetEnabled, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspShowTimeLabel = new CLabel(4, 20, 128, 16, "Show time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pSoundEspShowTime = new CSlider(4, 42, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pSoundEspShowTime->SetEventHandler(std::bind(&CSoundEsp::SetShowTime, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspFadeoutEnabled = new CCheckbox(4, 56, 128, 16, "Fadeout");
	m_pSoundEspFadeoutEnabled->SetEventHandler(std::bind(&CSoundEsp::SetFadeoutEnabled, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspFadeoutTimeLabel = new CLabel(4, 70, 128, 16, "Fadeout time (seconds)", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pSoundEspFadeoutTime = new CSlider(4, 92, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.1f, 5.0f);
	m_pSoundEspFadeoutTime->SetEventHandler(std::bind(&CSoundEsp::SetFadeTime, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspDrawOwnTeam = new CCheckbox(4, 124, 128, 16, "Own Team");
	m_pSoundEspDrawOwnTeam->SetEventHandler(std::bind(&CSoundEsp::SetDrawOwnTeam, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspOnlyNotVisible = new CCheckbox(4, 144, 128, 16, "Only Not Visible");
	m_pSoundEspOnlyNotVisible->SetEventHandler(std::bind(&CSoundEsp::SetDrawVisible, m_pApp->SoundEsp(), std::placeholders::_1));

	m_pSoundEspGroup = new CGroupbox(672, 16, 152, 268, "Sound Esp");
	m_pSoundEspGroup->AddChild(m_pSoundEspEnabled);
	m_pSoundEspGroup->AddChild(m_pSoundEspShowTimeLabel);
	m_pSoundEspGroup->AddChild(m_pSoundEspShowTime);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutEnabled);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutTimeLabel);
	m_pSoundEspGroup->AddChild(m_pSoundEspFadeoutTime);
	m_pSoundEspGroup->AddChild(m_pSoundEspDrawOwnTeam);
	m_pSoundEspGroup->AddChild(m_pSoundEspOnlyNotVisible);

	m_pPlayerVisualsTab = new CTabPage("Player Visuals");
	m_pPlayerVisualsTab->AddChild(m_pEspGroup);
	m_pPlayerVisualsTab->AddChild(m_pWeaponEspGroup);
	m_pPlayerVisualsTab->AddChild(m_pChamsGroup);
	m_pPlayerVisualsTab->AddChild(m_pSoundEspGroup);


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

	m_pEffectsNoFlashValue = new CSlider(4, 124, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 100.0f);
	m_pEffectsNoFlashValue->SetEventHandler(std::bind(&CVisuals::NoFlash, m_pApp->Visuals(), std::placeholders::_1));

	// OthersGroup
	m_pVisualsOthersHandsDrawStyle = new CSelectbox(4, 12, 128, 16, "Hands Drawstyle");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NONE, "None");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NOHANDS, "NoHands");
	m_pVisualsOthersHandsDrawStyle->AddOption(HANDSDRAWSTYLE_WIREFRAME, "Wireframe");
	m_pVisualsOthersHandsDrawStyle->SetEventHandler(std::bind(&CVisuals::SetHandsDrawStyle, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersHitmarkerEnabled = new CCheckbox(4, 40, 128, 16, "Hitmarker");
	m_pVisualsOthersHitmarkerEnabled->SetEventHandler(std::bind(&CVisuals::SetHitmarker, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersCrosshairEnabled = new CCheckbox(4, 68, 128, 16, "Custom Crosshair");
	m_pVisualsOthersCrosshairEnabled->SetEventHandler(std::bind(&CVisuals::SetCrosshair, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersRecoilCrosshairEnabled = new CCheckbox(4, 88, 128, 16, "Show Recoil Crosshair");
	m_pVisualsOthersRecoilCrosshairEnabled->SetEventHandler(std::bind(&CVisuals::SetCrosshairShowRecoil, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersThirdperson = new CCheckbox(4, 116, 128, 16, "Thirdperson");
	m_pVisualsOthersThirdperson->SetEventHandler(std::bind(&CVisuals::SetThirdperson, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersThirdpersonLabel = new CLabel(4, 130, 128, 16, "Thirdperson Distance", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pVisualsOthersThirdpersonDistance = new CSlider(4, 152, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 30.0f, 300.0f);
	m_pVisualsOthersThirdpersonDistance->SetEventHandler(std::bind(&CVisuals::SetThirdpersonDistance, m_pApp->Visuals(), std::placeholders::_1));

	m_pVisualsOthersMirror = new CCheckbox(4, 176, 128, 16, "Mirror");
	m_pVisualsOthersMirror->SetEventHandler(std::bind(&CMirror::SetEnabled, m_pApp->Mirror(), std::placeholders::_1));



	// FovChangerGroup
	m_pFovChangerFovEnabled = new CCheckbox(4, 0, 128, 16, "FOV");
	m_pFovChangerFovEnabled->SetEventHandler(std::bind(&CVisuals::SetFovChange, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerFovScopeEnabled = new CCheckbox(4, 20, 128, 16, "Scoping FOV");
	m_pFovChangerFovScopeEnabled->SetEventHandler(std::bind(&CVisuals::SetFovChangeScoped, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerFovLabel = new CLabel(4, 34, 128, 16, "FOV Value", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pFovChangerFovValue = new CSlider(4, 56, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 170.0f);
	m_pFovChangerFovValue->SetEventHandler(std::bind(&CVisuals::SetFovValue, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerViewmodelFovEnabled = new CCheckbox(4, 68, 128, 16, "Viewmodel-FOV");
	m_pFovChangerViewmodelFovEnabled->SetEventHandler(std::bind(&CVisuals::SetViewmodelFov, m_pApp->Visuals(), std::placeholders::_1));

	m_pFovChangerViewmodelFovLabel = new CLabel(4, 82, 128, 16, "FOV Viewmodel Value", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pFovChangerViewmodelFovValue = new CSlider(4, 106, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 150.0f);
	m_pFovChangerViewmodelFovValue->SetEventHandler(std::bind(&CVisuals::SetViewmodelFovValue, m_pApp->Visuals(), std::placeholders::_1));


	// Groups
	m_pEffectsGroup = new CGroupbox(16, 16, 152, 268, "Effects");
	m_pEffectsGroup->AddChild(m_pEffectsNoVisualRecoil);
	m_pEffectsGroup->AddChild(m_pEffectsNoSmoke);
	m_pEffectsGroup->AddChild(m_pEffectsNoScope);
	m_pEffectsGroup->AddChild(m_pEffectsDisablePostprocessing);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlash);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlashLabel);
	m_pEffectsGroup->AddChild(m_pEffectsNoFlashValue);

	m_pVisualsOthersGroup = new CGroupbox(184, 16, 152, 268, "Others");
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHandsDrawStyle);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersHitmarkerEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersCrosshairEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersRecoilCrosshairEnabled);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdperson);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdpersonLabel);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersThirdpersonDistance);
	m_pVisualsOthersGroup->AddChild(m_pVisualsOthersMirror);

	m_pFovChangerGroup = new CGroupbox(352, 16, 152, 268, "Fov Changer");
	m_pFovChangerGroup->AddChild(m_pFovChangerFovEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovScopeEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovLabel);
	m_pFovChangerGroup->AddChild(m_pFovChangerFovValue);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovEnabled);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovLabel);
	m_pFovChangerGroup->AddChild(m_pFovChangerViewmodelFovValue);


	m_pOtherVisualsTab = new CTabPage("Other Visuals");
	m_pOtherVisualsTab->AddChild(m_pEffectsGroup);
	m_pOtherVisualsTab->AddChild(m_pVisualsOthersGroup);
	m_pOtherVisualsTab->AddChild(m_pFovChangerGroup);

	m_pVisualsTabContainer = new CTabContainer();
	m_pVisualsTabContainer->AddChild(m_pPlayerVisualsTab);
	m_pVisualsTabContainer->AddChild(m_pOtherVisualsTab);
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
	m_pMiscOthersNoNameEnabled->SetEventHandler(std::bind(&CMisc::SetNoNameClanTag, m_pApp->Misc(), std::placeholders::_1));

	m_pFakelagEnabled = new CCheckbox(4, 0, 128, 16, "Enabled");
	m_pFakelagEnabled->SetEventHandler(std::bind(&CMisc::SetFakelag, m_pApp->Misc(), std::placeholders::_1));

	m_pFakelagLabel = new CLabel(4, 20, 128, 16, "Packets choking", RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);

	m_pFakelagChokeAmount = new CSlider(4, 42, 128, 16, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 1.0f, 16.0f);
	m_pFakelagChokeAmount->SetEventHandler(std::bind(&CMisc::SetFakelagChokeAmount, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscOthersGroup = new CGroupbox(16, 16, 152, 268, "Others");
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
	m_pMiscBhopAutoStrafeMode->AddOption(AUTOSTRAFEMODE_RAGE, "Rage (WIP)");
	m_pMiscBhopAutoStrafeMode->SetEventHandler(std::bind(&CMisc::SetAutoStrafeMode, m_pApp->Misc(), std::placeholders::_1));

	m_pFakelagGroup = new CGroupbox(184, 16, 152, 268, "Fakelag");
	m_pFakelagGroup->AddChild(m_pFakelagEnabled);
	m_pFakelagGroup->AddChild(m_pFakelagLabel);
	m_pFakelagGroup->AddChild(m_pFakelagChokeAmount);

	m_pMiscBhopAutoStrafeGroup = new CGroupbox(4, 20, 136, 50, "Auto Strafe");
	m_pMiscBhopAutoStrafeGroup->AddChild(m_pMiscBhopAutoStrafeMode);

	m_pMiscBhopGroup = new CGroupbox(352, 16, 152, 268, "Bhop");
	m_pMiscBhopGroup->AddChild(m_pMiscBhopEnabled);
	m_pMiscBhopGroup->AddChild(m_pMiscBhopAutoStrafeGroup);

	m_pMiscTab = new CTabPage("Misc");
	m_pMiscTab->AddChild(m_pMiscOthersGroup);
	m_pMiscTab->AddChild(m_pFakelagGroup);
	m_pMiscTab->AddChild(m_pMiscBhopGroup);
}

void CMenu::CreateSkinChangerTab()
{
	// Sieht fertig aus oder? :D

	m_pSkinChangerTab = new CTabPage("Skin Changer");
}

void CMenu::CreateConfigTab()
{
	m_pDetachBtn = new CButton(16, 64, 120, 45, "Detach");
	m_pDetachBtn->SetButtonClickEventHandler(DetachBtnClick);

	m_pConfigTab = new CTabPage("Config");
	m_pConfigTab->AddChild(m_pDetachBtn);
}

void DetachBtnClick(IControl* p)
{
	CApplication::Instance()->Detach();
}