#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std lib
#include <cstring>
#include <vector>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Tooltip.h"
#include "Canvas.h"

#define TITLEBAR_HEIGHT	30

const Color g_clrTitlebar(255, 30, 30, 30);
const Color g_clrClientRegion(250, 60, 60, 60);

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

	void SetPopup(IControl* pPopup) { m_pPopup = pPopup; }
	IControl* GetPopup() { return m_pPopup; }

	void SetTooltip(CTooltip* pTooltip) { m_pTooltip = pTooltip; }
	CTooltip* GetTooltip() { return m_pTooltip; }
private:
	CCanvas* m_pCanvas;
	CLabel* m_pLabelTitle;

	IControl* m_pPopup;
	CTooltip* m_pTooltip;

	bool m_bIsDragging;
	int m_iDragOffsetX, m_iDragOffsetY;
};

#endif // __GUI_WINDOW_H__