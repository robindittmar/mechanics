#ifndef __ICONTROLPOPUP_H__
#define __ICONTROLPOPUP_H__

// Std Lib

// Source SDK

// Custom
#include "IControlClickable.h"
#include "Window.h"

class IControlPopup : public IControlClickable
{
public:
	IControlPopup(int x = 0, int y = 0, int w = 0, int h = 0, CWindow* pParentWindow = nullptr);
	virtual ~IControlPopup();

	void SetParentWindow(CWindow* pParentWindow) { m_pParentWindow = pParentWindow; }
	CWindow* GetParentWindow() const { return m_pParentWindow; }

	virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;
protected:
	CWindow* m_pParentWindow;
};

#endif // __ICONTROLPOPUP_H__