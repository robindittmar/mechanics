#include "Checkbox.h"

CCheckbox::CCheckbox(int x, int y, int w, int h, const char* pText, bool isChecked) : IControl(x, y, w, h)
{
	m_bIsChecked = isChecked;
	m_pEventHandler = NULL;

	int offsetX = (CHECKBOX_BOXPADDING * 2) + CHECKBOX_BOXSIZE;
	m_pLabel = new CLabel(offsetX, 0, m_iWidth - offsetX, m_iHeight, pText, RM_FONT_NORMAL);
	this->AddChild(m_pLabel);
}

CCheckbox::~CCheckbox()
{
}

void CCheckbox::OnClicked()
{
	m_bIsChecked = !m_bIsChecked;

	if (m_pEventHandler)
		m_pEventHandler(m_bIsChecked);
}

void CCheckbox::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	Color borderColor;
	Color fillColor;
	Color textColor;
	if(m_bMouseOver)
	{
		borderColor = g_clrCheckboxBorderHover;
		fillColor = g_clrCheckboxFillerHover;
		textColor = g_clrCheckboxTextHover;
	}
	else
	{
		borderColor = g_clrCheckboxBorder;
		fillColor = g_clrCheckboxFiller;
		textColor = g_clrCheckboxText;
	}

	// Draw box that holds the checkmark
	pSurface->DrawSetColor(borderColor);
	pSurface->DrawOutlinedRect(x + CHECKBOX_BOXPADDING, y + CHECKBOX_BOXPADDING, x + m_iHeight - CHECKBOX_BOXPADDING, y + m_iHeight - CHECKBOX_BOXPADDING);

	// Draw checkmark if checked
	if (m_bIsChecked)
	{
		pSurface->DrawSetColor(fillColor);
		pSurface->DrawFilledRect(
			x + CHECKBOX_BOXPADDING + CHECKBOX_FILLERPADDING,
			y + CHECKBOX_BOXPADDING + CHECKBOX_FILLERPADDING,
			x + m_iHeight - CHECKBOX_BOXPADDING - CHECKBOX_FILLERPADDING,
			y + m_iHeight - CHECKBOX_BOXPADDING - CHECKBOX_FILLERPADDING
		);
	}

	// Draw label
	m_pLabel->SetTextColor(textColor);
	m_pLabel->Draw(pSurface);
}
