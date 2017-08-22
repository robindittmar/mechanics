#include "Selectbox.h"

CSelectbox::CSelectbox(int x, int y, int w, int h, const char* label, int selection) : IControl(x, y, w, h)
{
	m_bExpanded = false;

	m_iLenLargestOptionString = -1;
	m_iLargestOptionStringIndex = -1;

	m_iCountOptions = 0;
	m_iSelection = selection;

	m_iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
}

CSelectbox::~CSelectbox()
{
	wchar_t* pCurrent;
	for(std::vector<wchar_t*>::iterator it = m_vOptions.begin(); it != m_vOptions.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete[] pCurrent;
	}
}

void CSelectbox::ProcessEvent(CInputEvent* pEvent)
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
									m_pEventHandler(m_vOptionIds[m_iSelection]);

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
}

void CSelectbox::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int width, height;
	pSurface->GetTextSize(m_iFont, m_vOptions[m_iSelection], width, height);

	// Draw box that holds the current selection
	pSurface->DrawSetColor(255, 50, 50, 50);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	// Draw current selection
	pSurface->DrawSetTextFont(m_iFont);
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawSetTextPos(x + SELECTBOX_PADDING, (y + m_iHeight / 2) - (height / 2));
	pSurface->DrawPrintText(m_vOptions[m_iSelection], m_vOptionsLength[m_iSelection]);

	// Draw all other options if expanded :)
	if (m_bExpanded)
	{
		// Get size of longest string
		pSurface->GetTextSize(m_iFont, m_vOptions[m_iLargestOptionStringIndex], width, height);

		// Save row width/height for event handling
		m_iRowRenderWidth = width;
		m_iRowRenderHeight = height;

		// Render popup box first
		pSurface->DrawSetColor(255, 50, 50, 50);
		pSurface->DrawFilledRect(x, y + m_iHeight, x + width, y + m_iHeight + ((height + SELECTBOX_PADDING) * m_iCountOptions));

		// Render options
		int iCurY = y + m_iHeight + SELECTBOX_PADDING;
		pSurface->DrawSetTextColor(255, 255, 255, 255);
		for (int i = 0; i < m_iCountOptions; i++)
		{
			pSurface->DrawSetTextPos(x + SELECTBOX_PADDING, iCurY);
			pSurface->DrawPrintText(m_vOptions[i], m_vOptionsLength[i]);

			iCurY += height + SELECTBOX_PADDING;
		}
	}
}

void CSelectbox::AddOption(int id, const char* text)
{
	int iLen = strlen(text);
	wchar_t* pOption = new wchar_t[iLen + 1];
	mbstowcs(pOption, text, iLen);

	m_vOptionIds.push_back(id);
	m_vOptions.push_back(pOption);
	m_vOptionsLength.push_back(iLen);

	if(iLen > m_iLenLargestOptionString)
	{
		m_iLenLargestOptionString = iLen;
		m_iLargestOptionStringIndex = m_iCountOptions;
	}

	m_iCountOptions++;
}