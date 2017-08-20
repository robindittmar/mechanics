#include "TabPage.h"

CTabPage::CTabPage(const char* pTitle)
{
	m_bIsEnabled = false;

	m_pTitle = NULL;
	m_pTitleW = NULL;
	this->Title(pTitle);
}

CTabPage::~CTabPage()
{
	if (m_pTitleW)
		delete[] m_pTitleW;

	if (m_pTitle)
		delete[] m_pTitle;
}

void CTabPage::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	IControl::ProcessEvent(pEvent);
}

void CTabPage::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	IControl::Draw(pSurface);
}

void CTabPage::Title(const char* pTitle)
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