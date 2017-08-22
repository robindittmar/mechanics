#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std lib
#include <cstring>
#include <vector>

// Custom
#include "Gui.h"
#include "IControl.h"

#define TITLEBAR_HEIGHT	30

class CWindow : public IControl
{
public:
	// x, y, w, h define the client area of the window
	CWindow(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "Window");
	~CWindow();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void IsVisible(bool bIsVisible) { m_bIsVisible = bIsVisible; }
	bool IsVisible() { return m_bIsVisible; }

	void Title(const char* pTitle);
	const char* Title() { return (const char*)m_pTitle; }
private:
	bool m_bIsVisible;

	bool m_bIsDragging;
	int m_iDragOffsetX, m_iDragOffsetY;

	unsigned int m_iFont;

	int m_iLenTitle;
	char* m_pTitle;
	wchar_t* m_pTitleW;
};

#endif // __GUI_WINDOW_H__