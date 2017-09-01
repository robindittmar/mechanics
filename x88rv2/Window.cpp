#include "Window.h"

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle) : IControl(x, y, w, h + TITLEBAR_HEIGHT)
{
	m_bHitcheckForMouseMove = false;
	m_bHitcheckForMouseUp = false;

	m_bIsVisible = false;
	m_bIsDragging = false;

	m_pLabelTitle = new CLabel(0, 0, w, TITLEBAR_HEIGHT, pTitle, RM_FONT_HEADER, LABEL_ORIENTATION_CENTER);
	m_pCanvas = new CCanvas(0, TITLEBAR_HEIGHT, m_iWidth, m_iHeight - TITLEBAR_HEIGHT, g_clrClientRegion);

	IControl::AddChild(m_pLabelTitle);
	IControl::AddChild(m_pCanvas);
}

CWindow::~CWindow()
{
}

void CWindow::OnMouseMove(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	if (m_bIsDragging)
	{
		m_iX = mx - m_iDragOffsetX;
		m_iY = my - m_iDragOffsetY;

		// Prevent clipping X
		if (m_iX < 0)
			m_iX = 0;
		else if (m_iX + m_iWidth > pGui->ScreenWidth())
			m_iX = pGui->ScreenWidth() - m_iWidth;

		// Prevent clipping Y
		if (m_iY < 0)
			m_iY = 0;
		else if (m_iY + m_iHeight > pGui->ScreenHeight())
			m_iY = pGui->ScreenHeight() - m_iHeight;
	}
}

void CWindow::OnMouseDown(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	if (pGui->IsMouseInRect(m_iX, m_iY, m_iWidth, TITLEBAR_HEIGHT))
	{
		m_iDragOffsetX = mx - m_iX;
		m_iDragOffsetY = my - m_iY;
		m_bIsDragging = true;
	}
}

void CWindow::OnMouseUp(int mx, int my)
{
	m_bIsDragging = false;
}

void CWindow::AddChild(IControl* pControl)
{
	m_pCanvas->AddChild(pControl);
}

void CWindow::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible || !m_bIsEnabled)
		return;

	// Draw titlebar
	pSurface->DrawSetColor(g_clrTitlebar);
	pSurface->DrawFilledRect(m_iX, m_iY, m_iX + m_iWidth, m_iY + TITLEBAR_HEIGHT);

	// Client region is drawn by m_pCanvas

	IControl::Draw(pSurface);
}
