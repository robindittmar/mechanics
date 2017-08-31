#ifndef __SLIDER_H__
#define __SLIDER_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

class CSlider : public IControl
{
public:
	CSlider(int x = 0, int y = 0, int w = 100, int h = 20, float fMin = 0.0, float fMax = 1.0f, float fValue = 0.0f);
	~CSlider();

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseDown(int mx, int my);

	virtual void Draw(ISurface* pSurface);

	void SetValue(float fValue) { m_fValue = fValue; }
	float GetValue() { return m_fValue; }

	void SetEventHandler(std::function<void(float)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	void SetValueToCursorPos(int mx, int my);

	float m_fMinValue, m_fMaxValue;
	float m_fValue;

	std::function<void(float)> m_pEventHandler;
};

#endif // __SLIDER_H__