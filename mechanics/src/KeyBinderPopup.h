#ifndef __KEYBINDERPOPUP_H__
#define __KEYBINDERPOPUP_H__

// Std Lib

// Source SDK

// Custom
#include "IControlPopup.h"

class CKeyBinder;

class CKeyBinderPopup : public IControlPopup
{
public:
	CKeyBinderPopup(CKeyBinder* pKeyBinder = nullptr, CWindow* pParentWindow = nullptr);
	virtual ~CKeyBinderPopup();

	void OnOpen();
	void OnClose();

	virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;
private:
	CKeyBinder* m_pKeyBinder;

	CLabel* m_pPressKeyLabel;
};

#endif // __KEYBINDERPOPUP_H__