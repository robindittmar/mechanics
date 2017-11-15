#include "SelectboxItem.h"

CSelectboxItem::CSelectboxItem(int id, const char* contentText)
{
	m_iId = id;
	m_pContentText = NULL;
	
	this->SetContentText(contentText);
}

CSelectboxItem::~CSelectboxItem()
{
	m_vControlsToEnable.clear();

	if (m_pContentText)
		delete[] m_pContentText;
}

void CSelectboxItem::SetContentText(const char* pContentText)
{
	if (m_pContentText)
		delete[] m_pContentText;

	if (pContentText == NULL)
	{
		m_pContentText = NULL;
		return;
	}

	m_iContentTextLen = strlen(pContentText);
	m_pContentText = new char[m_iContentTextLen + 1];
	memcpy(m_pContentText, pContentText, m_iContentTextLen + 1);
}

void CSelectboxItem::AddControlToEnable(IControl* p)
{
	m_vControlsToEnable.push_back(p);
}

void CSelectboxItem::EnableDependentControls()
{
	for (std::vector<IControl*>::iterator it = m_vControlsToEnable.begin(); it != m_vControlsToEnable.end(); it++)
	{
		(*it)->SetEnabled(true);
	}
}

void CSelectboxItem::DisableDependentControls()
{
	for (std::vector<IControl*>::iterator it = m_vControlsToEnable.begin(); it != m_vControlsToEnable.end(); it++)
	{
		(*it)->SetEnabled(false);
	}
}
