#include "Window.h"

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle) : IControl(x, y, w, h)
{
	m_bIsVisible = false;

	m_bIsDragging = false;

	m_pTitle = NULL;
	m_pTitleW = NULL;
	this->Title(pTitle);
}

CWindow::~CWindow()
{
	if (m_pTitleW)
		delete[] m_pTitleW;

	if (m_pTitle)
		delete[] m_pTitle;
}

void CWindow::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	CGui* pGui = CGui::Instance();

	if(pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if(pEvent->buttons & EVENT_BTN_LMOUSE)
		{
			if(pEvent->buttonProperties & EVENT_BTN_LMOUSE) // Mouse down
			{
				if (pGui->IsMouseInRect(m_iX, m_iY - TITLEBAR_HEIGHT, m_iWidth, TITLEBAR_HEIGHT))
				{
					m_iDragOffsetX = pEvent->mousex - m_iX;
					m_iDragOffsetY = pEvent->mousey - m_iY;
					m_bIsDragging = true;
				}
			}
			else // Mouse up
			{
				m_bIsDragging = false;
			}
		}

		if(m_bIsDragging)
		{
			m_iX = pEvent->mousex - m_iDragOffsetX;
			m_iY = pEvent->mousey - m_iDragOffsetY;

			// Prevent clipping X
			if (m_iX < 0)
				m_iX = 0;
			else if (m_iX + m_iWidth > pGui->ScreenWidth())
				m_iX = pGui->ScreenWidth() - m_iWidth;

			// Prevent clipping Y
			if (m_iY - TITLEBAR_HEIGHT < 0)
				m_iY = TITLEBAR_HEIGHT;
			else if (m_iY + m_iHeight > pGui->ScreenHeight())
				m_iY = pGui->ScreenHeight() - m_iHeight;
		}
	}

	IControl::ProcessEvent(pEvent);
}

void CWindow::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible || !m_bIsEnabled)
		return;

	// Draw titlebar
	pSurface->DrawSetColor(255, 150, 150, 150);
	pSurface->DrawFilledRect(m_iX, m_iY - TITLEBAR_HEIGHT, m_iX + m_iWidth, m_iY + TITLEBAR_HEIGHT);

	// TODO
	static unsigned long iFont = NULL;
	if (iFont == NULL)
	{
		iFont = pSurface->SCreateFont();
		pSurface->SetFontGlyphSet(iFont, "Arial", 20, 255, 0, 0, 0x200);
	}
	
	// Draw title
	int width, height;
	pSurface->GetTextSize(iFont, m_pTitleW, width, height);
	pSurface->DrawSetTextFont(iFont);
	pSurface->DrawSetTextPos((m_iX + m_iWidth / 2) - (width / 2), (m_iY - TITLEBAR_HEIGHT / 2) - (height / 2));
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawPrintText(m_pTitleW, m_iLenTitle);

	// Draw client region
	pSurface->DrawSetColor(150, 230, 230, 230);
	pSurface->DrawFilledRect(m_iX, m_iY, m_iX + m_iWidth, m_iY + m_iHeight);

	IControl::Draw(pSurface);
}

void CWindow::Title(const char* pTitle)
{
	if (m_pTitle)
		delete[] m_pTitle;

	if (m_pTitleW)
		delete[] m_pTitleW;

	m_iLenTitle = strlen(pTitle) + 1;
	m_pTitle = new char[m_iLenTitle];
	memcpy(m_pTitle, pTitle, m_iLenTitle);

	m_pTitleW = new wchar_t[m_iLenTitle];
	mbstowcs(m_pTitleW, m_pTitle, m_iLenTitle);
}