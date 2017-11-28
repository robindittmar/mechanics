#include "Label.h"

CLabel::CLabel(int x, int y, int w, int h, const char* pText, int font, int orientation, Color color)
	: IControl(x, y, w, h), m_pContentText(nullptr), m_pContentTextW(nullptr),
	m_iOrientation(orientation), m_cTextColor(color)
{
	m_iFont = g_pResourceManager->GetFont(font);
	this->SetContentText(pText);
}

CLabel::~CLabel()
{
	this->FreeMemory();
}

void CLabel::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	if (!m_pContentTextW)
		return;

	int x = 0, y = 0, w, h, frameX, frameY;
	this->GetAbsolutePosition(&x, &y);
	pSurface->GetTextSize(m_iFont, m_pContentTextW, w, h);

	pSurface->DrawSetTextFont(m_iFont);

	if (m_bIsEnabled)
		pSurface->DrawSetTextColor(m_cTextColor);
	else
		pSurface->DrawSetTextColor(g_clrControlDisabled);
	
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

bool CLabel::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}

void CLabel::GetTextSize(ISurface* pSurface, int& width, int& height)
{
	pSurface->GetTextSize(m_iFont, m_pContentTextW, width, height);
}

void CLabel::SetContentText(const char* pText)
{
	this->FreeMemory();

	if (!pText)
		return;

	m_iContentTextLen = strlen(pText) + 1;

	m_pContentText = new char[m_iContentTextLen];
	memcpy(m_pContentText, pText, m_iContentTextLen);

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	mbstowcs(m_pContentTextW, m_pContentText, m_iContentTextLen);
}

void CLabel::SetContentTextW(const wchar_t* pTextW)
{
	this->FreeMemory();

	if (!pTextW)
		return;

	m_iContentTextLen = wcslen(pTextW) + 1;

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	memcpy(m_pContentTextW, pTextW, m_iContentTextLen * sizeof(wchar_t));

	m_pContentText = new char[m_iContentTextLen];
	wcstombs(m_pContentText, m_pContentTextW, m_iContentTextLen);
}

void CLabel::FreeMemory()
{
	if (m_pContentTextW)
	{
		delete[] m_pContentTextW;
		m_pContentTextW = nullptr;
	}

	if (m_pContentText)
	{
		delete[] m_pContentText;
		m_pContentText = nullptr;
	}
}