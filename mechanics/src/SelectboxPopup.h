#ifndef __SELECTBOXPOPUP_H__
#define __SELECTBOXPOPUP_H__

// Std lib
#include <cstring>

// Custom
#include "Gui.h"
#include "IControlPopup.h"
#include "Label.h"

class CSelectbox;

const Color g_clrSelectboxPopupSelection(255, 255, 128, 0);

class CSelectboxPopup : public IControlPopup
{
public:
	CSelectboxPopup(int x = 0, int y = 0, CSelectbox* pSelectbox = NULL, CWindow* pParentWindow = NULL);
	~CSelectboxPopup();

	void SetSelectbox(CSelectbox* pSelectbox) { m_pSelectbox = pSelectbox; }
	CSelectbox* GetSelectbox() const { return m_pSelectbox; }

	virtual void OnMouseMove(int mx, int my);
	virtual void OnMouseUp(int mx, int my);

	virtual void GetAbsolutePosition(int* pX, int* pY);

	virtual void ProcessEvent(CInputEvent* pEvent) override;
	virtual void Draw(ISurface* pSurface) override;

	void ClearOptions();
	void AddOption(int curIdx, const char* text, int len);
private:
	CSelectbox* m_pSelectbox;

	int m_iLenLargestOptionString;
	int m_iLargestOptionStringIndex;

	int m_iSelectedOption;

	int m_iRowRenderWidth;
	int m_iRowRenderHeight;

	std::vector<CLabel*> m_vOptionLabels;
};

#endif // __SELECTBOXPOPUP_H__