#ifndef __MENU_H__
#define __MENU_H__

// Std Lib
#include <windows.h>

// Custom
#include "InputEvent.h"
#include "InputHandler.h"
#include "Controls.h"

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

	void SetMainColor(Color clrMainColor) { m_clrMainColor = clrMainColor; }
	Color GetMainColor() { return m_clrMainColor; }

	CWindow* GetMainWindow() { return m_pWindow; }
	CWindow* GetMirrorWindow() { return m_pWindowMirror; }

	// TODO
	void AddKnifeToWeapons(int iWeaponId);
	void FillSkinIds(int iWeaponId);
	void ApplySkin(int iTeamNum);
	void ApplySkinsBothTeams();

	/// <summary>
	/// Fills given selectbox with all files (Directories are excluded) from the given directory
	/// </summary>
	/// <param name="pSelectbox">Selectbox to fill</param>
	/// <param name="pPath">Relative path of folder to list (please include trailing slash!) e.g. "cfg\\", "cfg\\skins\\"</param>
	void FillSelectboxWithFiles(CSelectbox* pSelectbox, const char* pPath);
	void LoadConfig();
	void SaveConfig();

	// <DELETE ME PLS>
	void PrintNumUpDownValue();
	// </DELETE ME PLS>
private:
	void CreateRageTab();
	void CreateLegitTab();
	void CreateVisualsTab();
	void CreateMiscTab();
	void CreateSkinChangerTab();
	void CreateConfigTab();

	CInputEvent m_inputEvent;
	CInputHandler* m_pInputHandler;

	Color m_clrMainColor;

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
	CSlider* m_pAimbotMultipointScale;

	CCheckbox* m_pRageNoRecoilEnabled;
	CCheckbox* m_pRageNoSpreadEnabled;

	CCheckbox* m_pAimbotHitchanceEnabled;
	CSlider* m_pAimbotHitchanceSlider;

	CSelectbox* m_pAimbotTargetCriteria;
	CSelectbox* m_pAimbotVisibleMode;

	CGroupbox* m_pRageHitboxGroup;
	CCheckbox* m_pRageHitboxHead;
	CCheckbox* m_pRageHitboxChest;
	CCheckbox* m_pRageHitboxPelvis;
	CCheckbox* m_pRageHitboxRForearm;
	CCheckbox* m_pRageHitboxLForearm;
	CCheckbox* m_pRageHitboxRCalf;
	CCheckbox* m_pRageHitboxLCalf;

	CGroupbox* m_pRageOthersGroup;
	CCheckbox* m_pRageOthersAutoZeusEnabled;
	CCheckbox* m_pRageOthersAutoRevolverEnabled;
	CCheckbox* m_pRageOthersLagCompensationEnabled;
	CSelectbox* m_pRageOthersResolverType;

	CGroupbox* m_pAntiaimGroup;
	CCheckbox* m_pAntiaimEnabled;
	CCheckbox* m_pAntiaimLbyIndicator;

	CGroupbox* m_pAntiaimStandingGroup;
	CSelectbox* m_pAntiaimStandingPitch;
	CSelectbox* m_pAntiaimStandingYaw;
	CSlider* m_pAntiaimStandingYawOffset;
	CSelectbox* m_pAntiaimStandingYawFake;
	CSlider* m_pAntiaimYawStandingFakeOffset;
	CCheckbox* m_pAntiaimStandingLbyBreaker;

	CGroupbox* m_pAntiaimMovingGroup;
	CSelectbox* m_pAntiaimMovingPitch;
	CSelectbox* m_pAntiaimMovingYaw;
	CSlider* m_pAntiaimMovingYawOffset;
	CSelectbox* m_pAntiaimMovingYawFake;
	CSlider* m_pAntiaimYawMovingFakeOffset;

	CGroupbox* m_pAntiaimEdgeAntiaimGroup;
	CCheckbox* m_pAntiaimEdgeAntiaimEnabled;
	CLabel* m_pAntiaimEdgeAntiaimCheckOffsetLabel;
	CSlider* m_pAntiaimEdgeAntiaimCheckOffset;
	CCheckbox* m_pAntiaimEdgeAntiaimDrawPoints;
	CCheckbox* m_pAntiaimEdgeAntiaimDrawLines;

	//
	// Legit tab
	CTabPage* m_pLegitTab;

	CGroupbox* m_pLegitbotGroup;
	CCheckbox* m_pLegitbotEnabled;
	CCheckbox* m_pLegitbotAutoshoot;
	CSlider* m_pLegitbotHelpAfterShots;
	CSlider* m_pLegitbotTimeToAim;
	CSlider* m_pLegitbotCurve;
	CSlider* m_pLegitbotFov;
	CSlider* m_pLegitbotPointScale;
	CCheckbox* m_pLegitbotDrawPath;
	CColorPicker* m_pLegitbotPathColor;

	CGroupbox* m_pLegitHitboxGroup;
	CCheckbox* m_pLegitHitboxHead;
	CCheckbox* m_pLegitHitboxChest;
	CCheckbox* m_pLegitHitboxPelvis;
	CCheckbox* m_pLegitHitboxRForearm;
	CCheckbox* m_pLegitHitboxLForearm;
	CCheckbox* m_pLegitHitboxRCalf;
	CCheckbox* m_pLegitHitboxLCalf;

	CGroupbox* m_pTriggerbotGroup;
	CCheckbox* m_pTriggerbotEnabled;
	CKeyBinder* m_pTriggerbotKey;
	CSlider* m_pTriggerbotDelayValue;
	CSlider* m_pTriggerbotDelayJitterValue;
	CCheckbox* m_pTriggerbotBurst;
	CSlider* m_pTriggerbotMinShots; // TODO: CRangeSlider, instead of 2 sliders
	CSlider* m_pTriggerbotMaxShots;

	CGroupbox* m_pLegitLagCompensationGroup;
	CCheckbox* m_pLegitLagCompensationEnabled;
	CSlider* m_pLegitLagCompensationDuration;

	//
	// Visuals tab
	CTabPage* m_pVisualsTab;
	CTabContainer* m_pVisualsTabContainer;

	CTabPage* m_pVisualsOneTab;
	CTabPage* m_pVisualsTwoTab;
	CTabPage* m_pVisualsThreeTab;
	
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
	CCheckbox* m_pEspDrawAmmobar;
	CCheckbox* m_pEspDrawAmmonumber;

	CCheckbox* m_pEspDrawGlowEnabled;
	CCheckbox* m_pEspDrawSkeletonEnabled;

	CCheckbox* m_pEspDrawOwnTeam;
	CCheckbox* m_pEspDrawOwnModel;

	CCheckbox* m_pEspFadeoutEnabled;
	CLabel* m_pEspFadeoutLabel;
	CSlider* m_pEspFadeoutValue;

	CCheckbox* m_pEspDrawOnlyVisible;
	CCheckbox* m_pEspDrawOnlySpotted;
	CCheckbox* m_pEspDrawActiveWeapon;

	CCheckbox* m_pEspDrawNames;

	CCheckbox* m_pEspBarrelEnabled;
	CLabel* m_pEspBarrelLabel;
	CSlider* m_pEspBarrelValue;

	// WeaponEsp
	CGroupbox* m_pWeaponEspGroup;
	CLabel* m_pWeaponEspWeaponLabel;
	CCheckbox* m_pWeaponEspEnabled;
	CCheckbox* m_pWeaponEspWeaponBoundingBoxEnabled;
	CCheckbox* m_pWeaponEspWeaponNameEnabled;
	CLabel* m_pWeaponEspGrenadeLabel;
	CCheckbox* m_pWeaponEspGrenadeNameEnabled;
	CCheckbox* m_pWeaponEspGrenadeBoundingBoxEnabled;
	CLabel* m_pWeaponEspBombLabel;
	CCheckbox* m_pWeaponEspBombNameEnabled;
	CCheckbox* m_pWeaponEspBombBoundingBoxEnabled;
	CCheckbox* m_pWeaponEspBombTimerEnabled;
	CCheckbox* m_pWeaponEspBombDefuseTimerEnabled;
	CCheckbox* m_pWeaponEspBombDamageIndicatorEnabled;

	// Chams
	CGroupbox* m_pChamsGroup;
	CCheckbox* m_pChamsEnabled;
	CSelectbox* m_pChamsStyle;
	CCheckbox* m_pChamsDrawOwnTeam;
	CCheckbox* m_pChamsDrawOwnModel;
	CCheckbox* m_pChamsIgnoreZ;
	CCheckbox* m_pChamsFakeAngle;
	CSelectbox* m_pChamsWeaponChams;

	CColorPicker* m_pChamsColorVisibleCT;
	CColorPicker* m_pChamsColorHiddenCT;
	CColorPicker* m_pChamsColorVisibleT;
	CColorPicker* m_pChamsColorHiddenT;

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

	// LC Drawing
	CGroupbox* m_pDrawLagCompensationGroup;
	CSelectbox* m_pDrawLagCompensationStyle;
	CLabel* m_pDrawLagCompensationFrequencyLabel;
	CSlider* m_pDrawLagCompensationFrequency;
	CCheckbox* m_pDrawLagCompensationOnlyVisible;

	// Bullet Tracer
	CGroupbox* m_pBulletTracerGroup;
	CCheckbox* m_pBulletTracerEnabled;
	CCheckbox* m_pBulletTracerSelf;
	CCheckbox* m_pBulletTracerTeam;

	// FOV Changer
	CGroupbox* m_pFovChangerGroup;
	CCheckbox* m_pFovChangerFovEnabled;
	CCheckbox* m_pFovChangerFovScopeEnabled;
	CLabel* m_pFovChangerFovLabel;
	CSlider* m_pFovChangerFovValue;
	CCheckbox* m_pFovChangerViewmodelFovEnabled;
	CLabel* m_pFovChangerViewmodelFovLabel;
	CSlider* m_pFovChangerViewmodelFovValue;

	// Effects
	CGroupbox* m_pEffectsGroup;
	CCheckbox* m_pEffectsNoVisualRecoil;
	CCheckbox* m_pEffectsNoSmoke;
	CCheckbox* m_pEffectsNoScope;
	CCheckbox* m_pEffectsDisablePostprocessing;
	CCheckbox* m_pEffectsNoFlash;
	CLabel* m_pEffectsNoFlashLabel;
	CSlider* m_pEffectsNoFlashValue;

	// Others
	CGroupbox* m_pVisualsOthersGroup;
	CSelectbox* m_pVisualsOthersHandsDrawStyle;
	CCheckbox* m_pVisualsOthersHitmarkerEnabled;
	CCheckbox* m_pVisualsOthersHitmarkerSoundEnabled;
	CCheckbox* m_pVisualsOthersHitmarkerHitpointEnabled;
	CCheckbox* m_pVisualsOthersCrosshairEnabled;
	CCheckbox* m_pVisualsOthersRecoilCrosshairEnabled;
	CCheckbox* m_pVisualsOthersSpreadConeEnabled;
	CCheckbox* m_pVisualsOthersRecoilSpreadConeEnabled;
	CSelectbox* m_pVisualsOtherSpreadConeStyle;
	CColorPicker* m_pVisualsOtherSpreadConeColor;

	CSlider* m_pVisualsOthersNightmode;
	CSlider* m_pVisualsOthersAsuswalls;
	CSelectbox* m_pVisualsOthersSkychanger;

	CCheckbox* m_pVisualsOthersThirdperson;
	CLabel* m_pVisualsOthersThirdpersonLabel;
	CSlider* m_pVisualsOthersThirdpersonDistance;
	CCheckbox* m_pVisualsOthersMirror;

	//
	// Misc tab
	CTabPage* m_pMiscTab;
	CGroupbox* m_pFakelagGroup;
	CCheckbox* m_pFakelagEnabled;
	CCheckbox* m_pFakelagOnlyInAir;
	CLabel* m_pFakelagLabel;
	CSlider* m_pFakelagChokeAmount;
	CSelectbox* m_pFakelagType;

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
	CGroupbox* m_pMiscBhopCircleStrafeGroup;
	CCheckbox* m_pMiscBhopCircleStrafeEnabled;
	CSelectbox* m_pMiscBhopCircleStrafeDirection;
	CLabel* m_pMiscBhopCircleStrafeLabel;

	//
	// SkinChanger tab
	CTabPage* m_pSkinChangerTab;
	CGroupbox* m_pSkinChangerSkinsGroup;
	CSelectbox* m_pSkinChangerWeapon;
	CSelectbox* m_pSkinChangerSkin;
	CButton* m_pSkinChangerApplyBoth;
	CButton* m_pSkinChangerApplyCt;
	CButton* m_pSkinChangerApplyT;

	CTextbox* m_pSkinChangerWeaponName;
	CNumericUpDown* m_pSkinChangerWeaponStattrakCount;
	CNumericUpDown* m_pSkinChangerWeaponSeed;
	CSlider* m_pSkinChangerWeaponWear;

	CCheckbox* m_pSkinChangerOnlyMyWeapon;

	CSelectbox* m_pSkinChangerKnife;
	CSelectbox* m_pSkinChangerGlove;

	CButton* m_pSkinChangerLoadCfg;
	CButton* m_pSkinChangerSaveCfg;

	//
	// Config tab
	CTabPage* m_pConfigTab;
	CButton* m_pDetachBtn;
	CButton* m_pForceFullUpdate;
	CColorPicker* m_pColorPicker;

	CGroupbox* m_pConfigGroup;
	CSelectbox* m_pConfigLoadables;
	CButton* m_pConfigLoadConfigButton;
	CButton* m_pConfigReloadLoadables;
	CTextbox* m_pConfigSaveConfigName;
	CButton* m_pConfigSaveConfigButton;

	CNumericUpDown* m_pNumericUpDown;
	CButton* m_pConfigPrintValue;
	//

	CApplication* m_pApp;

	// Singleton
	CMenu();
	CMenu(CMenu const&);
	~CMenu();

	void operator=(CMenu const&) {}
};

#endif // __MENU_H__