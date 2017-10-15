#ifndef __SELECTBOXPOPUP_H__
#define __SELECTBOXPOPUP_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

class CWindow;
class CSelectbox;

const Color g_clrSelectboxPopupSelection(255, 255, 128, 0);

class CSelectboxPopup : public IControl
{
public:
	CSelectboxPopup(int x = 0, int y = 0, CSelectbox* pSelectbox = NULL, CWindow* pParentWindow = NULL);
	~CSelectboxPopup();

	void SetSelectbox(CSelectbox* pSelectbox) { m_pSelectbox = pSelectbox; }
	void SetParentWindow(CWindow* pParentWindow) { m_pParentWindow = pParentWindow; }

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseUp(int mx, int my);

	virtual void GetAbsolutePosition(int* pX, int* pY);

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void ClearOptions();
	void AddOption(int curIdx, const char* text, int len);
private:
	CSelectbox* m_pSelectbox;
	CWindow* m_pParentWindow;

	int m_iLenLargestOptionString;
	int m_iLargestOptionStringIndex;

	int m_iSelectedOption;

	int m_iRowRenderWidth;
	int m_iRowRenderHeight;

	std::vector<CLabel*> m_vOptionLabels;
};

#endif // __SELECTBOXPOPUP_H__