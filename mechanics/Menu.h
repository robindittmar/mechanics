#ifndef __MENU_H__
#define __MENU_H__

#include "InputEvent.h"
#include "InputHandler.h"
#include "Controls.h"

void DetachBtnClick(IControl* p);

class CApplication;

class CMenu
{
public:
	static CMenu* Instance();

	void Init(CApplication* pApp);

	void CreateMenu();
	void ApplySettings();

	void HandleInput();
	void Draw(ISurface* pSurface);

	CWindow* GetMainWindow() { return m_pWindow; }
	CWindow* GetMirrorWindow() { return m_pWindowMirror; }
private:
	void CreateRageTab();
	void CreateLegitTab();
	void CreateVisualsTab();
	void CreateMiscTab();
	void CreateSkinChangerTab();
	void CreateConfigTab();

	CInputEvent m_inputEvent;
	CInputHandler m_inputHandler;

	// Main Windows & Controls
	CWindow* m_pWindow;
	CWindow* m_pWindowMirror;
	
	CTabContainer* m_pMainContainer;

	// Rage tab
	CTabPage* m_pRageTab;

	CGroupbox* m_pAimbotGroup;
	CCheckbox* m_pAimbotEnabled;
	CCheckbox* m_pAimbotSilentAim;
	CCheckbox* m_pAimbotAutoshoot;
	CCheckbox* m_pAimbotAutoscope;
	CCheckbox* m_pAimbotAutoReload;
	CCheckbox* m_pAimbotMultipoint;

	CCheckbox* m_pAimbotNoSpreadEnabled;

	CCheckbox* m_pAimbotHitchanceEnabled;
	CSlider* m_pAimbotHitchanceSlider;

	CSelectbox* m_pAimbotTargetCriteria;
	CSelectbox* m_pAimbotVisibleMode;

	CGroupbox* m_pHitboxGroup;
	CCheckbox* m_pHitboxHead;
	CCheckbox* m_pHitboxChest;
	CCheckbox* m_pHitboxPelvis;
	CCheckbox* m_pHitboxRForearm;
	CCheckbox* m_pHitboxLForearm;
	CCheckbox* m_pHitboxRCalf;
	CCheckbox* m_pHitboxLCalf;

	CGroupbox* m_pRageOthersGroup;
	CCheckbox* m_pRageOthersAutoZeusEnabled;
	CCheckbox* m_pRageOthersAutoRevolverEnabled;

	CGroupbox* m_pAntiaimGroup;
	CCheckbox* m_pAntiaimEnabled;
	CCheckbox* m_pAntiaimLbyIndicator;

	CGroupbox* m_pAntiaimStandingGroup;
	CSelectbox* m_pAntiaimStandingPitch;
	CSlider* m_pAntiaimStandingPitchOffset;
	CSelectbox* m_pAntiaimStandingYaw;
	CSlider* m_pAntiaimStandingYawOffset;
	CSelectbox* m_pAntiaimStandingYawFake;
	CSlider* m_pAntiaimYawStandingFakeOffset;
	CCheckbox* m_pAntiaimStandingLbyBreaker;

	CGroupbox* m_pAntiaimMovingGroup;
	CSelectbox* m_pAntiaimMovingPitch;
	CSlider* m_pAntiaimMovingPitchOffset;
	CSelectbox* m_pAntiaimMovingYaw;
	CSlider* m_pAntiaimMovingYawOffset;
	CSelectbox* m_pAntiaimMovingYawFake;
	CSlider* m_pAntiaimYawMovingFakeOffset;

	//
	// Legit tab
	CTabPage* m_pLegitTab;
	CGroupbox* m_pTriggerbotGroup;
	CCheckbox* m_pTriggerbotEnabled;
	CLabel* m_pTriggerbotDelayLabel;
	CSlider* m_pTriggerbotDelayValue;
	CLabel* m_pTriggerbotDelayJitterLabel;
	CSlider* m_pTriggerbotDelayJitterValue;
	//
	// Visuals tab
	CTabPage* m_pVisualsTab;
	CTabContainer* m_pVisualsTabContainer;

	CTabPage* m_pPlayerVisualsTab;
	
	// Esp
	CGroupbox* m_pEspGroup;
	CCheckbox* m_pEspEnabled;

	CSelectbox* m_pEspDrawBoundingBox;
	CCheckbox* m_pEspDrawFilledBox;
	CCheckbox* m_pEspDrawOutline;

	CCheckbox* m_pEspDrawHealthbar;
	CCheckbox* m_pEspDrawHealthnumber;
	CCheckbox* m_pEspDrawArmorbar;
	CCheckbox* m_pEspDrawArmornumber;

