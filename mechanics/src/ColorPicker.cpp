#include "ColorPicker.h"
#include "Window.h"

CColorPicker::CColorPicker(int x, int y, int w, int h)
	: IControlTooltip(x, y, w, h), m_pEventHandler(nullptr), m_clrValue(Color(0, 0, 0, 0))
{
	m_bPopupInitialized = false;
	m_pPopup = new CColorPickerPopup(0, m_iHeight, this);
}

CColorPicker::~CColorPicker()
{
	if (m_pPopup)
		delete m_pPopup;
}

void CColorPicker::SetValue(Color clrValue)
{
	m_clrValue = clrValue;

	if (m_pEventHandler)
		m_pEventHandler(m_clrValue);
}

void CColorPicker::OnClicked()
{
	CWindow* pWindow = (CWindow*)this->GetParentWindow();

	if (!m_bPopupInitialized)
	{
		m_pPopup->SetParentWindow(pWindow);
		m_bPopupInitialized = true;
	}

	m_pPopup->OnOpen();
	pWindow->SetPopup(m_pPopup);
}

void CColorPicker::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	pSurface->DrawSetColor(m_clrValue);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawOutlinedRect(x, y, x + m_iWidth, y + m_iHeight);

	IControlTooltip::Draw(pSurface);
}

bool CColorPicker::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}