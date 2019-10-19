#include "IControlTooltip.h"
#include "../Application.h"

IControlTooltip::IControlTooltip(int x, int y, int w, int h)
	: IControlClickable(x, y, w, h), m_pTooltip(nullptr)
{
}

IControlTooltip::~IControlTooltip()
{
	if (m_pTooltip)
		delete m_pTooltip;
}

void IControlTooltip::OnMouseMove(int mx, int my)
{
	if (m_bMouseOver)
	{
		if (m_pTooltip)
		{
			m_fTimeLastMouseMovement = CApplication::Instance()->GlobalVars()->curtime;
			m_pTooltip->SetPosition(mx + TOOLTIP_DISTANCE_TO_MOUSE_X, my + TOOLTIP_DISTANCE_TO_MOUSE_Y);
		}
	}
}

void IControlTooltip::SetTooltipText(const char* pTooltipText)
{
	if (!m_pTooltip)
		m_pTooltip = new CTooltip();

	m_pTooltip->SetText(pTooltipText);
}

void IControlTooltip::Draw(ISurface* pSurface)
{
	IControlClickable::Draw(pSurface);

	if (m_pTooltip && m_bMouseOver)
	{
		if (CApplication::Instance()->GlobalVars()->curtime - m_fTimeLastMouseMovement > TOOLTIP_TIME_TO_DISPLAY)
		{
			CWindow* pParentWindow = (CWindow*)this->GetParentWindow();
			pParentWindow->SetTooltip(m_pTooltip);
		}
	}
}