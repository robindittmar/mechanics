#ifndef __TABCONTAINER_H__
#define __TABCONTAINER_H__

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"
#include "TabPage.h"

const Color g_clrTab(255, 80, 80, 80);
const Color g_clrTabOver(255, 100, 100, 100);
const Color g_clrTabMouseDown(255, 110, 110, 110);
const Color g_clrTabSelected(255, 120, 120, 120);

//const Color g_clrTabText(255, 255, 255, 255);

class CTabContainer : public IControl
{
public:
	CTabContainer();
	~CTabContainer();
	
	void SelectTab(int idx);

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseDown(int mx, int my);
	virtual void OnMouseUp(int mx, int my);

	virtual void AddChild(IControl* pControl);

	virtual void Draw(ISurface* pSurface);
private:
	std::vector<CLabel*> m_vLabels;

	int m_iCountTabs;
	int m_iTabMouseDown;
	int m_iMouseOverTab, m_iSelectedTab;
};

#endif // __TABCONTAINER_H__