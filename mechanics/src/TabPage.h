#ifndef __TABPAGE_H__
#define __TABPAGE_H__

// Custom
#include "Gui.h"
#include "IControl.h"

#define TABCONTAINER_TABHEIGHT	30

class CTabPage : public IControl
{
public:
	CTabPage(const char* pTitle = "tbpg");
	~CTabPage();

	int GetTitleLength() { return m_iTitleLen; }

	void SetTitle(const char* pTitle);
	const char* GetTitle() { return (const char*)m_pTitle; }
private:
	int m_iTitleLen;
	char* m_pTitle;
};

#endif // __TABPAGE_H__