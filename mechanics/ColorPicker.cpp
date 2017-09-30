#include "ColorPicker.h"
#include "Window.h"

CColorPicker::CColorPicker(int x, int y, int w, int h) : IControl(x, y, w, h)
{
	m_bPopupInitialized = false;
	m_pPopup = NULL;

	m_clrValue = Color(255, 255, 255);
	
}

CColorPicker::~CColorPicker()
{
	if (m_pPopup)
		delete m_pPopup;
}

void CColorPicker::OnClicked()
{
	CWindow* pWindow = (CWindow*)this->GetParentWindow();

	if (!m_bPopupInitialized)
	{
		m_pPopup->SetColorPicker(this);
		m_pPopup->SetParentWindow(pWindow);
	}

	pWindow->SetPopup(m_pPopup);
}

void CColorPicker::Draw(ISurface* pSurface)
{
	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	pSurface->DrawSetColor(m_clrValue);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawOutlinedRect(x, y, x + m_iWidth, y + m_iHeight);
}
