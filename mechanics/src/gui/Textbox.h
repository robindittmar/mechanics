#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "InputHandler.h"
#include "IControlClickable.h"
#include "Label.h"

#define TEXTBOX_PADDING	4

const Color g_clrTextboxText(255, 255, 255, 255);
const Color g_clrTextboxBackground(255, 50, 50, 50);
const Color g_clrTextboxBackgroundOutline(255, 80, 80, 80);
const Color g_clrTextboxBackgroundOutlineHover(255, 255, 128, 0);

class CTextbox : public IControlClickable
{
public:
	CTextbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "txtbx", int maxLen = 256);
	virtual ~CTextbox();

	virtual void OnTextInput(char c);

	void SetText(const char* pText);
	const char* GetText() { return m_pContentLabel->GetContentText(); }

	virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	void SetEventHandler(std::function<void(const char*)> pEventHandler) { m_pEventHandler = pEventHandler; }
protected:
	CLabel* m_pLabel;
	CLabel* m_pContentLabel;

	int m_iMaxLen;

	float m_fLastBlink;
	bool m_bBlink;

	std::function<void(const char*)> m_pEventHandler;
};

#endif // __TEXTBOX_H__