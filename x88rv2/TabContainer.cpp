#include "TabContainer.h"

CTabContainer::CTabContainer() : IControl(0, TABCONTAINER_TABHEIGHT, 0, 0)
{
	m_iFont = g_pResourceManager->GetFont(RM_FONT_SUBHEADER);
}

CTabContainer::~CTabContainer()
{
}

void CTabContainer::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	if(pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if(pEvent->buttons & EVENT_BTN_LMOUSE &&
			pEvent->buttonProperties & EVENT_BTN_LMOUSE)
		{
			int x = 0, y = 0;
			this->GetAbsolutePosition(&x, &y);
	
			m_iWidth = m_pParent->Width();
			m_iHeight = m_pParent->Height();

			int iCountTabs = m_pChildren.size();
			int iSizePerTab = m_iWidth / iCountTabs;

			int iCurX;
			int iCurY = y - m_iY;
			int iCurWidth = iSizePerTab;
			int iCurHeight = TABCONTAINER_TABHEIGHT;

			CGui* pGui = CGui::Instance();
			CTabPage* pTabPage;
			for (int i = 0; i < iCountTabs; i++)
			{
				pTabPage = (CTabPage*)m_pChildren[i];

				iCurX = x + (iSizePerTab * i);
				if(pGui->IsMouseInRect(iCurX, iCurY, iCurWidth, iCurHeight))
				{
					this->SelectTab(pTabPage);
					break;
				}
			}
		}
	}

	IControl::ProcessEvent(pEvent);
}

void CTabContainer::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);
	y -= m_iY;

	m_iWidth = m_pParent->Width();
	m_iHeight = m_pParent->Height();

	pSurface->DrawSetColor(255, 80, 80, 80);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + TABCONTAINER_TABHEIGHT);

	pSurface->DrawSetTextFont(m_iFont);

	CTabPage* pTabPage;
	int iCountTabs = m_pChildren.size();
	int iSizePerTab = m_iWidth / iCountTabs;
	for(int i = 0; i < iCountTabs; i++)
	{
		pTabPage = (CTabPage*)m_pChildren[i];
		
		// Draw title
		int width, height;
		pSurface->GetTextSize(m_iFont, pTabPage->TitleW(), width, height);
		pSurface->DrawSetTextPos(((x + iSizePerTab * i) + iSizePerTab / 2) - (width / 2), (y + TABCONTAINER_TABHEIGHT / 2) - (height / 2));

		if (pTabPage->IsEnabled())
			pSurface->DrawSetTextColor(255, 255, 0, 0);
		else
			pSurface->DrawSetTextColor(255, 255, 255, 255);

		pSurface->DrawPrintText(pTabPage->TitleW(), pTabPage->TitleLength());
	}

	IControl::Draw(pSurface);
}

void CTabContainer::SelectTab(CTabPage* p)
{
	// Disable all tabs that aren't passed as argument
	CTabPage* pTabPage;
	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		pTabPage = (CTabPage*)*it;
		if(pTabPage != p)
		{
			pTabPage->IsEnabled(false);
		}
	}

	p->IsEnabled(true);
}
