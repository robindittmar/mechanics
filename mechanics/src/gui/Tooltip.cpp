#include "Tooltip.h"
#include "Label.h"

CTooltip::CTooltip()
{
	m_pLabel = new CLabel();
}

CTooltip::~CTooltip()
{
	if (m_pLabel)
		delete m_pLabel;
}

void CTooltip::SetText(const char* pText)
{
	m_pLabel->SetContentText(pText);
}

void CTooltip::SetPosition(int x, int y)
{
	m_iX = x;
	m_iY = y;
}

void CTooltip::Draw(ISurface* pSurface)
{
	int x, y, width, height;
	m_pLabel->GetTextSize(pSurface, width, height);
	m_pLabel->SetBoundaries(m_iX, m_iY, width, height);

	x = m_iX - TOOLTIP_PADDING_TO_TEXT;
	y = m_iY - TOOLTIP_PADDING_TO_TEXT;
	width += 2 * TOOLTIP_PADDING_TO_TEXT;
	height += 2 * TOOLTIP_PADDING_TO_TEXT;

	pSurface->DrawSetColor(g_clrTooltipBackground);
	pSurface->DrawFilledRect(x, y, x + width, y + height);
	
	pSurface->DrawSetColor(g_clrTooltipBorder);
	pSurface->DrawOutlinedRect(x, y, x + width, y + height);

	m_pLabel->Draw(pSurface);
}