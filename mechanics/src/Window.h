#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std lib
#include <cstring>
#include <vector>

// Custom
#include "Gui.h"
#include "IControlClickable.h"
#include "Tooltip.h"
#include "Canvas.h"

#define TITLEBAR_HEIGHT	30

const Color g_clrTitlebar(255, 30, 30, 30);
const Color g_clrClientRegion(250, 60, 60, 60);

class CWindow : public IControlClickable
{
public:
	// x, y, w, h define the client area of the window
	CWindow(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "wnd");
	virtual ~CWindow();

	virtual void OnMouseMove(int mx, int my) override;
	virtual void OnMouseDown(int mx, int my) override;
	virtual void OnMouseUp(int mx, int my) override;

	virtual void AddChild(IControl* pControl) override;

	virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	void SetClientPosition(int x, int y);
	void GetClientPosition(int* x, int* y);
	void GetClientSize(int* w, int* h);

	void SetFocus(IControl* pFocus) { m_pFocus = pFocus; }
	IControl* GetFocus() { return m_pFocus; }

	void SetPopup(IControl* pPopup) { m_pPopup = pPopup; }
	IControl* GetPopup() { return m_pPopup; }

	void SetTooltip(CTooltip* pTooltip) { m_pTooltip = pTooltip; }
	CTooltip* GetTooltip() { return m_pTooltip; }
private:
	CCanvas* m_pCanvas;
	CLabel* m_pLabelTitle;

	IControl* m_pFocus;
	IControl* m_pPopup;
	CTooltip* m_pTooltip;

	bool m_bIsDragging;
	int m_iDragOffsetX, m_iDragOffsetY;
};

#endif // __GUI_WINDOW_H__