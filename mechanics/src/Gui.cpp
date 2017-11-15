#include "Gui.h"
#include "Application.h"

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

	this->GetWorldToScreenMatrix();

	// Screen size
	m_pApp->EngineClient()->GetScreenSize(m_iScreenWidth, m_iScreenHeight);

	// cl_mouseenable
	m_pMouseEnable = m_pApp->CVar()->FindVar(CXorString("tgÚ¯x~ö§reä {n").ToCharArray());

	// Game Window
	m_hGameWindow = FindWindow(CXorString("Ajé´r;µó").ToCharArray(), NULL);
	while (!m_hGameWindow)
	{
		Sleep(1000);
		m_hGameWindow = FindWindow(CXorString("Ajé´r;µó").ToCharArray(), NULL);
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

void CGui::GetWorldToScreenMatrix()
{
	// Pointer to WorldToScreenMatrix
	m_pWorldToScreenMatrix = &m_pApp->EngineClient()->WorldToScreenMatrix();
}

void CGui::SetEnableGameInput(bool bEnableGameInput)
{
	m_bEnableGameInput = bEnableGameInput;
	m_pMouseEnable->SetValue((int)m_bEnableGameInput);
}

bool CGui::IsMouseEnabled()
{
	return (m_pMouseEnable->value[0] != '0');
}

bool CGui::IsMouseInRect(int x, int y, int w, int h)
{
	if (m_iMouseX >= x && m_iMouseX <= (x + w) &&
		m_iMouseY >= y && m_iMouseY <= (y + h))
	{
		return true;
	}

	return false;
}

bool CGui::GetMousePos()
{
	POINT p;
	if(GetCursorPos(&p))
	{
		// Gh3tt0 f!x
		if (m_hGameWindow)
		{
			ScreenToClient(m_hGameWindow, &p);
		}
		
		m_iMouseX = p.x;
		m_iMouseY = p.y;

		return true;
	}

	return false;
}

void CGui::DrawMouse(ISurface* pSurface)
{
	if(m_bDrawMouse)
	{
		pSurface->DrawSetColor(255, 255, 255, 255);
		pSurface->DrawSetTexture(g_pResourceManager->GetTexture(RM_TEXTURE_CURSOR));

		// TODO:
		Vertex_t v[4];
		v[0].Init(Vector2D((float)m_iMouseX, (float)m_iMouseY));
		v[1].Init(Vector2D((float)m_iMouseX + 20.0f, (float)m_iMouseY + 10.0f));
		v[2].Init(Vector2D((float)m_iMouseX + 7.5f, (float)m_iMouseY + 10.0f));
		v[3].Init(Vector2D((float)m_iMouseX, (float)m_iMouseY + 20.0f));
		pSurface->DrawTexturedPolygon(4, v, false);

		/*pSurface->DrawLine(m_iMouseX, m_iMouseY, m_iMouseX, m_iMouseY + 10);
		pSurface->DrawLine(m_iMouseX, m_iMouseY, m_iMouseX + 10, m_iMouseY);
		pSurface->DrawLine(m_iMouseX + 10, m_iMouseY, m_iMouseX, m_iMouseY + 10);*/
	}
}

bool CGui::WorldToScreen(const Vector &origin, Vector &screen)
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

LRESULT CALLBACK hk_WndProc(HWND hWnd, UINT nCode, WPARAM wParam, LPARAM lParam)
{
	CGui* pGui = CGui::Instance();
	if (!pGui->m_bEnableGameInput)
	{
		switch (nCode)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return 1L;
		}
	}

	return CallWindowProc(pGui->m_wndProc, hWnd, nCode, wParam, lParam);
}