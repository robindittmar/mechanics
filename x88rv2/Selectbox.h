#ifndef __SELECTBOX_H__
#define __SELECTBOX_H__

// Std lib
#include <functional>

// Custom
#include "Gui.h"
#include "IControl.h"

#define SELECTBOX_PADDING	4

class CSelectbox : public IControl
{
public:
	CSelectbox(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "sbx", int selection = 0);
	~CSelectbox();

	virtual void ProcessEvent(CInputEvent* pEvent);
	virtual void Draw(ISurface* pSurface);

	void AddOption(int id, const char* text);

	void SetSelection(int iSelection) { m_iSelection = iSelection; }
	int GetSelection() { return m_iSelection; }

	void SetEventHandler(std::function<void(int)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	bool m_bExpanded;

	int m_iLenLargestOptionString;
	int m_iLargestOptionStringIndex;

	int m_iRowRenderWidth;
	int m_iRowRenderHeight;

	int m_iCountOptions;
	int m_iSelection;
	std::vector<int> m_vOptionIds;
	std::vector<wchar_t*> m_vOptions;
	std::vector<int> m_vOptionsLength;

	std::function<void(int)> m_pEventHandler;

	int m_iContentTextLen;
	char* m_pContentText;
	wchar_t* m_pContentTextW;
};

#endif // __SELECTBOX_H__