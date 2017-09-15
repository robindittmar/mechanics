#include "Slider.h"

CSlider::CSlider(int x, int y, int w, int h, float fDisplayValue, int iOrientation, bool bReverse, float fMin, float fMax) : IControl(x, y, w, h)
{
	m_bHitcheckForMouseMove = false;

	m_iOrientation = iOrientation;
	m_bReverse = bReverse;

	m_fMinValue = fMin;
	m_fMaxValue = fMax;
	m_fValueSpan = m_fMaxValue - m_fMinValue;

	m_pLabel = new CLabel(0, 0, 0, 0, "", RM_FONT_NORMAL, iOrientation == SLIDER_ORIENTATION_HORIZONTAL ? LABEL_ORIENTATION_CENTER : LABEL_ORIENTATION_LEFT);
	this->SetDisplayValue(fDisplayValue);
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

	int knob;
	pSurface->DrawSetColor(255, 0, 0, 0);

	switch(m_iOrientation)
	{
	case SLIDER_ORIENTATION_HORIZONTAL:
		pSurface->DrawLine(x, y, x, y + m_iHeight);
		pSurface->DrawLine(x, y + (m_iHeight / 2), x + m_iWidth, y + (m_iHeight / 2));
		pSurface->DrawLine(x + m_iWidth, y, x + m_iWidth, y + m_iHeight);

		knob = m_fValue * m_iWidth;
		if(m_bReverse)
			knob = m_iWidth - knob;

		pSurface->DrawSetColor(255, 0, 0, 255);
		pSurface->DrawLine(x + knob, y, x + knob, y + m_iHeight);
		// TODO: 18? 16? pls
		m_pLabel->SetBoundaries(x + knob, y - 18, 1, 16);
		break;
	case SLIDER_ORIENTATION_VERTICAL:
		pSurface->DrawLine(x, y, x + m_iWidth, y);
		pSurface->DrawLine(x + (m_iWidth / 2), y, x + (m_iWidth / 2), y + m_iHeight);
		pSurface->DrawLine(x, y + m_iHeight, x + m_iWidth, y + m_iHeight);

		knob = m_fValue * m_iHeight;
		if (m_bReverse)
			knob = m_iHeight - knob;

		pSurface->DrawSetColor(255, 0, 0, 255);
		pSurface->DrawLine(x, y + knob, x + m_iWidth, y + knob);
		// TODO:
		m_pLabel->SetBoundaries(x + m_iWidth + 4, y + knob, 1, 1);
		break;
	default:
		break;
	}

	m_pLabel->Draw(pSurface);
}

void CSlider::SetValue(float fValue)
{
	m_fValue = fValue;
	m_fDisplayValue = (m_fValueSpan * m_fValue) + m_fMinValue;
}

void CSlider::SetDisplayValue(float fDisplayValue)
{
	m_fDisplayValue = fDisplayValue;
	m_fValue = (m_fDisplayValue - m_fMinValue) / m_fValueSpan;
	this->SetLabelText();
}

void CSlider::SetValueToCursorPos(int mx, int my)
{
	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	float fDelta, fRefMax;
	switch (m_iOrientation)
	{
	case SLIDER_ORIENTATION_HORIZONTAL:
		fDelta = mx - x;
		fRefMax = m_iWidth;
		break;
	case SLIDER_ORIENTATION_VERTICAL:
		fDelta = my - y;
		fRefMax = m_iHeight;
		break;
	default:
		break;
	}

	if(m_bReverse)
	{
		fDelta = fRefMax - fDelta;
	}

	if (fDelta < 0.0f)
		fDelta = 0.0f;
	else if (fDelta > fRefMax)
		fDelta = fRefMax;

	this->SetValue(fDelta / fRefMax);
	this->SetLabelText();

	if (m_pEventHandler)
		m_pEventHandler(m_fDisplayValue);
}

void CSlider::SetLabelText()
{
	char pBuffer[16];
	snprintf(pBuffer, 16, "%.2f", m_fDisplayValue);
	m_pLabel->SetContentText(pBuffer);
}
