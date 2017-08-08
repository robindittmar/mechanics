#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

// Std C lib
#include <cstring>
#include <vector>

// Custom
#include "IControl.h"

class CWindow
{
public:
	CWindow(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "Window");
	~CWindow();

	/// <summary>
	/// Adds child to window container,
	/// the window will take care of cleaning up the control
	/// </summary>
	bool AddChild(IControl* pControl);

	// TODO:
	void HandleEvents();
	void Draw(/*IDirect3DDevice9* pDevice*/);

	void IsVisible(bool bIsVisible) {
		m_bIsVisible = bIsVisible;
	}

	bool IsVisible() {
		return m_bIsVisible;
	}

	void Title(const char* pTitle) {
		if (m_pTitle)
			delete[] m_pTitle;

		int iLen = strlen(pTitle) + 1;
		m_pTitle = new char[iLen];
		memcpy(m_pTitle, pTitle, iLen);
	}

	const char* Title() {
		return (const char*)m_pTitle;
	}
private:
	bool m_bIsVisible;
	int m_iX, m_iY;
	int m_iWidth, m_iHeight;
	char* m_pTitle;
	std::vector<IControl*> m_pChildren;
};

#endif // __GUI_WINDOW_H__