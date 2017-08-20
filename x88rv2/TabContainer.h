#ifndef __TABCONTAINER_H__
#define __TABCONTAINER_H__

// Std lib


// Custom
#include "Gui.h"
#include "IControl.h"
#include "TabPage.h"

#define TABCONTAINER_TABHEIGHT	40

class CTabContainer : public IControl
{
public:
	CTabContainer();
	~CTabContainer();
	
	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void SelectTab(CTabPage* p);
private:
};

#endif // __TABCONTAINER_H__