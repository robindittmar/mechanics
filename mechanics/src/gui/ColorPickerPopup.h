#ifndef __COLORPICKERPOPUP_H__
#define __COLORPICKERPOPUP_H__

// Std lib
#include <cstring>
#include <cfloat>

// Custom
#include "Gui.h"
#include "IControlPopup.h"
#include "Label.h"
#include "Slider.h"

#define COLORPICKER_SVMAP_X		30
#define COLORPICKER_SVMAP_Y		4
#define COLORPICKER_SVMAP_W		200
#define COLORPICKER_SVMAP_H		200

class CColorPicker;

const Color g_clrColorPickerPopupContainer(255, 80, 80, 80);

class CColorPickerPopup : public IControlPopup
{
public:
	CColorPickerPopup(int x = 0, int y = 0, CColorPicker* pColorPicker = nullptr, CWindow* pParentWindow = nullptr);
	virtual ~CColorPickerPopup();

	void SetColorPicker(CColorPicker* pColorPicker) { m_pColorPicker = pColorPicker; }
	CColorPicker* GetColorPicker() const { return m_pColorPicker; }

	void OnOpen();

	void SetAlpha(float fAlpha);
	void SetHue(float fHue);
	void SetSaturationAndValue(float fSaturation, float fValue);
	void UpdateValue();

	void SetSaturationAndValueByCursorPos(int mx, int my);

	virtual void GetAbsolutePosition(int* pX, int* pY) override;

	virtual void OnMouseDown(int mx, int my) override;
	virtual void OnMouseMove(int mx, int my) override;
	virtual void OnMouseUp(int mx, int my) override;

	//virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;
private:
	static int m_iColorFadeTexture;

	int m_iAlpha;
	int m_iHue;
	float m_fSaturation;
	float m_fValue;

	CLabel* m_pAlphaLabel;
	CSlider* m_pAlphaSlider;
	CSlider* m_pHueSlider;

	bool m_bMouseDownInSatValPicker;

	CColorPicker* m_pColorPicker;
};

#endif // __COLORPICKERPOPUP_H__