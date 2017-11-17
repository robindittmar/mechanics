#ifndef __ICONTROLPOPUP_H__
#define __ICONTROLPOPUP_H__

// Std Lib

// Source SDK

// Custom
#include "IControl.h"
#include "Window.h"

class IControlPopup : public IControl
{
public:
	IControlPopup(int x = 0, int y = 0, int w = 0, int h = 0, CWindow* pParentWindow = NULL);
	~IControlPopup();

	void SetParentWindow(CWindow* pParentWindow) { m_pParentWindow = pParentWindow; }
	CWindow* GetParentWindow() const { return m_pParentWindow; }

	virtual void ProcessEvent(CInputEvent* pEvent) override;
protected:
	CWindow* m_pParentWindow;
};

#endif // __ICONTROLPOPUP_H__