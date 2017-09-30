#include "Slider.h"

CSlider::CSlider(int x, int y, int w, int h, float fStepSize, int iOrientation, bool bReverse, float fMin, float fMax) : IControl(x, y, w, h)
{
	m_bHitcheckForMouseMove = false;

	m_iOrientation = iOrientation;
	m_bReverse = bReverse;

	m_fMinValue = fMin;
	m_fMaxValue = fMax;
	m_fValueSpan = m_fMaxValue - m_fMinValue;
	m_fStepSize = fStepSize;

	m_fPossibleValues = NULL;

	if (m_fStepSize != 0.0f)
	{
		float m_fCountPossibleValues = m_fValueSpan / m_fStepSize;
		m_iCountPossibleValues = m_fValueSpan / m_fStepSize;
		if (m_fCountPossibleValues - m_iCountPossibleValues > 0.0f)
			m_iCountPossibleValues += 2;
		else
			m_iCountPossibleValues++;

		m_fPossibleValues = new float[m_iCountPossibleValues];
		float fCurValue = fMin;

		for (int i = 0; i < m_iCountPossibleValues; i++)
		{
			m_fPossibleValues[i] = fCurValue;
			fCurValue += m_fStepSize;
		}
		
		m_fPossibleValues[m_iCountPossibleValues - 1] = fMax;
	}

	m_pLabel = new CLabel(0, 0, 0, 0, "", RM_FONT_NORMAL, iOrientation == SLIDER_ORIENTATION_HORIZONTAL ? LABEL_ORIENTATION_CENTER : LABEL_ORIENTATION_LEFT);
}

CSlider::~CSlider()
{
	if (m_fPossibleValues)
		delete[] m_fPossibleValues;
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
	pSurface->DrawSetColor(g_clrControl);

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

		pSurface->DrawSetColor(g_clrKnob);
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

		pSurface->DrawSetColor(g_clrKnob);
		pSurface->DrawFilledRect((x + m_iWidth / 2) - SLIDER_KNOBSIZE / 2, (y + knob) - SLIDER_KNOBSIZE / 2, (x + m_iWidth / 2) + SLIDER_KNOBSIZE / 2, (y + knob) + SLIDER_KNOBSIZE / 2);
		//pSurface->DrawLine(x, y + knob, x + m_iWidth, y + knob);
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

	fDelta /= fRefMax;
	fDelta = (fDelta * m_fValueSpan) + m_fMinValue;

	if (m_fStepSize != 0.0f)
	{
		for (int i = 0; i < m_iCountPossibleValues; i++)
		{
			if (fDelta > m_fPossibleValues[i] && fDelta < m_fPossibleValues[i + 1])
			{
				float fDist1 = fDelta - m_fPossibleValues[i];
				float fDist2 = m_fPossibleValues[i + 1] - fDelta;

				if (fDist1 < fDist2)
				{
					fDelta = m_fPossibleValues[i];
				}
				else
				{
					fDelta = m_fPossibleValues[i + 1];
				}
				break;
			}
		}
	}

	this->SetValue(fDelta);
	this->SetLabelText();

	if (m_pEventHandler)
		m_pEventHandler(m_fValue);
}

void CSlider::SetLabelText()
{
	char pBuffer[16];
	snprintf(pBuffer, 16, "%.2f", m_fValue);
	m_pLabel->SetContentText(pBuffer);
}
