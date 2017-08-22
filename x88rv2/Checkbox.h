#ifndef __CHECKBOX_H__
#define __CHECKBOX__H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControl.h"

#define CHECKBOX_BOXPADDING		5
#define CHECKBOX_BOXSIZE		30
#define CHECKBOX_FILLERPADDING	3

class CCheckbox : public IControl
{
public:
	CCheckbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* pText = "cbx", bool isChecked = false);
	~CCheckbox();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void IsChecked(bool bIsChecked) { m_bIsChecked = bIsChecked; }
	bool IsChecked() { return m_bIsChecked; }

	void SetEventHandler(std::function<void(bool)> pEventHandler) { m_pEventHandler = pEventHandler; }

	void ContentText(const char* pText);
	const char* ContentText() { return (const char*)m_pContentText; }
private:
	bool m_bIsClicked;
	bool m_bIsChecked;

	std::function<void(bool)> m_pEventHandler;

	unsigned int m_iFont;

	int m_iContentTextLen;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
};

#endif // __CHECKBOX_H__