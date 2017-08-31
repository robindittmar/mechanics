#include "TabContainer.h"

CTabContainer::CTabContainer() : IControl(0, 0, 0, 0)
{
	m_iCountTabs = 0;

	m_iMouseOverTab = -1;
	m_iSelectedTab = -1;
}

CTabContainer::~CTabContainer()
{
	// Cleanup labels
	CLabel* pCurrent;
	for(std::vector<CLabel*>::iterator it = m_vLabels.begin(); it != m_vLabels.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete pCurrent;
	}
}

void CTabContainer::SelectTab(int idx)
{
	m_iSelectedTab = -1;

	for(int i = 0; i < m_iCountTabs; i++)
	{
		if(i == idx)
		{
			m_pChildren[i]->SetEnabled(true);
			m_iSelectedTab = i;
		}
		else
		{
			m_pChildren[i]->SetEnabled(false);
		}
	}
}

void CTabContainer::OnMouseMove(int mx, int my)
{
	m_iMouseOverTab = -1;

	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	int iCurX;
	int iCurY = y;
	int iCurWidth = iSizePerTab;
	int iCurHeight = TABCONTAINER_TABHEIGHT;

	for (int i = 0; i < m_iCountTabs; i++)
	{
		iCurX = x + (iSizePerTab * i);
		if (pGui->IsMouseInRect(iCurX, iCurY, iCurWidth, iCurHeight))
		{
			m_iMouseOverTab = i;
			break;
		}
	}
}

void CTabContainer::OnClicked()
{
	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	int iCurX;
	int iCurY = y;
	int iCurWidth = iSizePerTab;
	int iCurHeight = TABCONTAINER_TABHEIGHT;

	for (int i = 0; i < m_iCountTabs; i++)
	{
		iCurX = x + (iSizePerTab * i);
		if (pGui->IsMouseInRect(iCurX, iCurY, iCurWidth, iCurHeight))
		{
			this->SelectTab(i);
			break;
		}
	}
}

void CTabContainer::AddChild(IControl* pControl)
{
	IControl::AddChild(pControl);

	m_vLabels.push_back(
		new CLabel(
			0,
			0,
			0,
			0,
			((CTabPage*)pControl)->GetTitle(),
			RM_FONT_SUBHEADER,
			LABEL_ORIENTATION_CENTER
		)
	);
	m_iCountTabs++;
}

void CTabContainer::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	m_iWidth = m_pParent->Width();
	m_iHeight = m_pParent->Height();

	pSurface->DrawSetColor(g_clrTab);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + TABCONTAINER_TABHEIGHT);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	int iCurX;
	int iCurY = y;
	int iCurWidth = iSizePerTab;
	int iCurHeight = TABCONTAINER_TABHEIGHT;
	
	CLabel* pLabel;
	for(int i = 0; i < m_iCountTabs; i++)
	{
		iCurX = x + (iSizePerTab * i);
		pLabel = (CLabel*)m_vLabels[i];

		if(m_iSelectedTab == i)
		{
			pSurface->DrawSetColor(g_clrTabSelected);
			pSurface->DrawFilledRect(iCurX, iCurY, iCurX + iCurWidth, iCurY + iCurHeight);
		}
		else if(m_iMouseOverTab == i && m_bMouseOver)
		{
			pSurface->DrawSetColor(g_clrTabOver);
			pSurface->DrawFilledRect(iCurX, iCurY, iCurX + iCurWidth, iCurY + iCurHeight);
		}

		pLabel->SetBoundaries(iCurX, iCurY, iCurWidth, iCurHeight);
		pLabel->Draw(pSurface);
	}

	IControl::Draw(pSurface);
}
