#ifndef __BUTTON_H__
#define __BUTTON_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"

class CButton : public IControl
{
public:
	CButton(int x = 0, int y = 0, int w = 100, int h = 20, const char* pText = "btn");
	~CButton();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void SetButtonDownEventHandler(SimpleEventHandler_t onDown) { m_pButtonDownHandler = onDown; }
	void SetButtonUpEventHandler(SimpleEventHandler_t onUp) { m_pButtonUpHandler = onUp; }

	void ContentText(const char* pText);
	const char* ContentText() { return (const char*)m_pContentText; }
private:
	bool m_bIsPressed;
	
	SimpleEventHandler_t m_pButtonDownHandler;
	SimpleEventHandler_t m_pButtonUpHandler;

	int m_iContentTextLen;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
};

#endif // __BUTTON_H__