#ifndef __KEYBINDER_H__
#define __KEYBINDER_H__

// Std Lib
#include <functional>

// Source SDK

// Custom
#include "InputHandler.h"
#include "IControlTooltip.h"
#include "Label.h"
#include "KeyBinderPopup.h"

const Color g_clrKeyBinderText(255, 255, 255, 255);

class CKeyBinder : public IControlTooltip
{
public:
	CKeyBinder(int x = 0, int y = 0, int w = 0, int h = 0, int iEventBtn = -1);
	virtual ~CKeyBinder();

	virtual void OnClicked() override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	virtual void SetEnabled(bool bIsEnabled) override;

	void SetKey(unsigned short nKey);
	unsigned short GetKey() { return m_nKey; }

	void SetEventHandler(std::function<void(unsigned short)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	void GenerateLabelText();

	bool m_bPopupInitialized;
	CKeyBinderPopup* m_pPopup;

	CLabel* m_pLabel;

	int m_iEventButton;
	unsigned short m_nKey;

	std::function<void(unsigned short)> m_pEventHandler;

	static bool m_bKeyTranslationMapInitialized;
	static std::unordered_map<unsigned short, wchar_t*> m_mapKeys;
};

#endif // __KEYBINDER_H__