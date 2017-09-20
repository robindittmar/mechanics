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

	m_pAimbotMultipoint->SetChecked(m_pApp->TargetSelector()->GetMultipoint());
	m_pAimbotTargetCriteria->SetSelectionByValue(m_pApp->Ragebot()->GetTargetCriteria());
	m_pAimbotVisibleMode->SetSelectionByValue(m_pApp->TargetSelector()->GetVisibleMode());

	m_pHitboxHead->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_HEAD));
	m_pHitboxChest->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_CHEST));
	m_pHitboxPelvis->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
	m_pHitboxRForearm->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
	m_pHitboxLForearm->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
	m_pHitboxRCalf->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
	m_pHitboxLCalf->SetChecked(m_pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

	m_pAntiaimPitch->SetSelectionByValue(m_pApp->AntiAim()->GetPitchSetting());
	m_pAntiaimYaw->SetSelectionByValue(m_pApp->AntiAim()->GetYawSetting());

	m_pEspDrawBoundingBox->SetChecked(m_pApp->Esp()->GetDrawBoundingBox());
	m_pEspDrawHealthbar->SetChecked(m_pApp->Esp()->GetDrawHealthBar());
	m_pEspDrawHealthnumber->SetChecked(m_pApp->Esp()->GetDrawHealthNumber());
	m_pEspDrawArmorbar->SetChecked(m_pApp->Esp()->GetDrawArmorBar());
	m_pEspDrawOwnTeam->SetChecked(m_pApp->Esp()->GetDrawOwnTeam());
	m_pEspDrawOwnModel->SetChecked(m_pApp->Esp()->GetDrawOwnModel());
	m_pEspDrawOnlySpotted->SetChecked(m_pApp->Esp()->GetDrawOnlySpotted());
	m_pEspDrawOutline->SetChecked(m_pApp->Esp()->GetDrawOutline());
	m_pEspDrawNames->SetChecked(m_pApp->Esp()->GetDrawNames());

	m_pChamsDrawOwnTeam->SetChecked(m_pApp->Chams()->GetRenderTeam());
	m_pChamsDrawOwnModel->SetChecked(m_pApp->Chams()->GetRenderLocalplayer());
	m_pChamsIgnoreZ->SetChecked(m_pApp->Chams()->GetIgnoreZIndex());
	m_pChamsFlatModels->SetChecked(m_pApp->Chams()->GetFlatModels());

	m_pMirror->SetChecked(m_pApp->Mirror()->GetEnabled());
	m_pThirdperson->SetChecked(m_pApp->Visuals()->GetThirdperson());
	m_pHandsDrawStyle->SetSelectionByValue(m_pApp->Visuals()->GetHandsDrawStyle());
	m_pFlashAmount->SetDisplayValue(m_pApp->Visuals()->GetFlashPercentage());
	m_pFov->SetDisplayValue(m_pApp->Visuals()->GetFovValue());
	// TODO: m_pViewmodelFov
	
	m_pNoName->SetChecked(m_pApp->Misc()->GetNoName());
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

			m_pThirdperson->SetChecked(bNewValue);
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
	m_pAimbotEnabled = new CCheckbox(16, 16, 120, 32, "Aimbot");
	m_pAimbotEnabled->SetEventHandler(std::bind(&CRagebot::SetEnabled, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotSilentAim = new CCheckbox(16, 64, 120, 32, "Silent Aim");
	m_pAimbotSilentAim->SetEventHandler(std::bind(&CRagebot::SetSilentAim, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotAutoshoot = new CCheckbox(16, 112, 120, 32, "Autoshoot");
	m_pAimbotAutoshoot->SetEventHandler(std::bind(&CRagebot::SetAutoshoot, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotAutoscope = new CCheckbox(16, 160, 120, 32, "Autoscope");
	m_pAimbotAutoscope->SetEventHandler(std::bind(&CRagebot::SetAutoscope, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotMultipoint = new CCheckbox(152, 16, 120, 32, "Multipoint");
	m_pAimbotMultipoint->SetEventHandler(std::bind(&CTargetSelector::SetMultipoint, m_pApp->TargetSelector(), std::placeholders::_1));

	m_pAimbotTargetCriteria = new CSelectbox(16, 208, 120, 32, "Target criteria");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_UNSPECIFIED, "First found");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_ORIGIN, "Closest to position");
	m_pAimbotTargetCriteria->AddOption(TARGETCRITERIA_VIEWANGLES, "Closest to crosshair");
	m_pAimbotTargetCriteria->SetEventHandler(std::bind(&CRagebot::SetTargetCriteria, m_pApp->Ragebot(), std::placeholders::_1));

	m_pAimbotVisibleMode = new CSelectbox(152, 64, 120, 32, "Visible mode");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_IGNORE, "Ignore");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_CANHIT, "Can Hit (Autowall)");
	m_pAimbotVisibleMode->AddOption(VISIBLEMODE_FULLVISIBLE, "Full Visible");
	m_pAimbotVisibleMode->SetEventHandler(std::bind(&CTargetSelector::SetVisibleMode, m_pApp->TargetSelector(), std::placeholders::_1));

	m_pHitboxHead = new CCheckbox(16, 0, 120, 32, "Head");
	m_pHitboxHead->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_HEAD, std::placeholders::_1));

	m_pHitboxChest = new CCheckbox(16, 36, 120, 32, "Chest");
	m_pHitboxChest->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_CHEST, std::placeholders::_1));

	m_pHitboxPelvis = new CCheckbox(16, 72, 120, 32, "Pelvis");
	m_pHitboxPelvis->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_PELVIS, std::placeholders::_1));

	m_pHitboxRForearm = new CCheckbox(16, 108, 120, 32, "Right forearm");
	m_pHitboxRForearm->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_RIGHT_FOREARM, std::placeholders::_1));

	m_pHitboxLForearm = new CCheckbox(16, 144, 120, 32, "Left forearm");
	m_pHitboxLForearm->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_LEFT_FOREARM, std::placeholders::_1));

	m_pHitboxRCalf = new CCheckbox(16, 180, 120, 32, "Right calf");
	m_pHitboxRCalf->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_RIGHT_CALF, std::placeholders::_1));

	m_pHitboxLCalf = new CCheckbox(16, 216, 120, 32, "Left calf");
	m_pHitboxLCalf->SetEventHandler(std::bind(&CTargetSelector::SetCheckHitbox, m_pApp->TargetSelector(), TARGET_HITBOX_LEFT_CALF, std::placeholders::_1));

	m_pHitboxGroup = new CGroupbox(288, 16, 152, 288, "Hitboxes");
	m_pHitboxGroup->AddChild(m_pHitboxHead);
	m_pHitboxGroup->AddChild(m_pHitboxChest);
	m_pHitboxGroup->AddChild(m_pHitboxPelvis);
	m_pHitboxGroup->AddChild(m_pHitboxRForearm);
	m_pHitboxGroup->AddChild(m_pHitboxLForearm);
	m_pHitboxGroup->AddChild(m_pHitboxRCalf);
	m_pHitboxGroup->AddChild(m_pHitboxLCalf);

	
	m_pAntiaimPitch = new CSelectbox(16, 16, 100, 32, "Pitch");
	m_pAntiaimPitch->AddOption(PITCHANTIAIM_NONE, "None");
	m_pAntiaimPitch->AddOption(PITCHANTIAIM_DOWN, "Down");
	m_pAntiaimPitch->AddOption(PITCHANTIAIM_UP, "Up");
	m_pAntiaimPitch->SetEventHandler(std::bind(&CAntiAim::SetPitchSetting, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimYaw = new CSelectbox(16, 64, 100, 32, "Yaw");
	m_pAntiaimYaw->AddOption(YAWANTIAIM_NONE, "None");
	m_pAntiaimYaw->AddOption(YAWANTIAIM_BACKWARDS, "Backwards");
	m_pAntiaimYaw->AddOption(YAWANTIAIM_STATICJITTERBACKWARDS, "Jitter Backwards");
	m_pAntiaimYaw->AddOption(YAWANTIAIM_REALLEFTFAKERIGHT, "REAL LEFT FAKE RIGHT");
	m_pAntiaimYaw->AddOption(YAWANTIAIM_REALRIGHTFAKELEFT, "REAL RIGHT FAKE LEFT");
	m_pAntiaimYaw->SetEventHandler(std::bind(&CAntiAim::SetYawSetting, m_pApp->AntiAim(), std::placeholders::_1));

	m_pAntiaimGroup = new CGroupbox(450, 16, 152, 288, "AntiAim");
	m_pAntiaimGroup->AddChild(m_pAntiaimPitch);
	m_pAntiaimGroup->AddChild(m_pAntiaimYaw);

	m_pRageTab = new CTabPage("Rage");
	m_pRageTab->AddChild(m_pAimbotEnabled);
	m_pRageTab->AddChild(m_pAimbotSilentAim);
	m_pRageTab->AddChild(m_pAimbotAutoshoot);
	m_pRageTab->AddChild(m_pAimbotAutoscope);
	m_pRageTab->AddChild(m_pAimbotMultipoint);
	m_pRageTab->AddChild(m_pAimbotTargetCriteria);
	m_pRageTab->AddChild(m_pAimbotVisibleMode);
	m_pRageTab->AddChild(m_pHitboxGroup);
	m_pRageTab->AddChild(m_pAntiaimGroup);
}

void CMenu::CreateLegitTab()
{
	m_pLabelWip = new CLabel(0, 0, 1020, 550, "[WIP]", RM_FONT_HEADER, LABEL_ORIENTATION_CENTER, Color(255, 255, 128, 0));

	m_pLegitTab = new CTabPage("Legit");
	m_pLegitTab->AddChild(m_pLabelWip);
}

void CMenu::CreateVisualsTab()
{
	m_pEspDrawBoundingBox = new CCheckbox(4, 4, 128, 32, "Bounding Box");
	m_pEspDrawBoundingBox->SetEventHandler(std::bind(&CEsp::SetDrawBoundingBox, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawHealthbar = new CCheckbox(4, 52, 128, 32, "Health bar");
	m_pEspDrawHealthbar->SetEventHandler(std::bind(&CEsp::SetDrawHealthBar, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawHealthnumber = new CCheckbox(4, 100, 128, 32, "Health number");
	m_pEspDrawHealthnumber->SetEventHandler(std::bind(&CEsp::SetDrawHealthNumber, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawArmorbar = new CCheckbox(4, 160, 128, 32, "Armor bar");
	m_pEspDrawArmorbar->SetEventHandler(std::bind(&CEsp::SetDrawArmorBar, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOwnTeam = new CCheckbox(4, 208, 128, 32, "Own team");
	m_pEspDrawOwnTeam->SetEventHandler(std::bind(&CEsp::SetDrawOwnTeam, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOwnModel = new CCheckbox(4, 256, 128, 32, "Own model (3rd person)");
	m_pEspDrawOwnModel->SetEventHandler(std::bind(&CEsp::SetDrawOwnModel, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOnlySpotted = new CCheckbox(4, 304, 128, 32, "Only spotted");
	m_pEspDrawOnlySpotted->SetEventHandler(std::bind(&CEsp::SetDrawOnlySpotted, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawOutline = new CCheckbox(136, 16, 128, 32, "Outlines");
	m_pEspDrawOutline->SetEventHandler(std::bind(&CEsp::SetDrawOutline, m_pApp->Esp(), std::placeholders::_1));

	m_pEspDrawNames = new CCheckbox(136, 64, 128, 32, "Names");
	m_pEspDrawNames->SetEventHandler(std::bind(&CEsp::SetDrawNames, m_pApp->Esp(), std::placeholders::_1));

	m_pEspGroup = new CGroupbox(16, 16, 292, 312, "Esp");
	m_pEspGroup->AddChild(m_pEspDrawBoundingBox);
	m_pEspGroup->AddChild(m_pEspDrawHealthbar);
	m_pEspGroup->AddChild(m_pEspDrawHealthnumber);
	m_pEspGroup->AddChild(m_pEspDrawArmorbar);
	m_pEspGroup->AddChild(m_pEspDrawOwnTeam);
	m_pEspGroup->AddChild(m_pEspDrawOwnModel);
	m_pEspGroup->AddChild(m_pEspDrawOnlySpotted);
	m_pEspGroup->AddChild(m_pEspDrawOutline);
	m_pEspGroup->AddChild(m_pEspDrawNames);

	m_pChamsDrawOwnTeam = new CCheckbox(4, 4, 128, 32, "Own Team");
	m_pChamsDrawOwnTeam->SetEventHandler(std::bind(&CChams::SetRenderTeam, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsDrawOwnModel = new CCheckbox(4, 52, 128, 32, "Own Model");
	m_pChamsDrawOwnModel->SetEventHandler(std::bind(&CChams::SetRenderLocalplayer, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsIgnoreZ = new CCheckbox(4, 100, 128, 32, "Behind Walls");
	m_pChamsIgnoreZ->SetEventHandler(std::bind(&CChams::SetIgnoreZIndex, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsFlatModels = new CCheckbox(4, 160, 128, 32, "Flat Models");
	m_pChamsFlatModels->SetEventHandler(std::bind(&CChams::SetFlatModels, m_pApp->Chams(), std::placeholders::_1));

	m_pChamsGroup = new CGroupbox(400, 16, 292, 312, "Chams");
	m_pChamsGroup->AddChild(m_pChamsDrawOwnTeam);
	m_pChamsGroup->AddChild(m_pChamsDrawOwnModel);
	m_pChamsGroup->AddChild(m_pChamsIgnoreZ);
	m_pChamsGroup->AddChild(m_pChamsFlatModels);

	m_pPlayerVisualsTab = new CTabPage("Player Visuals");
	m_pPlayerVisualsTab->AddChild(m_pEspGroup);
	m_pPlayerVisualsTab->AddChild(m_pChamsGroup);

	m_pMirror = new CCheckbox(304, 304, 128, 32, "Mirror");
	m_pMirror->SetEventHandler(std::bind(&CMirror::SetEnabled, m_pApp->Mirror(), std::placeholders::_1));

	m_pThirdperson = new CCheckbox(160, 112, 120, 32, "Thirdperson");
	m_pThirdperson->SetEventHandler(std::bind(&CVisuals::SetThirdperson, m_pApp->Visuals(), std::placeholders::_1));

	m_pHandsDrawStyle = new CSelectbox(304, 16, 128, 32);
	m_pHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NONE, "None");
	m_pHandsDrawStyle->AddOption(HANDSDRAWSTYLE_NOHANDS, "NoHands");
	m_pHandsDrawStyle->AddOption(HANDSDRAWSTYLE_WIREFRAME, "Wireframe");
	m_pHandsDrawStyle->SetEventHandler(std::bind(&CVisuals::SetHandsDrawStyle, m_pApp->Visuals(), std::placeholders::_1));

	m_pFlashAmount = new CSlider(304, 64, 128, 32);
	m_pFlashAmount->SetEventHandler(std::bind(&CVisuals::SetFlashPercentage, m_pApp->Visuals(), std::placeholders::_1));

	m_pFov = new CSlider(563, 5, 32, 350, 0.0f, SLIDER_ORIENTATION_VERTICAL, true, 1.0f, 180.0f);
	m_pFov->SetEventHandler(std::bind(&CVisuals::SetFovValue, m_pApp->Visuals(), std::placeholders::_1));

	m_pViewmodelFov = new CSlider(600, 5, 32, 350, 0.0f, SLIDER_ORIENTATION_VERTICAL, true, 1.0f, 180.0f);
	m_pViewmodelFov->SetEventHandler(std::bind(&CVisuals::SetFovValue, m_pApp->Visuals(), std::placeholders::_1));

	m_pOtherVisualsTab = new CTabPage("Others");
	m_pOtherVisualsTab->AddChild(m_pMirror);
	m_pOtherVisualsTab->AddChild(m_pThirdperson);
	m_pOtherVisualsTab->AddChild(m_pHandsDrawStyle);
	m_pOtherVisualsTab->AddChild(m_pFlashAmount);
	m_pOtherVisualsTab->AddChild(m_pFov);
	m_pOtherVisualsTab->AddChild(m_pViewmodelFov);

	m_pVisualsTabContainer = new CTabContainer();
	m_pVisualsTabContainer->AddChild(m_pPlayerVisualsTab);
	m_pVisualsTabContainer->AddChild(m_pOtherVisualsTab);

	m_pVisualsTab = new CTabPage("Visuals");
	m_pVisualsTab->AddChild(m_pVisualsTabContainer);
}

void CMenu::CreateMiscTab()
{
	m_pNoName = new CCheckbox(16, 160, 120, 32, "NoName");
	m_pNoName->SetEventHandler(std::bind(&CMisc::SetNoNameClanTag, m_pApp->Misc(), std::placeholders::_1));

	m_pMiscTab = new CTabPage("Misc");
	m_pMiscTab->AddChild(m_pNoName);
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