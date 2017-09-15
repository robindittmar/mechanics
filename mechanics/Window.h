#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std lib
#include <cstring>
#include <vector>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Canvas.h"

#define TITLEBAR_HEIGHT	30

const Color g_clrTitlebar(255, 150, 150, 150);
const Color g_clrClientRegion(150, 230, 230, 230);

class CWindow : public IControl
{
public:
	// x, y, w, h define the client area of the window
	CWindow(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "wnd");
	~CWindow();

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseDown(int mx, int my);
	virtual void OnMouseUp(int mx, int my);

	virtual void AddChild(IControl* pControl);

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void SetClientPosition(int x, int y);
	void GetClientPosition(int* x, int* y);

	void SetVisible(bool bIsVisible) { m_bIsVisible = bIsVisible; }
	bool GetVisible() { return m_bIsVisible; }

	void SetPopup(IControl* pPopup) { m_pPopup = pPopup; }
	IControl* GetPopup() { return m_pPopup; }
private:
	CCanvas* m_pCanvas;
	CLabel* m_pLabelTitle;

	IControl* m_pPopup;

	bool m_bIsVisible;

	bool m_bIsDragging;
	int m_iDragOffsetX, m_iDragOffsetY;
};

#endif // __GUI_WINDOW_H__