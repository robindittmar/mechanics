#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "IControl.h"
#include <cstring>

class CButton : public IControl
{
public:
	CButton(int x = 0, int y = 0, int w = 100, int h = 20, const char* pText = "Button");
	~CButton();

	virtual void ProcessEvents();
	virtual void Draw(ISurface* pSurface, int xOff, int yOff);

	void ContentText(const char* pText);
	const char* ContentText() {
		return (const char*)m_pContentText;
	}
private:
	int m_iContentTextLen;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
};

#endif // __BUTTON_H__