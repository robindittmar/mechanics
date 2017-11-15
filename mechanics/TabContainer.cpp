#include "TabContainer.h"
#include "Window.h"

CTabContainer::CTabContainer() : IControl(0, 0, 0, 0)
{
	m_iCountTabs = 0;

	m_iTabMouseDown = -1;
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
		m_pChildren[i]->SetVisible(false);
	}

	m_pChildren[idx]->SetVisible(true);
	m_iSelectedTab = idx;
}

void CTabContainer::OnMouseMove(int mx, int my)
{
	m_iMouseOverTab = -1;

	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	int iCurX;
	for (int i = 0; i < m_iCountTabs; i++)
	{
		iCurX = x + (iSizePerTab * i);
		if (pGui->IsMouseInRect(iCurX, y, iSizePerTab, TABCONTAINER_TABHEIGHT))
		{
			m_iMouseOverTab = i;
			break;
		}
	}
}

void CTabContainer::OnMouseDown(int mx, int my)
{
	m_iTabMouseDown = -1;

	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	int iCurX;
	for (int i = 0; i < m_iCountTabs; i++)
	{
		iCurX = x + (iSizePerTab * i);
		if (pGui->IsMouseInRect(iCurX, y, iSizePerTab, TABCONTAINER_TABHEIGHT))
		{
			m_iTabMouseDown = i;
			break;
		}
	}
}

void CTabContainer::OnMouseUp(int mx, int my)
{
	if (m_iTabMouseDown == -1)
		return;

	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iSizePerTab = m_iWidth / m_iCountTabs;
	if (pGui->IsMouseInRect(x + (iSizePerTab * m_iTabMouseDown), y, iSizePerTab, TABCONTAINER_TABHEIGHT))
	{
		this->SelectTab(m_iTabMouseDown);
	}

	m_iTabMouseDown = -1;
}

void CTabContainer::AddChild(IControl* pControl)
{
	pControl->SetVisible(false);
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
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	CWindow* pParentWindow = (CWindow*)this->GetParentWindow();
	pParentWindow->GetClientSize(&m_iWidth, &m_iHeight);

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
		else if(m_iTabMouseDown == i)
		{
			pSurface->DrawSetColor(g_clrTabMouseDown);
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

	pSurface->DrawSetColor(g_clrSplitline);
	pSurface->DrawLine(x, y - 1, x, y + TABCONTAINER_TABHEIGHT);
	pSurface->DrawLine(x, y + TABCONTAINER_TABHEIGHT - 1, x + m_iWidth - 1, y + TABCONTAINER_TABHEIGHT - 1);
	pSurface->DrawLine(x + m_iWidth - 1, y - 1, x + m_iWidth - 1, y + TABCONTAINER_TABHEIGHT);

	// Draw Background image if no tab is selected
	if (m_iSelectedTab == -1)
	{
		pSurface->DrawSetColor(255, 255, 255, 255);
		pSurface->DrawSetTexture(g_pResourceManager->GetTexture(RM_TEXTURE_BACKGROUND));
		pSurface->DrawTexturedRect(x, y + TABCONTAINER_TABHEIGHT, x + m_iWidth, y + m_iHeight);
	}

	IControl::Draw(pSurface);
}
