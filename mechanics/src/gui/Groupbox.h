#ifndef __GROUPBOX_H__
#define __GROUPBOX_H__

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"
#include "Canvas.h"

#define GROUPBOX_PADDING	4

class CGroupbox : public IControl
{
public:
	CGroupbox(int x = 0, int y = 0, int w = 0, int h = 0, const char* pTitle = "grpbx");
	virtual ~CGroupbox();

	virtual void AddChild(IControl* pControl) override;
	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;
private:
	bool m_bAlreadySetSize;

	CCanvas* m_pCanvas;
	CLabel* m_pLabel;
};

#endif // __GROUPBOX_H__