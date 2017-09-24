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

	CGroupbox* m_pAntiaimGroup;
	CCheckbox* m_pAntiaimEnabled;
	CSelectbox* m_pAntiaimPitch;
	CSlider* m_pAntiaimPitchOffset;
	CSelectbox* m_pAntiaimYaw;
	CSlider* m_pAntiaimYawOffset;
	CSelectbox* m_pAntiaimYawFake;
	CSlider* m_pAntiaimYawFakeOffset;
	CCheckbox* m_pAntiaimLbyIndicator;
	CCheckbox* m_pAntiaimLbyBreaker;
	//
	// Legit tab
	CTabPage* m_pLegitTab;
	CLabel* m_pLabelWip;
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
	CCheckbox* m_pEffectsNoFlash;
	CLabel* m_pEffectsNoFlashLabel;
	CSlider* m_pEffectsNoFlashValue;
	CCheckbox* m_pEffectsDisablePostprocessing;


	CGroupbox* m_pOthersGroup;
	CSelectbox* m_pOthersHandsDrawStyle;
	CCheckbox* m_pOthersHitmarkerEnabled;
	CCheckbox* m_pOthersCrosshairEnabled;
	CCheckbox* m_pOthersRecoilCrosshairEnabled;
	CCheckbox* m_pOthersThirdperson;
	CLabel* m_pOthersThirdpersonLabel;
	CSlider* m_pOthersThirdpersonDistance;
	CCheckbox* m_pOthersMirror;


	CSlider* m_pFlashAmount;

	//
	// Misc tab
	CTabPage* m_pMiscTab;
	CCheckbox* m_pNoName;
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