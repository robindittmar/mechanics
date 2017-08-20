#include "Gui.h"
#include "Application.h"

CGui::CGui()
{
	m_bDrawMouse = false;
}

CGui::~CGui()
{
}

CGui* CGui::Instance()
{
	static CGui inst;
	return &inst;
}

void CGui::GetScreenSize()
{
	CApplication::Instance()->EngineClient()->GetScreenSize(m_iScreenWidth, m_iScreenHeight);
}

void CGui::EnableIngameMouse()
{
	static CXorString xorEnableMouse("tgگx~��re�{n��");
	CApplication::Instance()->EngineClient()->ClientCmd(xorEnableMouse.ToCharArray());
}

void CGui::DisableIngameMouse()
{
	static CXorString xorDisableMouse("tgگx~��re�{n��");
	CApplication::Instance()->EngineClient()->ClientCmd(xorDisableMouse.ToCharArray());
}

bool CGui::GetMousePos()
{
	POINT p;
	if(GetCursorPos(&p))
	{
		// TODO: Xor or remove ghetto fix for actual implementation :D
		// Gh3tt0 f!x
		static HWND hWnd = FindWindow(NULL, CXorString("Td�cn��D��|n��Pg�vg��qm�db�").ToCharArray());
		if(hWnd)
		{
			ScreenToClient(hWnd, &p);
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
		pSurface->DrawSetColor(255, 0, 0, 128);
		pSurface->DrawLine(m_iMouseX, m_iMouseY, m_iMouseX, m_iMouseY + 10);
		pSurface->DrawLine(m_iMouseX, m_iMouseY, m_iMouseX + 10, m_iMouseY);
		pSurface->DrawLine(m_iMouseX + 10, m_iMouseY, m_iMouseX, m_iMouseY + 10);
	}
}

bool CGui::IsMouseInRect(int x, int y, int w, int h)
{
	if(	m_iMouseX >= x && m_iMouseX <= (x + w) &&
		m_iMouseY >= y && m_iMouseY <= (y + h))
	{
		return true;
	}

	return false;
}
