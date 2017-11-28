#ifndef __SELECTBOXITEM_H__
#define __SELECTBOXITEM_H__

// Std Lib
#include <cstring>
#include <cstdlib>

// Source SDK

// Custom
#include "IControl.h"

class CSelectboxItem
{
public:
	CSelectboxItem(int id = 0, const char* contentText = nullptr);
	CSelectboxItem(int id = 0, const wchar_t* contentTextW = nullptr);
	~CSelectboxItem();

	void SetId(int id) { m_iId = id; }
	int GetId() { return m_iId; }

	void SetContentText(const char* pContentText);
	const char* GetContentText() const { return (const char*)m_pContentText; }

	void SetContentTextW(const wchar_t* pContentTextW);
	const wchar_t* GetContentTextW() const { return (const wchar_t*)m_pContentTextW; }

	int GetContentTextLength() { return m_iContentTextLen; }
private:
	void FreeMemory();

	int m_iId;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
	int m_iContentTextLen;
};

#endif // __SELECTBOXITEM_H__