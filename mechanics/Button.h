#ifndef __BUTTON_H__
#define __BUTTON_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

const Color g_clrButton(255, 80, 80, 80);
const Color g_clrButtonOver(255, 70, 70, 70);
const Color g_clrButtonDown(255, 255, 128, 0);

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