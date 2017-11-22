#include "NumericUpDown.h"

CNumericUpDown::CNumericUpDown(int x, int y, int w, int h, const char* label)
	: CTextbox(x, y, w, h, label, 32), m_iValue(0)
{
	int iBtnHeight = h / 2 - 1;
	m_pBtnUp = new CButton(w - NUMERICUPDOWN_BUTTON_WIDTH, 1, NUMERICUPDOWN_BUTTON_WIDTH - 1, iBtnHeight, "^");
	m_pBtnDown = new CButton(w - NUMERICUPDOWN_BUTTON_WIDTH, iBtnHeight + 1, NUMERICUPDOWN_BUTTON_WIDTH - 1, iBtnHeight, "v");

	m_pBtnUp->SetEventHandler(std::bind(&CNumericUpDown::IncrementValue, this));
	m_pBtnDown->SetEventHandler(std::bind(&CNumericUpDown::DecrementValue, this));

	this->AddChild(m_pBtnUp);
	this->AddChild(m_pBtnDown);

	this->PrintValue();
}

CNumericUpDown::~CNumericUpDown()
{
}

void CNumericUpDown::OnTextInput(char c)
{
	if (c == TEXTINPUT_BACKSPACE ||
		c >= '0' && c <= '9')
	{
		CTextbox::OnTextInput(c);
		SetValue(atoi(this->GetText()));
	}
}

void CNumericUpDown::SetValue(int iValue)
{
	m_iValue = iValue;
	PrintValue();

	if (m_pEventHandler)
		m_pEventHandler(m_iValue);
}

void CNumericUpDown::PrintValue()
{
	char pBuffer[16];
	sprintf(pBuffer, "%d", m_iValue);
	m_pContentLabel->SetContentText(pBuffer);
}