#ifndef __GUI_H__
#define __GUI_H__

#include <Windows.h>
#include "ISurface.h"

class CGui
{
public:
	static CGui* Instance();

	void GetScreenSize();
	int ScreenWidth() { return m_iScreenWidth; }
	int ScreenHeight() { return m_iScreenHeight; }

	void EnableIngameMouse();
	void DisableIngameMouse();

	bool GetMousePos();
	int MouseX() { return m_iMouseX; }
	int MouseY() { return m_iMouseY; }

	void SetDrawMouse(bool bDrawMouse) { m_bDrawMouse = bDrawMouse; }
	bool GetDrawMouse() { return m_bDrawMouse; }

	void DrawMouse(ISurface* pSurface);

	bool IsMouseInRect(int x, int y, int w, int h);
private:
	int m_iScreenWidth, m_iScreenHeight;
	int m_iMouseX, m_iMouseY;
	bool m_bDrawMouse;

	// Singleton
	CGui();
	CGui(CGui const&);
	~CGui();

	void operator=(CGui const&) {}
};

#endif // __GUI_H__