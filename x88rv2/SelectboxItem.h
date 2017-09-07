#ifndef __SELECTBOXITEM_H__
#define __SELECTBOXITEM_H__

#include <cstring>
#include <cstdlib>

class CSelectboxItem
{
public:
	CSelectboxItem(int id = 0, const char* contentText = NULL);
	~CSelectboxItem();

	void SetId(int id) { m_iId = id; }
	int GetId() { return m_iId; }

	void SetContentText(const char* pContentText);
	const char* GetContentText() { return (const char*)m_pContentText; }

	int GetContentTextLength() { return m_iContentTextLen; }
private:
	int m_iId;
	char* m_pContentText;
	int m_iContentTextLen;
};

#endif // __SELECTBOXITEM_H__