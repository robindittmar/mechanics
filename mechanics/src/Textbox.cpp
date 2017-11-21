#include "Textbox.h"
#include "Window.h"
#include "Application.h"

CTextbox::CTextbox(int x, int y, int w, int h, const char* label, int maxLen)
	: IControlClickable(x, y, w, h), m_iMaxLen(maxLen), m_fLastBlink(0.0f),
	m_bBlink(true)
{
	m_bCanHaveFocus = true;

	m_pLabel = new CLabel(0, -18, w, 20, label, RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pContentLabel = new CLabel(TEXTBOX_PADDING, 0, w, h, "");

	this->AddChild(m_pLabel);
	this->AddChild(m_pContentLabel);
}

CTextbox::~CTextbox()
{
}

void CTextbox::OnTextInput(char c)
{
	char pCurText[256];
	strcpy(pCurText, m_pContentLabel->GetContentText());

	if (c == TEXTINPUT_BACKSPACE)
	{
		int iLen = strlen(pCurText);
		if (iLen > 0)
			pCurText[iLen - 1] = '\0';
	}
	else
	{
		if (m_pContentLabel->GetContentTextLength() >= m_iMaxLen)
			return;

		char pAppend[2];
		pAppend[0] = c;
		pAppend[1] = '\0';
		strcat(pCurText, pAppend);
	}

	m_pContentLabel->SetContentText(pCurText);

	if (m_pEventHandler)
		m_pEventHandler(m_pContentLabel->GetContentText());
}

void CTextbox::ProcessEvent(CInputEvent* pEvent)
{
	if (pEvent->eventType == EVENT_TYPE_CHARACTER)
	{
		CWindow* pParent = (CWindow*)this->GetParentWindow();
		if (pParent)
		{
			if (pParent->GetFocus() == this)
			{
				this->OnTextInput(pEvent->character);
			}
		}
	}

	IControlClickable::ProcessEvent(pEvent);
}

void CTextbox::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	Color clrOutline;

	if (!m_bIsEnabled)
	{
		clrOutline = g_clrControlDisabled;
	}
	else if (m_bMouseOver)
	{
		clrOutline = g_clrTextboxBackgroundOutlineHover;
	}
	else
	{
		clrOutline = g_clrTextboxBackgroundOutline;
	}

	// Draw box that holds the text
	pSurface->DrawSetColor(g_clrTextboxBackground);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	pSurface->DrawSetColor(clrOutline);
	pSurface->DrawOutlinedRect(x, y, x + m_iWidth, y + m_iHeight);

	CWindow* pParent = (CWindow*)this->GetParentWindow();
	if (pParent)
	{
		if (pParent->GetFocus() == this)
		{
			CApplication* pApp = CApplication::Instance();
			if (pApp->GlobalVars()->curtime - m_fLastBlink > 0.5f)
			{
				m_fLastBlink = pApp->GlobalVars()->curtime;
				m_bBlink = !m_bBlink;
			}

			if (m_bBlink)
			{
				int width, height;
				m_pContentLabel->GetTextSize(pSurface, width, height);

				pSurface->DrawSetColor(Color(255, 255, 255, 255));
				pSurface->DrawLine(x + width + 5, y + 3, x + width + 5, y + height + 3);
			}
		}
	}

	IControlClickable::Draw(pSurface);
}

bool CTextbox::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}