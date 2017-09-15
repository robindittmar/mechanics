#include "Selectbox.h"
#include "Window.h"

CSelectbox::CSelectbox(int x, int y, int w, int h, const char* label, int selection) : IControl(x, y, w, h)
{
	m_iCountOptions = 0;
	m_iSelection = selection;

	m_bPopupInitialized = false;
	m_pPopup = new CSelectboxPopup(0, h - 1);

	m_pLabel = new CLabel(0, -20, w, 20, label, RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pSelectionLabel = new CLabel(SELECTBOX_PADDING, 0, w, h);

	this->AddChild(m_pLabel);
	this->AddChild(m_pSelectionLabel);
}

CSelectbox::~CSelectbox()
{
	if (m_pPopup)
		delete m_pPopup;

	CSelectboxItem* pCurrent;
	for(std::vector<CSelectboxItem*>::iterator it = m_vOptions.begin(); it != m_vOptions.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete pCurrent;
	}
}

/*void CSelectbox::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if (pEvent->buttons & EVENT_BTN_LMOUSE)
		{
			int x = 0, y = 0;
			this->GetAbsolutePosition(&x, &y);

			CGui* pGui = CGui::Instance();

			// Mouse down (somewhere)
			if (pEvent->buttonProperties & EVENT_BTN_LMOUSE)
			{
				// If we are hit, toggle expansion
				if (pGui->IsMouseInRect(x, y, m_iWidth, m_iHeight))
				{
					m_bExpanded = !m_bExpanded;
				}
				// Collapse if not hit
				else 
				{
					// Check if we got a new selection
					if(m_bExpanded)
					{
						int iCurY = y + m_iHeight + SELECTBOX_PADDING;
						for(int i = 0; i < m_iCountOptions; i++)
						{
							if(pGui->IsMouseInRect(x, iCurY, m_iRowRenderWidth, m_iRowRenderHeight))
							{
								m_iSelection = i;

								if (m_pEventHandler)
									m_pEventHandler(m_vOptions[m_iSelection]->GetId());

								break;
							}

							iCurY += m_iRowRenderHeight + SELECTBOX_PADDING;
						}
					}

					m_bExpanded = false;
				}
			}
		}
	}
}*/

void CSelectbox::OnClicked()
{
	CWindow* pWindow = (CWindow*)this->GetParentWindow();

	if (!m_bPopupInitialized)
	{
		m_pPopup->SetSelectbox(this);
		m_pPopup->SetParentWindow(pWindow);
	}

	pWindow->SetPopup(m_pPopup);
}

void CSelectbox::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	// Draw box that holds the current selection
	pSurface->DrawSetColor(255, 50, 50, 50);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	// Draw labels
	IControl::Draw(pSurface);
}

void CSelectbox::AddOption(int id, const char* text)
{
	CSelectboxItem* pOption = new CSelectboxItem(id, text);
	m_vOptions.push_back(pOption);
	m_pPopup->AddOption(m_iCountOptions, text, pOption->GetContentTextLength());

	m_iCountOptions++;
}

void CSelectbox::SetSelection(int iSelection)
{
	m_iSelection = iSelection;

	if (m_iSelection >= 0 && m_iSelection < m_iCountOptions)
		m_pSelectionLabel->SetContentText(m_vOptions[m_iSelection]->GetContentText());

	if (m_pEventHandler)
		m_pEventHandler(m_vOptions[m_iSelection]->GetId());
}
