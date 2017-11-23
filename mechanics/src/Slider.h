#ifndef __SLIDER_H__
#define __SLIDER_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControlTooltip.h"
#include "Label.h"

#define SLIDER_ORIENTATION_HORIZONTAL	0
#define SLIDER_ORIENTATION_VERTICAL		1

#define SLIDER_KNOBSIZE					6

const Color g_clrControl(255, 0, 0, 0);
const Color g_clrKnob(250, 255, 128, 0);

class CSlider : public IControlTooltip
{
public:
	CSlider(
		int x = 0,
		int y = 0,
		int w = 100,
		int h = 20,
		float fStepSize = 0.0f,
		int iOrientation = SLIDER_ORIENTATION_HORIZONTAL,
		bool bReverse = false,
		float fMin = 0.0,
		float fMax = 1.0f,
		const char* pText = nullptr
	);
	virtual ~CSlider();

	virtual void OnMouseMove(int mx, int my) override;
	virtual void OnMouseDown(int mx, int my) override;

	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	virtual void SetEnabled(bool bIsEnabled) override;

	void SetValue(float fValue);
	float GetValue() { return m_fValue; }

	void SetEventHandler(std::function<void(float)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	void SetValueToCursorPos(int mx, int my);
	void SetLabelText();

	CLabel* m_pTextLabel;
	CLabel* m_pLabel;

	int m_iOrientation;
	bool m_bReverse;

	float m_fValue;
	float m_fMinValue, m_fMaxValue, m_fValueSpan, m_fStepSize;

	std::function<void(float)> m_pEventHandler;
};

#endif // __SLIDER_H__