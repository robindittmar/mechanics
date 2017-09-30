#ifndef __COLORPICKERPOPUP_H__
#define __COLORPICKERPOPUP_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

class CWindow;
class CColorPicker;

class CColorPickerPopup : public IControl
{
public:
	CColorPickerPopup(int x = 0, int y = 0, CColorPicker* pColorPicker = NULL, CWindow* pParentWindow = NULL);
	~CColorPickerPopup();

	void SetColorPicker(CColorPicker* pColorPicker) { m_pColorPicker = pColorPicker; }
	void SetParentWindow(CWindow* pParentWindow) { m_pParentWindow = pParentWindow; }

	virtual void OnMouseUp(int mx, int my);

	virtual void GetAbsolutePosition(int* pX, int* pY);

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);
private:
	int m_iColorFadeTexture;

	CColorPicker* m_pColorPicker;
	CWindow* m_pParentWindow;
};

#endif // __COLORPICKERPOPUP_H__