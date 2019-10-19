#include "SelectboxItem.h"

CSelectboxItem::CSelectboxItem(int id, const char* contentText)
{
	m_iId = id;
	m_pContentText = nullptr;
	m_pContentTextW = nullptr;

	this->SetContentText(contentText);
}

CSelectboxItem::CSelectboxItem(int id, const wchar_t* contentTextW)
{
	m_iId = id;
	m_pContentText = nullptr;
	m_pContentTextW = nullptr;
	
	this->SetContentTextW(contentTextW);
}

CSelectboxItem::~CSelectboxItem()
{
	if (m_pContentTextW)
		delete[] m_pContentTextW;

	if (m_pContentText)
		delete[] m_pContentText;
}

void CSelectboxItem::SetContentText(const char* pContentText)
{
	this->FreeMemory();

	if (!pContentText)
		return;

	m_iContentTextLen = strlen(pContentText) + 1;

	m_pContentText = new char[m_iContentTextLen];
	memcpy(m_pContentText, pContentText, m_iContentTextLen);

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	mbstowcs(m_pContentTextW, m_pContentText, m_iContentTextLen);
}

void CSelectboxItem::SetContentTextW(const wchar_t* pContentTextW)
{
	this->FreeMemory();

	if (!pContentTextW)
		return;

	m_iContentTextLen = wcslen(pContentTextW) + 1;

	m_pContentTextW = new wchar_t[m_iContentTextLen];
	memcpy(m_pContentTextW, pContentTextW, m_iContentTextLen * sizeof(wchar_t));

	m_pContentText = new char[m_iContentTextLen];
	wcstombs(m_pContentText, m_pContentTextW, m_iContentTextLen);
}

void CSelectboxItem::FreeMemory()
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