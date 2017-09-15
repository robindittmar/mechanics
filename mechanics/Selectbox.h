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

class CSelectbox : public IControl
{
public:
	CSelectbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "sbx", int selection = 0);
	~CSelectbox();

	virtual void OnClicked();
	virtual void Draw(ISurface* pSurface);

	void AddOption(int id, const char* text);
	int GetCountOptions() { return m_iCountOptions; }

	void SetSelection(int iSelection);
	int GetSelection() { return m_iSelection; }

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