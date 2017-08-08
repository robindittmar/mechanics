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
	virtual void Draw(/*IDirect3DDevice9* pDevice, */int xOff, int yOff);

	void ContentText(const char* pText) {
		if (m_pContentText)
			delete[] m_pContentText;

		int iLen = strlen(pText) + 1;
		m_pContentText = new char[iLen];
		memcpy(m_pContentText, pText, iLen);
	}

	const char* ContentText() {
		return (const char*)m_pContentText;
	}
private:
	char* m_pContentText;
};

#endif // __BUTTON_H__