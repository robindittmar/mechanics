#ifndef __ICONTROLCLICKABLE_H__
#define __ICONTROLCLICKABLE_H__

// Std Lib

// Source SDK

// Custom
#include "IControl.h"

class IControlClickable : public IControl
{
public:
	IControlClickable(int x = 0, int y = 0, int w = 0, int h = 0);
	~IControlClickable();

	// Internal control events (may easily be overriden
	// will get called by IControl::ProcessEvent, when
	// appropiate)
	virtual void OnMouseMove(int mx, int my) {}
	virtual void OnMouseDown(int mx, int my) {}
	virtual void OnMouseUp(int mx, int my) {}
	virtual void OnClicked() {}

	virtual void ProcessEvent(CInputEvent* pEvent) override;
protected:
	bool m_bHitcheckForMouseMove;
	bool m_bHitcheckForMouseUp;

	bool m_bCanHaveFocus;

	bool m_bMouseOver;
	bool m_bMouseDown;
};

#endif // __ICONTROLCLICKABLE_H__