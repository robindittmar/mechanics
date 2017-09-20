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
	CCheckbox* m_pAimbotEnabled;
	CCheckbox* m_pAimbotSilentAim;
	CCheckbox* m_pAimbotAutoshoot;
	CCheckbox* m_pAimbotAutoscope;
	
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
	CSelectbox* m_pAntiaimPitch;
	CSelectbox* m_pAntiaimYaw;
	//
	// Legit tab
	CTabPage* m_pLegitTab;
	CLabel* m_pLabelWip;
	//
	// Visuals tab
	CTabPage* m_pVisualsTab;
	CTabContainer* m_pVisualsTabContainer;

	CTabPage* m_pPlayerVisualsTab;
	CGroupbox* m_pEspGroup;
	CCheckbox* m_pEspEnabled; // TODO
	CCheckbox* m_pEspDrawBoundingBox;
	CCheckbox* m_pEspDrawHealthbar;
	CCheckbox* m_pEspDrawHealthnumber;
	CCheckbox* m_pEspDrawArmorbar;
	CCheckbox* m_pEspDrawOwnTeam;
	CCheckbox* m_pEspDrawOwnModel;
	CCheckbox* m_pEspDrawOnlySpotted;
	CCheckbox* m_pEspDrawOutline;
	CCheckbox* m_pEspDrawNames;
	
	CGroupbox* m_pChamsGroup;
	CCheckbox* m_pChamsEnabled; // TODO
	CCheckbox* m_pChamsDrawOwnTeam;
	CCheckbox* m_pChamsDrawOwnModel;
	CCheckbox* m_pChamsIgnoreZ;
	CCheckbox* m_pChamsFlatModels;

	CTabPage* m_pOtherVisualsTab;
	CCheckbox* m_pMirror;
	CCheckbox* m_pThirdperson;
	CSelectbox* m_pHandsDrawStyle;
	CSlider* m_pFlashAmount;
	CSlider* m_pFov;
	CSlider* m_pViewmodelFov;
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