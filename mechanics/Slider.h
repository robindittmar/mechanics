#ifndef __SLIDER_H__
#define __SLIDER_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

#define SLIDER_ORIENTATION_HORIZONTAL	0
#define SLIDER_ORIENTATION_VERTICAL		1

class CSlider : public IControl
{
public:
	CSlider(
		int x = 0,
		int y = 0,
		int w = 100,
		int h = 20,
		float fDisplayValue = 0.0f,
		int iOrientation = SLIDER_ORIENTATION_HORIZONTAL,
		bool bReverse = false,
		float fMin = 0.0,
		float fMax = 1.0f
	);
	~CSlider();

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseDown(int mx, int my);

	virtual void Draw(ISurface* pSurface);

	// SetValue is used for technical stuff mostly, Label won't be updated
	void SetValue(float fValue);
	float GetValue() { return m_fValue; }

	// SetDisplayValue is mostly for displaying purposes, Label will be updated
	void SetDisplayValue(float fDisplayValue);
	float GetDisplayValue() { return m_fDisplayValue; }

	void SetEventHandler(std::function<void(float)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	void SetValueToCursorPos(int mx, int my);
	void SetLabelText();

	CLabel* m_pLabel;

	int m_iOrientation;
	bool m_bReverse;

	float m_fValue, m_fDisplayValue;
	float m_fMinValue, m_fMaxValue, m_fValueSpan;

	std::function<void(float)> m_pEventHandler;
};

#endif // __SLIDER_H__