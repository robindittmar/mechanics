#include "Groupbox.h"

CGroupbox::CGroupbox(int x, int y, int w, int h, const char* pTitle)
	: IControl(x, y, w, h), m_bAlreadySetSize(false)
{
	m_pCanvas = new CCanvas(GROUPBOX_PADDING, GROUPBOX_PADDING);
	m_pLabel = new CLabel(0, 0, w, 0, pTitle, RM_FONT_NORMAL, LABEL_ORIENTATION_CENTER);

	// Using IControl:: because we want to use the original AddChild,
	// not our overridden one :)
	IControl::AddChild(m_pLabel);
	IControl::AddChild(m_pCanvas);
}

CGroupbox::~CGroupbox()
{
}

void CGroupbox::AddChild(IControl* pControl)
{
	m_pCanvas->AddChild(pControl);
}

void CGroupbox::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int titleWidth, titleHeight;
	m_pLabel->GetTextSize(pSurface, titleWidth, titleHeight);

	if(!m_bAlreadySetSize)
	{
		m_pCanvas->SetBoundaries(GROUPBOX_PADDING, titleHeight, m_iWidth - (GROUPBOX_PADDING * 2), m_iHeight - titleHeight - GROUPBOX_PADDING);
		m_bAlreadySetSize = true;
	}

	int topLineY = y/* + titleHeight / 2*/;

	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawLine(x, topLineY, x + (m_iWidth / 2) - (titleWidth / 2) - GROUPBOX_PADDING, topLineY);
	pSurface->DrawLine(x + (m_iWidth / 2) + (titleWidth / 2) + GROUPBOX_PADDING, topLineY, x + m_iWidth, topLineY);
	pSurface->DrawLine(x + m_iWidth, topLineY, x + m_iWidth, y + m_iHeight);
	pSurface->DrawLine(x + m_iWidth, y + m_iHeight, x, y + m_iHeight);
	pSurface->DrawLine(x, y + m_iHeight, x, topLineY);
	//pSurface->DrawOutlinedRect(x, y + titleHeight / 2, x + m_iWidth, y + m_iHeight);

	IControl::Draw(pSurface);
}

bool CGroupbox::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}