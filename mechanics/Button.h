#ifndef __BUTTON_H__
#define __BUTTON_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

const Color g_clrButton(255, 0, 0, 0);
const Color g_clrButtonOver(255, 50, 50, 50);
const Color g_clrButtonDown(255, 100, 100, 100);

class CButton : public IControl
{
public:
	CButton(int x = 0, int y = 0, int w = 100, int h = 20, const char* pText = "btn");
	~CButton();

	virtual void OnClicked();
	virtual void Draw(ISurface* pSurface);

	void SetButtonClickEventHandler(SimpleEventHandler_t onClicked) { m_pButtonClicked = onClicked; }
private:
	CLabel* m_pLabel;
	SimpleEventHandler_t m_pButtonClicked;
};

#endif // __BUTTON_H__