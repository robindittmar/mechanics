#include "Slider.h"

CSlider::CSlider(int x, int y, int w, int h, float fMin, float fMax, float fValue) : IControl(x, y, w, h)
{
	m_bHitcheckForMouseMove = false;

	m_fMinValue = fMin;
	m_fMaxValue = fMax;

	m_fValue = fValue;
}

CSlider::~CSlider()
{
}

void CSlider::OnMouseMove(int mx, int my)
{
	if(m_bMouseDown)
	{
		this->SetValueToCursorPos(mx, my);
	}
}

void CSlider::OnMouseDown(int mx, int my)
{
	this->SetValueToCursorPos(mx, my);
}

void CSlider::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawLine(x, y, x, y + m_iHeight);
	pSurface->DrawLine(x, y + (m_iHeight / 2), x + m_iWidth, y + (m_iHeight / 2));
	pSurface->DrawLine(x + m_iWidth, y, x + m_iWidth, y + m_iHeight);

	int knobX = m_fValue * m_iWidth;
	pSurface->DrawSetColor(255, 0, 0, 255);
	pSurface->DrawLine(x + knobX, y, x + knobX, y + m_iHeight);
}

void CSlider::SetValueToCursorPos(int mx, int my)
{
	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int deltax = mx - x;
	if (deltax < 0)
		deltax = 0;
	else if (deltax > m_iWidth)
		deltax = m_iWidth;

	//m_fValue = ((m_fMaxValue - m_fMinValue) * (float)deltax / (float)m_iWidth) + m_fMinValue;
	m_fValue = ((float)deltax / (float)m_iWidth);

	if (m_pEventHandler)
		m_pEventHandler(m_fValue);
}
