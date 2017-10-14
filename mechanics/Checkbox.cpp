#include "Checkbox.h"

CCheckbox::CCheckbox(int x, int y, int w, int h, const char* pText, bool isChecked) : IControl(x, y, w, h)
{
	m_bIsChecked = isChecked;
	m_pEventHandler = NULL;

	int offsetX = CHECKBOX_BOXPADDING + CHECKBOX_BOXSIZE;
	m_pLabel = new CLabel(offsetX, 0, m_iWidth - offsetX, m_iHeight, pText, RM_FONT_NORMAL);
	this->AddChild(m_pLabel);
}

CCheckbox::~CCheckbox()
{
	m_pEnableOnChecked.clear();
	m_pDisableOnChecked.clear();
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

	// Draw label
	m_pLabel->SetTextColor(clrTextColor);
	//m_pLabel->Draw(pSurface);
	IControl::Draw(pSurface);
}

void CCheckbox::SetChecked(bool bIsChecked)
{
	m_bIsChecked = bIsChecked;

	if (m_pEventHandler)
		m_pEventHandler(m_bIsChecked);

	for (std::vector<IControl*>::iterator it = m_pEnableOnChecked.begin(); it != m_pEnableOnChecked.end(); it++)
	{
		(*it)->SetEnabled(m_bIsChecked);
	}

	for (std::vector<IControl*>::iterator it = m_pDisableOnChecked.begin(); it != m_pDisableOnChecked.end(); it++)
	{
		(*it)->SetEnabled(!m_bIsChecked);
	}
}

void CCheckbox::EnableOnChecked(IControl* p)
{
	m_pEnableOnChecked.push_back(p);
}

void CCheckbox::DisableOnChecked(IControl* p)
{
	m_pDisableOnChecked.push_back(p);
}
