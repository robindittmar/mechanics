#ifndef __COLORPICKER_H__
#define __COLORPICKER_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"

class CColorPicker : public IControl
{
public:
	CColorPicker(int x = 0, int y = 0, int w = 30, int h = 30, int a = 255, int r = 255, int g = 255, int b = 255);
	~CColorPicker();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);
private:
	int m_iColorFadeTexture;
	int m_iA, m_iR, m_iG, m_iB;
};

#endif // __COLORPICKER_H__