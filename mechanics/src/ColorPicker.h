#ifndef __COLORPICKER_H__
#define __COLORPICKER_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControlTooltip.h"
#include "ColorPickerPopup.h"

class CColorPicker : public IControlTooltip
{
public:
	CColorPicker(int x = 0, int y = 0, int w = 30, int h = 30);
	~CColorPicker();

	void SetValue(Color clrValue);
	Color GetValue() { return m_clrValue; }

	virtual void OnClicked() override;
	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	void SetEventHandler(std::function<void(Color)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	int m_iColorFadeTexture;
	Color m_clrValue;

	bool m_bPopupInitialized;
	CColorPickerPopup* m_pPopup;

	std::function<void(Color)> m_pEventHandler;
};

#endif // __COLORPICKER_H__