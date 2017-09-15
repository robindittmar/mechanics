#include "SelectboxItem.h"

CSelectboxItem::CSelectboxItem(int id, const char* contentText)
{
	m_iId = id;
	m_pContentText = NULL;
	
	this->SetContentText(contentText);
}

CSelectboxItem::~CSelectboxItem()
{
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
