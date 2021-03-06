#include "Window.h"
#include "../Application.h" // Remove pls

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle)
	: IControlClickable(x, y, w, h + TITLEBAR_HEIGHT)
{
	m_bHitcheckForMouseMove = false;
	m_bHitcheckForMouseUp = false;
	m_bCanHaveFocus = true;

	m_bIsVisible = false;
	m_bIsDragging = false;

	m_pPopup = NULL;
	m_pTooltip = NULL;

	m_pLabelTitle = new CLabel(0, 0, w, TITLEBAR_HEIGHT, pTitle, RM_FONT_HEADER, LABEL_ORIENTATION_CENTER);
	m_pCanvas = new CCanvas(0, TITLEBAR_HEIGHT, m_iWidth, m_iHeight - TITLEBAR_HEIGHT, g_clrClientRegion);

	// Using IControl:: because we want to call original AddChild
	IControl::AddChild(m_pLabelTitle);
	IControl::AddChild(m_pCanvas);
}

CWindow::~CWindow()
{
}

void CWindow::OnMouseMove(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	if (m_pTooltip)
		m_pTooltip = NULL;

	if (m_bIsDragging)
	{
		m_iX = mx - m_iDragOffsetX;
		m_iY = my - m_iDragOffsetY;
		
		// Clamp X
		if (m_iX < 0)
			m_iX = 0;
		else if (m_iX + m_iWidth > pGui->GetScreenWidth())
			m_iX = pGui->GetScreenWidth() - m_iWidth;

		// Clamp Y
		if (m_iY < 0)
			m_iY = 0;
		else if (m_iY + m_iHeight > pGui->GetScreenHeight())
			m_iY = pGui->GetScreenHeight() - m_iHeight;
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

void CWindow::ProcessEvent(CInputEvent* pEvent)
{
	if (m_pPopup)
		m_pPopup->ProcessEvent(pEvent);
	else
		IControlClickable::ProcessEvent(pEvent);
}

void CWindow::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible || !m_bIsEnabled)
		return;

	// Draw titlebar
	pSurface->DrawSetColor(g_clrTitlebar);
	pSurface->DrawFilledRect(m_iX, m_iY, m_iX + m_iWidth, m_iY + TITLEBAR_HEIGHT);

	// Client region is drawn by m_pCanvas
	IControlClickable::Draw(pSurface);

	// Render tooltip if we have one
	if (m_pTooltip)
		m_pTooltip->Draw(pSurface);

	// Render Popup if we have one
	if (m_pPopup)
		m_pPopup->Draw(pSurface);
}

bool CWindow::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}

void CWindow::SetClientPosition(int x, int y)
{
	this->SetPosition(x, y - TITLEBAR_HEIGHT);
}

void CWindow::GetClientPosition(int* x, int* y)
{
	*x = m_iX;
	*y = m_iY + TITLEBAR_HEIGHT;
}

void CWindow::GetClientSize(int* w, int* h)
{
	*w = m_iWidth;
	*h = m_iHeight - TITLEBAR_HEIGHT;
}