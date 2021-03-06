#include "Button.h"

CButton::CButton(int x, int y, int w, int h, const char* pText)
	: IControlClickable(x, y, w, h), m_pEventHandler(nullptr)
{
	// Add Label as child so we get correct positioning (also autocleanup)
	m_pLabel = new CLabel(0, 0, w, h, pText, RM_FONT_NORMAL, LABEL_ORIENTATION_CENTER);
	this->AddChild(m_pLabel);
}

CButton::~CButton()
{
}

void CButton::OnClicked()
{
	if (m_pEventHandler)
		m_pEventHandler();
}

void CButton::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	// Draw button box
	if (m_bMouseDown)
		pSurface->DrawSetColor(g_clrButtonDown);
	else if (m_bMouseOver)
		pSurface->DrawSetColor(g_clrButtonOver);
	else
		pSurface->DrawSetColor(g_clrButton);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	// Draw label
	IControlClickable::Draw(pSurface);
}

bool CButton::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}