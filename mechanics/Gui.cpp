#include "Gui.h"
#include "Application.h"

CGui::CGui()
{
	m_pWorldToScreenMatrix = NULL;
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

void CGui::Setup()
{
	m_pApp = CApplication::Instance();

	// Pointer to WorldToScreenMatrix
	m_pWorldToScreenMatrix = &m_pApp->EngineClient()->WorldToScreenMatrix();

	// Screen size
	m_pApp->EngineClient()->GetScreenSize(m_iScreenWidth, m_iScreenHeight);

	// cl_mouseenable
	m_pMouseEnable = m_pApp->CVar()->FindVar(CXorString("tgÚ¯x~ö§reä {n").ToCharArray());
}

void CGui::GetWorldToScreenMatrix()
{
	
}

void CGui::SetEnableMouse(bool bEnableMouse)
{
	m_pMouseEnable->SetValue((int)bEnableMouse);
}

bool CGui::GetMousePos()
{
	POINT p;
	if(GetCursorPos(&p))
	{
		// TODO: Xor or remove ghetto fix for actual implementation :D
		// Gh3tt0 f!x
		static HWND hWnd = FindWindow(CXorString("Ajé´r;µó").ToCharArray(), NULL);
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

bool CGui::IsMouseInRect(int x, int y, int w, int h)
{
	if(	m_iMouseX >= x && m_iMouseX <= (x + w) &&
		m_iMouseY >= y && m_iMouseY <= (y + h))
	{
		return true;
	}

	return false;
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