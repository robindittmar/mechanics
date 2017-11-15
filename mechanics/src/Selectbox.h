#ifndef __SELECTBOX_H__
#define __SELECTBOX_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControl.h"
#include "Label.h"
#include "SelectboxPopup.h"
#include "SelectboxItem.h"

#define SELECTBOX_PADDING	4

const Color g_clrSelectboxText(255, 255, 255, 255);
const Color g_clrSelectboxBackground(255, 50, 50, 50);
const Color g_clrSelectboxBackgroundOutline(255, 80, 80, 80);
const Color g_clrSelectboxBackgroundOutlineHover(255, 255, 128, 0);

class CSelectbox : public IControl
{
public:
	CSelectbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "sbx", int selection = 0);
	~CSelectbox();

	virtual void OnClicked();
	virtual void Draw(ISurface* pSurface);

	virtual void SetEnabled(bool bIsEnabled);

	void ClearOptions();
	void AddOption(int id, const char* text);
	int GetCountOptions() { return m_iCountOptions; }

	// OptionId & Control to enable/disable
	void EnableControlOnSelected(int id, IControl* p);

	void SetSelection(int iSelection);
	int GetSelection() { return m_iSelection; }

	void SetValue(int iValue);
	int GetValue() { return m_vOptions[m_iSelection]->GetId(); }

	const char* GetSelectedText() { return m_vOptions[m_iSelection]->GetContentText(); }

	void SetEventHandler(std::function<void(int)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	bool m_bPopupInitialized;
	CSelectboxPopup* m_pPopup;

	CLabel* m_pLabel;
	CLabel* m_pSelectionLabel;

	int m_iCountOptions;
	int m_iSelection;
	std::vector<CSelectboxItem*> m_vOptions;

	std::function<void(int)> m_pEventHandler;
};

#endif // __SELECTBOX_H__