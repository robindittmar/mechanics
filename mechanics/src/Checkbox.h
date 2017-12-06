#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControlTooltip.h"
#include "Label.h"

#define CHECKBOX_BOXPADDING		5
#define CHECKBOX_BOXSIZE		10
#define CHECKBOX_FILLERPADDING	2

const Color g_clrCheckboxBorder(255, 0, 0, 0);
const Color g_clrCheckboxBorderHover(255, 25, 25, 25);
const Color g_clrCheckboxFiller(255, 255, 128, 0);
const Color g_clrCheckboxFillerHover(255, 255, 100, 0);

const Color g_clrCheckboxText(255, 255, 255, 255);
const Color g_clrCheckboxTextHover(255, 200, 200, 200);

class CCheckbox : public IControlTooltip
{
public:
	CCheckbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* pText = "cbx");
	virtual ~CCheckbox();

	virtual void OnClicked();
	virtual void Draw(ISurface* pSurface);

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	void SetChecked(bool bIsChecked);
	bool GetChecked() { return m_bIsChecked; }

	void SetEventHandler(std::function<void(bool)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	CLabel* m_pLabel;

	bool m_bIsChecked;
	std::function<void(bool)> m_pEventHandler;
};

#endif // __CHECKBOX_H__