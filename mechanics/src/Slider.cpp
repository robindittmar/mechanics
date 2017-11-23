#include "Slider.h"

CSlider::CSlider(int x, int y, int w, int h, float fStepSize, int iOrientation, bool bReverse, float fMin, float fMax, const char* pText)
	: IControlTooltip(x, y, w, h), m_iOrientation(iOrientation),
	m_bReverse(bReverse), m_fMinValue(fMin), m_fMaxValue(fMax),
	m_fStepSize(fStepSize)
{
	m_bHitcheckForMouseMove = false;
	m_fValueSpan = m_fMaxValue - m_fMinValue;

	m_pLabel = new CLabel(0, -16, 0, 0, "", RM_FONT_NORMAL, iOrientation == SLIDER_ORIENTATION_HORIZONTAL ? LABEL_ORIENTATION_CENTER : LABEL_ORIENTATION_LEFT);

	if (pText)
	{
		m_pTextLabel = new CLabel(0, -18, 0, 0, pText, RM_FONT_NORMAL);
		this->AddChild(m_pTextLabel);
	}
}

CSlider::~CSlider()
{
	if (m_pLabel)
		delete m_pLabel;
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
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	Color clrControl;
	Color clrKnob;

	if (!m_bIsEnabled)
	{
		clrControl = g_clrControlDisabled;
		clrKnob = g_clrControlDisabled;
	}
	else
	{
		clrControl = g_clrControl;
		clrKnob = g_clrKnob;
	}

	int knob;
	pSurface->DrawSetColor(clrControl);

	float fValue = (m_fValue - m_fMinValue) / m_fValueSpan;

	switch(m_iOrientation)
	{
	case SLIDER_ORIENTATION_HORIZONTAL:
		//pSurface->DrawLine(x, y, x, y + m_iHeight);
		pSurface->DrawLine(x, y + (m_iHeight / 2), x + m_iWidth, y + (m_iHeight / 2));
		//pSurface->DrawLine(x + m_iWidth, y, x + m_iWidth, y + m_iHeight);

		knob = fValue * m_iWidth;
		if (m_bReverse)
			knob = m_iWidth - knob;

		pSurface->DrawSetColor(clrKnob);
		pSurface->DrawFilledRect((x + knob) - SLIDER_KNOBSIZE / 2, (y + m_iHeight / 2) - (SLIDER_KNOBSIZE / 2), (x + knob) + SLIDER_KNOBSIZE / 2, (y + m_iHeight / 2) + (SLIDER_KNOBSIZE / 2));
		//pSurface->DrawLine(x + knob, y, x + knob, y + m_iHeight);
		// TODO: 18? 16? pls
		m_pLabel->SetBoundaries(x + knob, y - 11, 1, 16);
		break;
	case SLIDER_ORIENTATION_VERTICAL:
		//pSurface->DrawLine(x, y, x + m_iWidth, y);
		pSurface->DrawLine(x + (m_iWidth / 2), y, x + (m_iWidth / 2), y + m_iHeight);
		//pSurface->DrawLine(x, y + m_iHeight, x + m_iWidth, y + m_iHeight);

		knob = fValue * m_iHeight;
		if (m_bReverse)
			knob = m_iHeight - knob;

		pSurface->DrawSetColor(clrKnob);
		pSurface->DrawFilledRect((x + m_iWidth / 2) - SLIDER_KNOBSIZE / 2, (y + knob) - SLIDER_KNOBSIZE / 2, (x + m_iWidth / 2) + SLIDER_KNOBSIZE / 2, (y + knob) + SLIDER_KNOBSIZE / 2);
		//pSurface->DrawLine(x, y + knob, x + m_iWidth, y + knob);
		// TODO:
		m_pLabel->SetBoundaries(x + m_iWidth + 4, y + knob, 1, 1);
		break;
	default:
		break;
	}

	m_pLabel->Draw(pSurface);
	IControlTooltip::Draw(pSurface);
}

bool CSlider::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}

void CSlider::SetEnabled(bool bIsEnabled)
{
	IControl::SetEnabled(bIsEnabled);

	if (m_bIsEnabled)
	{
		m_pLabel->SetTextColor(Color(255, 255, 255, 255));
	}
	else
	{
		m_pLabel->SetTextColor(g_clrControlDisabled);
	}
}

void CSlider::SetValue(float fValue)
{
	if (m_fStepSize != 0.0f)
		m_fValue = (float)((int)((fValue / m_fStepSize) + 0.5f) * m_fStepSize);
	else
		m_fValue = fValue;

	this->SetLabelText();

	if (m_pEventHandler)
		m_pEventHandler(m_fValue);
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

	fDelta /= fRefMax;
	fDelta = (fDelta * m_fValueSpan) + m_fMinValue;
	this->SetValue(fDelta);
}

void CSlider::SetLabelText()
{
	char pBuffer[16];
	snprintf(pBuffer, 16, "%.2f", m_fValue);
	m_pLabel->SetContentText(pBuffer);
}
