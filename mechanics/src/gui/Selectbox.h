#ifndef __SELECTBOX_H__
#define __SELECTBOX_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControlTooltip.h"
#include "Label.h"
#include "SelectboxPopup.h"
#include "SelectboxItem.h"

#define SELECTBOX_PADDING	4

const Color g_clrSelectboxText(255, 255, 255, 255);
const Color g_clrSelectboxBackground(255, 50, 50, 50);
const Color g_clrSelectboxBackgroundOutline(255, 80, 80, 80);
const Color g_clrSelectboxBackgroundOutlineHover(255, 255, 128, 0);

class CSelectbox : public IControlTooltip
{
public:
	CSelectbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "sbx");
	virtual ~CSelectbox();

	virtual void OnClicked() override;
	virtual void Draw(ISurface* pSurface) override;

	virtual bool ShouldDependentOnesBeEnabled(void* pParam) override;

	virtual void SetEnabled(bool bIsEnabled) override;

	void ClearOptions();
	void AddOption(int id, const char* text);
	void AddOptionW(int id, const wchar_t* text);
	int GetCountOptions() { return m_iCountOptions; }

	void ReplaceIdOfOption(const char* option, int newId);

	void SetSelection(int iSelection);
	int GetSelection() { return m_iSelection; }

	void SetValue(int iValue);
	int GetValue() { return m_vOptions[m_iSelection]->GetId(); }

	void SetSelectionById(int iValue);

	const char* GetSelectedText() const { return m_vOptions[m_iSelection]->GetContentText(); }
	const wchar_t* GetSelectedTextW() const { return m_vOptions[m_iSelection]->GetContentTextW(); }

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