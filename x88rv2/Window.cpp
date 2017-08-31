#include "Window.h"

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle) : IControl(x, y, w, h)
{
	m_bIsVisible = false;

	m_bIsDragging = false;

	m_pTitle = NULL;
	m_pTitleW = NULL;
	this->Title(pTitle);

	m_iFont = g_pResourceManager->GetFont(RM_FONT_HEADER);
}

CWindow::~CWindow()
{
	if (m_pTitleW)
		delete[] m_pTitleW;

	if (m_pTitle)
		delete[] m_pTitle;
}

void CWindow::OnMouseMove(int mx, int my)
{
}

void CWindow::OnMouseDown()
{
}

void CWindow::OnMouseUp()
{
}

void CWindow::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	CGui* pGui = CGui::Instance();

	// TODO: I'm struggling to move these into the above events
	//       => OnMouseMove, OnMouseDown & OnMouseUp, since
	//		 GetAbsolutePosition won't return the Titlebar.
	//		 x, y, w and height define the ClientArea of a window
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
	
	// Draw title
	int width, height;
	pSurface->GetTextSize(m_iFont, m_pTitleW, width, height);
	pSurface->DrawSetTextFont(m_iFont);
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