#include "Window.h"

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle)
{
	m_bIsVisible = false;

	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;

	m_pTitle = NULL;
	m_pTitleW = NULL;
	this->Title(pTitle);
}

CWindow::~CWindow()
{
	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;

		if (p)
		{
			delete p;
		}
	}

	if (m_pTitleW)
		delete[] m_pTitleW;

	if (m_pTitle)
		delete[] m_pTitle;
}

bool CWindow::AddChild(IControl* pControl)
{
	m_pChildren.push_back(pControl);
	return true;
}

void CWindow::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	// Draw titlebar
	pSurface->DrawSetColor(255, 100, 100, 100);
	pSurface->DrawFilledRect(m_iX, m_iY - TITLEBAR_HEIGHT, m_iX + m_iWidth, m_iY + TITLEBAR_HEIGHT);

	// TODO
	static unsigned long iFont = NULL;
	if (iFont == NULL)
	{
		iFont = pSurface->SCreateFont();
		pSurface->SetFontGlyphSet(iFont, "Arial", 16, 255, 0, 0, 0x200);
	}
	
	// Draw title
	int width, height;
	pSurface->GetTextSize(iFont, m_pTitleW, width, height);
	pSurface->DrawSetTextFont(iFont);
	pSurface->DrawSetTextPos((m_iX + m_iWidth / 2) - (width / 2), (m_iY - TITLEBAR_HEIGHT / 2) - (height / 2));
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawPrintText(m_pTitleW, m_iLenTitle);

	// Draw client region
	pSurface->DrawSetColor(255, 200, 200, 200);
	pSurface->DrawFilledRect(m_iX, m_iY, m_iX + m_iWidth, m_iY + m_iHeight);

	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(pSurface, m_iX, m_iY);
	}
}

void CWindow::Title(const char* pTitle)
{
	if (m_pTitle)
		delete[] m_pTitle;

	m_iLenTitle = strlen(pTitle) + 1;
	m_pTitle = new char[m_iLenTitle];
	memcpy(m_pTitle, pTitle, m_iLenTitle);

	m_pTitleW = new wchar_t[m_iLenTitle];
	mbstowcs(m_pTitleW, m_pTitle, m_iLenTitle);
}