#ifndef __TABCONTAINER_H__
#define __TABCONTAINER_H__

// Custom
#include "Gui.h"
#include "IControlClickable.h"
#include "Label.h"
#include "TabPage.h"

const Color g_clrTab(255, 45, 45, 45);
const Color g_clrTabOver(255, 40, 40, 40);
const Color g_clrTabMouseDown(255, 35, 35, 35);
const Color g_clrTabSelected(255, 255, 128, 0);
const Color g_clrSplitline(255, 30, 30, 30);

//const Color g_clrTabText(255, 255, 255, 255);

class CTabContainer : public IControlClickable
{
public:
	CTabContainer();
	~CTabContainer();
	
	void SelectTab(int idx);

	virtual void OnMouseMove(int mx, int my) override;
	virtual void OnMouseDown(int mx, int my) override;
	virtual void OnMouseUp(int mx, int my) override;

	virtual void AddChild(IControl* pControl) override;

	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;
private:
	std::vector<CLabel*> m_vLabels;

	int m_iCountTabs;
	int m_iTabMouseDown;
	int m_iMouseOverTab, m_iSelectedTab;
};

#endif // __TABCONTAINER_H__