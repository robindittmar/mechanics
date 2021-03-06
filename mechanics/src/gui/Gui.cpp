#include "Gui.h"
#include "../Application.h"

CGui::CGui()
	: m_pWorldToScreenMatrix(NULL), m_bDrawMouse(false), m_bEnableGameInput(true)
{
}

CGui::~CGui()
{
}

CGui* CGui::Instance()
{
	static CGui inst;
	return &inst;
}

void CGui::Setup()
{
	m_pApp = CApplication::Instance();

	this->GetScreenSize();
	this->GetWorldToScreenMatrix();

	m_pMouseEnable = m_pApp->CVar()->FindVar(/*cl_mouseenable*/CXorString("\x74\x67\xDA\xAF\x78\x7E\xF6\xA7\x72\x65\xE4\xA0\x7B\x6E").ToCharArray());

	// Game Window
	m_hGameWindow = FindWindow(/*Valve001*/CXorString("\x41\x6A\xE9\xB4\x72\x3B\xB5\xF3").ToCharArray(), nullptr);
	while (!m_hGameWindow)
	{
		Sleep(1000);
		m_hGameWindow = FindWindow(/*Valve001*/CXorString("\x41\x6A\xE9\xB4\x72\x3B\xB5\xF3").ToCharArray(), nullptr);
	}

	if (!m_bHookedWindowProc)
	{
		// Install window procedure hook
		m_wndProc = (WNDPROC)SetWindowLongPtr(m_hGameWindow, GWL_WNDPROC, (LONG)(LONG_PTR)hk_WndProc);
		m_bHookedWindowProc = true;
	}
}

void CGui::Cleanup()
{
	// Unhook window procedure
	if (m_hGameWindow && m_wndProc && m_bHookedWindowProc)
	{
		SetWindowLongPtr(m_hGameWindow, GWL_WNDPROC, (LONG_PTR)m_wndProc);
		m_bHookedWindowProc = false;
	}
}

void CGui::GetScreenSize()
{
	m_pApp->EngineClient()->GetScreenSize(m_iScreenWidth, m_iScreenHeight);
}

void CGui::GetWorldToScreenMatrix()
{
	// Pointer to WorldToScreenMatrix
	m_pWorldToScreenMatrix = &m_pApp->EngineClient()->WorldToScreenMatrix();
}

void CGui::SetEnableGameInput(bool bEnableGameInput)
{
	m_bEnableGameInput = bEnableGameInput;
}

bool CGui::IsMouseInRect(int x, int y, int w, int h)
{
    return m_iMouseX >= x && m_iMouseX <= (x + w) &&
           m_iMouseY >= y && m_iMouseY <= (y + h);
}

bool CGui::IsMouseEnabled()
{
	return (m_pMouseEnable->value[0] != '0');
}

void CGui::SetMousePos(int iX, int iY)
{
	m_iMouseX = iX;
	m_iMouseY = iY;
}

void CGui::DrawMouse(ISurface* pSurface)
{
	if (m_bDrawMouse)
	{
#ifdef _DEBUG
		pSurface->DrawSetColor(255, 255, 0, 0);

		pSurface->DrawLine(m_iMouseX, 0, m_iMouseX, m_iScreenHeight);
		pSurface->DrawLine(0, m_iMouseY, m_iScreenWidth, m_iMouseY);

		wchar_t pMousePos[32];
		int iLen = swprintf(pMousePos, L"(%d|%d)", m_iMouseX, m_iMouseY);

		pSurface->DrawSetTextColor(255, 255, 255, 255);
		pSurface->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_NORMAL));
		pSurface->DrawSetTextPos(m_iMouseX + 15, m_iMouseY + 15);
		pSurface->DrawPrintText(pMousePos, iLen);
