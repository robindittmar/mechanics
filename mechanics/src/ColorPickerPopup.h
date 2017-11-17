#ifndef __COLORPICKERPOPUP_H__
#define __COLORPICKERPOPUP_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControlPopup.h"
#include "Label.h"
#include "Slider.h"

class CColorPicker;

const Color g_clrColorPickerPopupContainer(255, 80, 80, 80);

class CColorPickerPopup : public IControlPopup
{
public:
	CColorPickerPopup(int x = 0, int y = 0, CColorPicker* pColorPicker = NULL, CWindow* pParentWindow = NULL);
	~CColorPickerPopup();

	void SetColorPicker(CColorPicker* pColorPicker) { m_pColorPicker = pColorPicker; }
	CColorPicker* GetColorPicker() const { return m_pColorPicker; }

	void SetHueValue(float fHue);

	virtual void GetAbsolutePosition(int* pX, int* pY) override;

	//virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;
private:
	int m_iColorFadeTexture;

	bool m_bInitialized;
	CSlider* m_pHueSlider;

	CColorPicker* m_pColorPicker;
};

#endif // __COLORPICKERPOPUP_H__