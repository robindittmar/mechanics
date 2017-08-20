#include "Button.h"
#include "Console.h"

CButton::CButton(int x, int y, int w, int h, const char* pText) : IControl(x, y, w, h)
{
	m_bIsPressed = false;

	m_pButtonDownHandler = NULL;
	m_pButtonUpHandler = NULL;

	m_pContentText = NULL;
	m_pContentTextW = NULL;
	this->ContentText(pText);
}

CButton::~CButton()
{
	if (m_pContentTextW)
		delete[] m_pContentTextW;

	if (m_pContentText)
		delete[] m_pContentText;
}

void CButton::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	if(pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if(pEvent->buttons & EVENT_BTN_LMOUSE)
		{
			if(!m_bIsPressed)
			{
				int x = 0;
				int y = 0;

				this->GetAbsolutePosition(&x, &y);

				// Over button
				if (CGui::Instance()->IsMouseInRect(x, y, m_iWidth, m_iHeight))
				{
					// Mouse down
					if(pEvent->buttonProperties & EVENT_BTN_LMOUSE)
					{
						m_bIsPressed = true;

						if (m_pButtonDownHandler)
							m_pButtonDownHandler(this);
					}
				}
			}
			else if(!(pEvent->buttonProperties & EVENT_BTN_LMOUSE))
			{
				m_bIsPressed = false;

				if (m_pButtonUpHandler)
					m_pButtonUpHandler(this);
			}
		}
	}
}

void CButton::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0;
	int y = 0;

	this->GetAbsolutePosition(&x, &y);

	if (m_bIsPressed)
		pSurface->DrawSetColor(255, 0, 255, 0);
	else
		pSurface->DrawSetColor(255, 255, 0, 0);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	// TODO
	static unsigned long iFont = NULL;
	if (iFont == NULL)
	{
		iFont = pSurface->SCreateFont();
		pSurface->SetFontGlyphSet(iFont, "Arial", 16, 255, 0, 0, 0x200);
	}

	int width, height;
	pSurface->GetTextSize(iFont, m_pContentTextW, width, height);

	pSurface->DrawSetTextFont(iFont);
	pSurface->DrawSetTextPos((x + m_iWidth / 2) - (width / 2), (y + m_iHeight / 2) - (height / 2));
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawPrintText(m_pContentTextW, m_iContentTextLen);
}

void CButton::ContentText(const char* pText)
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