#else // _DEBUG
		pSurface->DrawSetTexture(g_pResourceManager->GetWhiteTexture());

		// TODO:
		Vertex_t v[4];
		v[0].Init(Vector2D((float)m_iMouseX, (float)m_iMouseY));
		v[1].Init(Vector2D((float)m_iMouseX + 20.0f, (float)m_iMouseY + 10.0f));
		v[2].Init(Vector2D((float)m_iMouseX + 7.5f, (float)m_iMouseY + 10.0f));
		v[3].Init(Vector2D((float)m_iMouseX, (float)m_iMouseY + 20.0f));

		pSurface->DrawSetColor(255, 255, 128, 0);
		pSurface->DrawTexturedPolygon(4, v, false);

		pSurface->DrawSetColor(255, 50, 50, 50);
		pSurface->DrawTexturedPolyLine(v, 4);
#endif // _DEBUG
	}
}

bool CGui::WorldToScreen(const Vector& origin, Vector& screen)
{
	if (!ScreenTransform(origin, screen))
	{
		screen.x = (m_iScreenWidth / 2.0f) + (screen.x * m_iScreenWidth) / 2;
		screen.y = (m_iScreenHeight / 2.0f) - (screen.y * m_iScreenHeight) / 2;

		return true;
	}
	return false;
}

bool CGui::ScreenTransform(const Vector& point, Vector& screen)
{
	screen.x = m_pWorldToScreenMatrix->m[0][0] * point.x + m_pWorldToScreenMatrix->m[0][1] * point.y + m_pWorldToScreenMatrix->m[0][2] * point.z + m_pWorldToScreenMatrix->m[0][3];
	screen.y = m_pWorldToScreenMatrix->m[1][0] * point.x + m_pWorldToScreenMatrix->m[1][1] * point.y + m_pWorldToScreenMatrix->m[1][2] * point.z + m_pWorldToScreenMatrix->m[1][3];
	screen.z = 0.0f;

	float w = m_pWorldToScreenMatrix->m[3][0] * point.x + m_pWorldToScreenMatrix->m[3][1] * point.y + m_pWorldToScreenMatrix->m[3][2] * point.z + m_pWorldToScreenMatrix->m[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	return false;
}

unsigned short CGui::XButtonToVKey(WPARAM wParam)
{
	unsigned short vKey = 0;
	switch (GET_XBUTTON_WPARAM(wParam))
	{
	case XBUTTON1:
		vKey = VK_XBUTTON1;
		break;
	case XBUTTON2:
		vKey = VK_XBUTTON2;
		break;
	}

	return vKey;
}

#define WNDPROC_RETURN_OR_BREAK if (!pGui->m_bEnableGameInput) { return 1L; } break

LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT nCode, WPARAM wParam, LPARAM lParam)
{
	CGui* pGui = CGui::Instance();
	CInputHandler* pInputHandler = CInputHandler::Instance();
	int x, y;

	switch (nCode)
	{
	case WM_CHAR:
		pInputHandler->OnText((char)wParam);

		WNDPROC_RETURN_OR_BREAK;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		pInputHandler->OnKeyDown((unsigned short)wParam);

		WNDPROC_RETURN_OR_BREAK;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		pInputHandler->OnKeyUp((unsigned short)wParam);

		WNDPROC_RETURN_OR_BREAK;
	case WM_MOUSEMOVE:
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
		pGui->SetMousePos(x, y);
		pInputHandler->OnMouseMove(x, y);

		WNDPROC_RETURN_OR_BREAK;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		pInputHandler->OnMouseDown(VK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_LBUTTONUP:
		pInputHandler->OnMouseUp(VK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		pInputHandler->OnMouseDown(VK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_RBUTTONUP:
		pInputHandler->OnMouseUp(VK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		pInputHandler->OnMouseDown(VK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_MBUTTONUP:
		pInputHandler->OnMouseUp(VK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_XBUTTONDOWN:
		pInputHandler->OnKeyDown(pGui->XButtonToVKey(wParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_XBUTTONUP:
		pInputHandler->OnKeyUp(pGui->XButtonToVKey(wParam));

		WNDPROC_RETURN_OR_BREAK;
	case WM_MOUSEWHEEL:
		pInputHandler->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		WNDPROC_RETURN_OR_BREAK;
	}

	return CallWindowProc(pGui->m_wndProc, hWnd, nCode, wParam, lParam);
}