#include "Checkbox.h"

CCheckbox::CCheckbox(int x, int y, int w, int h, const char* pText)
	: IControlTooltip(x, y, w, h), m_bIsChecked(false), m_pEventHandler(nullptr)
{
	int offsetX = CHECKBOX_BOXPADDING + CHECKBOX_BOXSIZE;
	m_pLabel = new CLabel(offsetX, 0, m_iWidth - offsetX, m_iHeight, pText, RM_FONT_NORMAL);
	this->AddChild(m_pLabel);
}

CCheckbox::~CCheckbox()
{
}

void CCheckbox::OnClicked()
{
	this->SetChecked(!m_bIsChecked);
}

void CCheckbox::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	Color clrBorderColor;
	Color clrFillColor;
	Color clrTextColor;

	if (!m_bIsEnabled)
	{
		clrBorderColor = g_clrControlDisabled;
		clrFillColor = g_clrControlDisabled;
		clrTextColor = g_clrControlDisabled;
	}
	else if (m_bMouseOver)
	{
		clrBorderColor = g_clrCheckboxBorderHover;
		clrFillColor = g_clrCheckboxFillerHover;
		clrTextColor = g_clrCheckboxTextHover;
	}
	else
	{
		clrBorderColor = g_clrCheckboxBorder;
		clrFillColor = g_clrCheckboxFiller;
		clrTextColor = g_clrCheckboxText;
	}

	// Draw box that holds the checkmark
	pSurface->DrawSetColor(clrBorderColor);
	pSurface->DrawOutlinedRect(
		x,
		y + CHECKBOX_BOXPADDING + ((m_iHeight / 2) - CHECKBOX_BOXSIZE),
		x + CHECKBOX_BOXSIZE,
		y + CHECKBOX_BOXPADDING + CHECKBOX_BOXSIZE + ((m_iHeight / 2) - (CHECKBOX_BOXSIZE)));

	// Draw checkmark if checked
	if (m_bIsChecked)
	{
		pSurface->DrawSetColor(clrFillColor);
		pSurface->DrawFilledRect(
			x + CHECKBOX_FILLERPADDING,
			y + CHECKBOX_BOXPADDING + CHECKBOX_FILLERPADDING + ((m_iHeight / 2) - (CHECKBOX_BOXSIZE)),
			x + CHECKBOX_BOXSIZE - CHECKBOX_FILLERPADDING,
			y + CHECKBOX_BOXPADDING + CHECKBOX_BOXSIZE - CHECKBOX_FILLERPADDING + ((m_iHeight / 2) - (CHECKBOX_BOXSIZE))
		);
	}

	// Set label color
	m_pLabel->SetTextColor(clrTextColor);

	IControlTooltip::Draw(pSurface);
}

bool CCheckbox::ShouldDependentOnesBeEnabled(void* pParam)
{
	return m_bIsChecked;
}

void CCheckbox::SetChecked(bool bIsChecked)
{
	m_bIsChecked = bIsChecked;

	if (m_pEventHandler)
		m_pEventHandler(m_bIsChecked);

	this->NotifyDependentOnes();
}
