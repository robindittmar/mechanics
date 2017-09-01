#include "Label.h"

CLabel::CLabel(int x, int y, int w, int h, const char* pText, int font, int orientation, Color color) : IControl(x, y, w, h)
{
	m_pContentText = NULL;
	m_pContentTextW = NULL;
	this->SetContentText(pText);

	m_iOrientation = orientation;
	m_cTextColor = color;

	m_iFont = g_pResourceManager->GetFont(font);
}

CLabel::~CLabel()
{
	if (m_pContentTextW)
		delete[] m_pContentTextW;

	if (m_pContentText)
		delete[] m_pContentText;
}

void CLabel::ProcessEvent(CInputEvent* pEvent)
{
}

void CLabel::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	int x = 0, y = 0, w, h, frameX, frameY;
	this->GetAbsolutePosition(&x, &y);
	pSurface->GetTextSize(m_iFont, m_pContentTextW, w, h);

	pSurface->DrawSetTextFont(m_iFont);
	pSurface->DrawSetTextColor(m_cTextColor);
	
	frameY = (y + m_iHeight / 2) - (h / 2);
	switch(m_iOrientation)
	{
	case LABEL_ORIENTATION_LEFT:
		frameX = x;
		break;
	case LABEL_ORIENTATION_CENTER:
		frameX = (x + m_iWidth / 2) - (w / 2);
		break;
	case LABEL_ORIENTATION_RIGHT:
		frameX = x + (m_iWidth - w);
		break;
	default:
		break;
	}

	pSurface->DrawSetTextPos(frameX, frameY);
	pSurface->DrawPrintText(m_pContentTextW, m_iContentTextLen);
}

void CLabel::GetTextSize(ISurface* pSurface, int& width, int& height)
{
	pSurface->GetTextSize(m_iFont, m_pContentTextW, width, height);
}

void CLabel::SetContentText(const char* pText)
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
