#include "Button.h"

CButton::CButton(int x, int y, int w, int h, const char* pText) : IControl(x, y, w, h)
{
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

void CButton::ProcessEvents()
{
	// TODO
}

void CButton::Draw(ISurface* pSurface, int xOff, int yOff)
{
	if (!m_bIsEnabled)
		return;

	int offsetX = m_iX + xOff;
	int offsetY = m_iY + yOff;

	pSurface->DrawSetColor(255, 255, 0, 0);
	pSurface->DrawFilledRect(offsetX, offsetY, offsetX + m_iWidth, offsetY + m_iHeight);

	// TODO
	static unsigned long iFont = NULL;
	if (iFont == NULL)
	{
		iFont = pSurface->SCreateFont();
		pSurface->SetFontGlyphSet(iFont, "Arial", 12, 255, 0, 0, 0x200);
	}
	

	int width, height;
	pSurface->GetTextSize(iFont, m_pContentTextW, width, height);

	pSurface->DrawSetTextFont(iFont);
	pSurface->DrawSetTextPos((offsetX + m_iWidth / 2) - (width / 2), (offsetY + m_iHeight / 2) - (height / 2));
	pSurface->DrawSetTextColor(255, 255, 255, 255);
	pSurface->DrawPrintText(m_pContentTextW, m_iContentTextLen);
}

void CButton::ContentText(const char* pText)
{
	if (m_pContentText)
		delete[] m_pContentText;

	m_iContentTextLen = strlen(pText) + 1;
	m_pContentText = new char[m_iContentTextLen];
	memcpy(m_pContentText, pText, m_iContentTextLen);

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	mbstowcs(m_pContentTextW, m_pContentText, m_iContentTextLen);
}