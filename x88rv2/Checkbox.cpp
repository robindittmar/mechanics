#include "Checkbox.h"

CCheckbox::CCheckbox(int x, int y, int w, int h, const char* pText, bool isChecked) : IControl(x, y, w, h)
{
	m_bIsClicked = false;
	m_bIsChecked = isChecked;

	m_pEventHandler = NULL;

	m_pContentText = NULL;
	m_pContentTextW = NULL;
	this->ContentText(pText);
}

CCheckbox::~CCheckbox()
{
	if (m_pContentTextW)
		delete[] m_pContentTextW;

	if (m_pContentText)
		delete[] m_pContentText;
}

void CCheckbox::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if (pEvent->buttons & EVENT_BTN_LMOUSE)
		{
			if(!m_bIsClicked)
			{
				int x = 0;
				int y = 0;

				this->GetAbsolutePosition(&x, &y);

				// Over checkbox
				if (CGui::Instance()->IsMouseInRect(x, y, m_iWidth, m_iHeight))
				{
					// Mouse down
					if (pEvent->buttonProperties & EVENT_BTN_LMOUSE)
					{
						m_bIsClicked = true;
					}
				}
			}
			// Not clicked & mouse up
			else if (!(pEvent->buttonProperties & EVENT_BTN_LMOUSE))
			{
				m_bIsClicked = false;
				m_bIsChecked = !m_bIsChecked;

				if (m_pEventHandler)
					m_pEventHandler(m_bIsChecked);
			}
		}
	}
}

void CCheckbox::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0;
	int y = 0;

	this->GetAbsolutePosition(&x, &y);

	// TODO
	static unsigned long iFont = NULL;
	if (iFont == NULL)
	{
		iFont = pSurface->SCreateFont();
		pSurface->SetFontGlyphSet(iFont, "Arial", 16, 255, 0, 0, 0x200);
	}

	int width, height;
	pSurface->GetTextSize(iFont, m_pContentTextW, width, height);

	pSurface->DrawSetColor(255, 0, 0, 0);
	// Draw box that holds the checkmark
	pSurface->DrawOutlinedRect(x + CHECKBOX_BOXPADDING, y + CHECKBOX_BOXPADDING, x + m_iHeight - CHECKBOX_BOXPADDING, y + m_iHeight - CHECKBOX_BOXPADDING);

	// Draw checkmark if checked
	if(m_bIsChecked)
	{
		pSurface->DrawFilledRect(
			x + CHECKBOX_BOXPADDING + CHECKBOX_FILLERPADDING,
			y + CHECKBOX_BOXPADDING + CHECKBOX_FILLERPADDING,
			x + m_iHeight - CHECKBOX_BOXPADDING - CHECKBOX_FILLERPADDING,
			y + m_iHeight - CHECKBOX_BOXPADDING - CHECKBOX_FILLERPADDING
		);
	}

	pSurface->DrawSetTextFont(iFont);
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawSetTextPos(x + (CHECKBOX_BOXPADDING * 2) + CHECKBOX_BOXSIZE, (y + m_iHeight / 2) - (height / 2));
	pSurface->DrawPrintText(m_pContentTextW, m_iContentTextLen);
}

void CCheckbox::ContentText(const char* pText)
{
	if (m_pContentText)
		delete[] m_pContentText;

	if (m_pContentTextW)
		delete[] m_pContentTextW;

	m_iContentTextLen = strlen(pText) + 1;
	m_pContentText = new char[m_iContentTextLen];
	memcpy(m_pContentText, pText, m_iContentTextLen);

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	mbstowcs(m_pContentTextW, m_pContentText, m_iContentTextLen);
}