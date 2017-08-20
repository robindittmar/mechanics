#ifndef __TABPAGE_H__
#define __TABPAGE_H__

// Std lib


// Custom
#include "Gui.h"
#include "IControl.h"

class CTabPage : public IControl
{
public:
	CTabPage(const char* pTitle = "tbpg");
	~CTabPage();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	int TitleLength() { return m_iLenTitle; }

	void Title(const char* pTitle);
	const char* Title() { return (const char*)m_pTitle; }
	const wchar_t* TitleW() { return (const wchar_t*)m_pTitleW; }
private:
	int m_iLenTitle;
	char* m_pTitle;
	wchar_t* m_pTitleW;
};

#endif // __TABPAGE_H__