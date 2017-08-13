#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std C lib
#include <cstring>
#include <vector>

// Custom
#include "IControl.h"

#define TITLEBAR_HEIGHT	30

class CWindow
{
public:
	// x, y, w, h define the client area of the window
	CWindow(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "Window");
	~CWindow();

	/// <summary>
	/// Adds child to window container,
	/// the window will take care of cleaning up the control
	/// </summary>
	bool AddChild(IControl* pControl);

	// TODO:
	void HandleEvents();
	void Draw(ISurface* pSurface);

	void IsVisible(bool bIsVisible) {
		m_bIsVisible = bIsVisible;
	}

	bool IsVisible() {
		return m_bIsVisible;
	}

	void Title(const char* pTitle);
	const char* Title() {
		return (const char*)m_pTitle;
	}
private:
	bool m_bIsVisible;
	int m_iX, m_iY;
	int m_iWidth, m_iHeight;

	int m_iLenTitle;
	char* m_pTitle;
	wchar_t* m_pTitleW;
	std::vector<IControl*> m_pChildren;
};

#endif // __GUI_WINDOW_H__