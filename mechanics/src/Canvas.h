#ifndef __CANVAS_H__
#define __CANVAS_H__

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"

class CCanvas : public IControl
{
public:
	CCanvas(int x = 0, int y = 0, int w = 0, int h = 0, Color backgroundColor = Color(0, 0, 0, 0));
	~CCanvas();

	virtual void Draw(ISurface* pSurface) override;
	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;
private:
	Color m_clrBackground;
};

#endif // __CANVAS_H__