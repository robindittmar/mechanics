#ifndef __GUI_H__
#define __GUI_H__

#include <Windows.h>
#include "ISurface.h"
#include "Vector.h"
#include "ConVar.h"

class CApplication;

class CGui
{
public:
	static CGui* Instance();

	void Setup();
	void GetWorldToScreenMatrix();

	int GetScreenWidth() { return m_iScreenWidth; }
	int GetScreenHeight() { return m_iScreenHeight; }

	void SetEnableMouse(bool bEnableMouse);

	bool GetMousePos();
	int MouseX() { return m_iMouseX; }
	int MouseY() { return m_iMouseY; }

	void SetDrawMouse(bool bDrawMouse) { m_bDrawMouse = bDrawMouse; }
	bool GetDrawMouse() { return m_bDrawMouse; }

	void DrawMouse(ISurface* pSurface);

	bool IsMouseInRect(int x, int y, int w, int h);

	bool WorldToScreen(const Vector &origin, Vector &screen);

	bool IsMouseEnabled();
private:
	bool ScreenTransform(const Vector& point, Vector& screen);

	const VMatrix* m_pWorldToScreenMatrix;
	int m_iScreenWidth, m_iScreenHeight;

	int m_iMouseX, m_iMouseY;
	bool m_bDrawMouse;
	ConVar* m_pMouseEnable;

	CApplication* m_pApp;

	// Singleton
	CGui();
	CGui(CGui const&);
	~CGui();

	void operator=(CGui const&) {}
};

#endif // __GUI_H__