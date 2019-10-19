#include "TabPage.h"

// Setting y=TABCONTAINER_TABHEIGHT for correct Child alignment
CTabPage::CTabPage(const char* pTitle) : IControl(0, TABCONTAINER_TABHEIGHT)
{
	m_pTitle = NULL;
	this->SetTitle(pTitle);
}

CTabPage::~CTabPage()
{
	if (m_pTitle)
		delete[] m_pTitle;
}

void CTabPage::SetTitle(const char* pTitle)
{
	if (m_pTitle)
		delete[] m_pTitle;

	m_iTitleLen = strlen(pTitle) + 1;
	m_pTitle = new char[m_iTitleLen];
	memcpy(m_pTitle, pTitle, m_iTitleLen);
}

bool CTabPage::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}