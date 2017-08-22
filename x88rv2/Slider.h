#ifndef __SLIDER_H__
#define __SLIDER_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControl.h"

class CSlider : public IControl
{
public:
	CSlider();
	~CSlider();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);
private:
};

#endif // __SLIDER_H__