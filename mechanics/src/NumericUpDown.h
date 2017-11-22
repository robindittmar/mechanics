#ifndef __NUMERICUPDOWN_H__
#define __NUMERICUPDOWN_H__

// Std Lib

// Source SDK

// Custom
#include "Button.h"
#include "Textbox.h"

#define NUMERICUPDOWN_BUTTON_WIDTH	16

class CNumericUpDown : public CTextbox
{
public:
	CNumericUpDown(int x = 0, int y = 0, int w = 100, int h = 20, const char* label = "nmcupdn");
	virtual ~CNumericUpDown();

	virtual void OnTextInput(char c) override;

	void SetValue(int iValue);
	int GetValue() { return m_iValue; }

	void IncrementValue() { m_iValue++; PrintValue(); }
	void DecrementValue() { m_iValue--; PrintValue(); }

	void SetEventHandler(std::function<void(int)> pEventHandler) { m_pEventHandler = pEventHandler; }
private:
	void PrintValue();

	CButton* m_pBtnUp;
	CButton* m_pBtnDown;

	int m_iValue;

	std::function<void(int)> m_pEventHandler;
};

#endif // __NUMERICUPDOWN_H__