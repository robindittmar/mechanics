#ifndef __GUI_H__
#define __GUI_H__

// Std Lib
#include <Windows.h>
#include <Windowsx.h>

// Source SDK
#include "Vector.h"
#include "ISurface.h"
#include "ConVar.h"

class CApplication;

class CGui
{
public:
	static CGui* Instance();

	void Setup();
	void Cleanup();

	void GetWorldToScreenMatrix();

	int GetScreenWidth() const { return m_iScreenWidth; }
	int GetScreenHeight() const { return m_iScreenHeight; }

	void SetEnableGameInput(bool bEnableGameInput);
	bool GetEnableGameInput() { return m_bEnableGameInput; }

	bool IsMouseInRect(int x, int y, int w, int h);

	bool IsMouseEnabled();
	void SetMousePos(int iX, int iY);
	int MouseX() { return m_iMouseX; }
	int MouseY() { return m_iMouseY; }

	void SetDrawMouse(bool bDrawMouse) { m_bDrawMouse = bDrawMouse; }
	bool GetDrawMouse() { return m_bDrawMouse; }

	void DrawMouse(ISurface* pSurface);

	bool WorldToScreen(const Vector &origin, Vector &screen);
private:
	bool ScreenTransform(const Vector& point, Vector& screen);
	
	unsigned short XButtonToVKey(WPARAM wParam);
	friend LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT nCode, WPARAM wParam, LPARAM lParam);

	const VMatrix* m_pWorldToScreenMatrix;
	int m_iScreenWidth, m_iScreenHeight;

	bool m_bHookedWindowProc;
	HWND m_hGameWindow;
	WNDPROC m_wndProc;
	bool m_bEnableGameInput;

	int m_iMouseX, m_iMouseY;
	bool m_bDrawMouse;
	ConVar* m_pMouseEnable;

	CApplication* m_pApp;

	// Singleton
	CGui();
	CGui(const CGui&) = delete;
	~CGui();
};

#endif // __GUI_H__