	CCheckbox* m_pEspDrawOwnTeam;
	CCheckbox* m_pEspDrawOwnModel;

	CCheckbox* m_pEspFadeoutEnabled;
	CLabel* m_pEspFadeoutLabel;
	CSlider* m_pEspFadeoutValue;

	CCheckbox* m_pEspDrawOnlyVisible;
	CCheckbox* m_pEspDrawOnlySpotted;

	CCheckbox* m_pEspDrawNames;

	CCheckbox* m_pEspBarrelEnabled;
	CLabel* m_pEspBarrelLabel;
	CSlider* m_pEspBarrelValue;
	
	// Chams
	CGroupbox* m_pChamsGroup;
	CCheckbox* m_pChamsEnabled;
	CSelectbox* m_pChamsStyle;
	CCheckbox* m_pChamsDrawOwnTeam;
	CCheckbox* m_pChamsDrawOwnModel;
	CCheckbox* m_pChamsIgnoreZ;

	// Sound Esp
	CGroupbox* m_pSoundEspGroup;
	CCheckbox* m_pSoundEspEnabled;

	CLabel* m_pSoundEspShowTimeLabel;
	CSlider* m_pSoundEspShowTime;

	CCheckbox* m_pSoundEspFadeoutEnabled;
	CLabel* m_pSoundEspFadeoutTimeLabel;
	CSlider* m_pSoundEspFadeoutTime;
	CCheckbox* m_pSoundEspDrawOwnTeam;
	CCheckbox* m_pSoundEspOnlyNotVisible;

	CTabPage* m_pOtherVisualsTab;

	CGroupbox* m_pFovChangerGroup;
	CCheckbox* m_pFovChangerFovEnabled;
	CCheckbox* m_pFovChangerFovScopeEnabled;
	CLabel* m_pFovChangerFovLabel;
	CSlider* m_pFovChangerFovValue;
	CCheckbox* m_pFovChangerViewmodelFovEnabled;
	CLabel* m_pFovChangerViewmodelFovLabel;
	CSlider* m_pFovChangerViewmodelFovValue;

	CGroupbox* m_pEffectsGroup;
	CCheckbox* m_pEffectsNoVisualRecoil;
	CCheckbox* m_pEffectsNoSmoke;
	CCheckbox* m_pEffectsNoScope;
	CCheckbox* m_pEffectsDisablePostprocessing;
	CCheckbox* m_pEffectsNoFlash;
	CLabel* m_pEffectsNoFlashLabel;
	CSlider* m_pEffectsNoFlashValue;

	CGroupbox* m_pVisualsOthersGroup;
	CSelectbox* m_pVisualsOthersHandsDrawStyle;
	CCheckbox* m_pVisualsOthersHitmarkerEnabled;
	CCheckbox* m_pVisualsOthersCrosshairEnabled;
	CCheckbox* m_pVisualsOthersRecoilCrosshairEnabled;
	CCheckbox* m_pVisualsOthersThirdperson;
	CLabel* m_pVisualsOthersThirdpersonLabel;
	CSlider* m_pVisualsOthersThirdpersonDistance;
	CCheckbox* m_pVisualsOthersMirror;

	//
	// Misc tab
	CTabPage* m_pMiscTab;
	CGroupbox* m_pFakelagGroup;
	CCheckbox* m_pFakelagEnabled;
	CLabel* m_pFakelagLabel;
	CSlider* m_pFakelagChokeAmount;

	CGroupbox* m_pMiscOthersGroup;
	CCheckbox* m_pMiscOthersNoRecoilEnabled;
	CCheckbox* m_pMiscOthersAutoPistolEnabled;
	CCheckbox* m_pMiscOthersJumpScoutEnabled;
	CCheckbox* m_pMiscOthersAutoAcceptEnabled;
	CCheckbox* m_pMiscOthersNoNameEnabled;

	CGroupbox* m_pMiscBhopGroup;
	CCheckbox* m_pMiscBhopEnabled;
	CGroupbox* m_pMiscBhopAutoStrafeGroup;
	CSelectbox* m_pMiscBhopAutoStrafeMode;

	//
	// SkinChanger tab
	CTabPage* m_pSkinChangerTab;
	//
	// Config tab
	CTabPage* m_pConfigTab;
	CButton* m_pDetachBtn;
	//

	CApplication* m_pApp;

	// Singleton
	CMenu();
	CMenu(CMenu const&);
	~CMenu();

	void operator=(CMenu const&) {}
};

#endif // __MENU_H__