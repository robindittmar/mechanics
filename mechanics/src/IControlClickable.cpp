#include "IControlClickable.h"
#include "Window.h"

IControlClickable::IControlClickable(int x, int y, int w, int h)
	: IControl(x, y, w, h), m_bHitcheckForMouseMove(true), m_bHitcheckForMouseUp(true),
	m_bCanHaveFocus(false), m_bMouseOver(false), m_bMouseDown(false)
{
}

IControlClickable::~IControlClickable()
{
}

void IControlClickable::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsVisible)
		return;

	CGui* pGui = CGui::Instance();
	int x = 0, y = 0;

	if (pEvent->eventType == EVENT_TYPE_MOUSE ||
		pEvent->eventType == EVENT_TYPE_MOUSEMOVE)
	{
		this->GetAbsolutePosition(&x, &y);
		m_bMouseOver = pGui->IsMouseInRect(x, y, m_iWidth, m_iHeight);
	}

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		if (m_bMouseOver) // Mouse inside control
		{
			if (pEvent->button == EVENT_BTN_LMOUSE)
			{
				if (pEvent->buttonDirection == EVENT_BTNDIR_DOWN) // Button down
				{
					if (m_bCanHaveFocus)
					{
						CWindow* pParent = (CWindow*)this->GetParentWindow();
						if (pParent)
						{
							pParent->SetFocus(this);
						}
					}

					this->OnMouseDown(pEvent->mousex, pEvent->mousey);
					m_bMouseDown = true;
				}
				else // Button up
				{
					this->OnMouseUp(pEvent->mousex, pEvent->mousey);

					if (m_bMouseDown)
					{
						this->OnClicked();
						m_bMouseDown = false;
					}
				}
			}
		}
		else // Mouse outside control
		{
			// Check if MouseDown is set & mouse is released
			if (m_bMouseDown &&
				pEvent->button == EVENT_BTN_LMOUSE &&
				pEvent->buttonDirection == EVENT_BTNDIR_UP)
			{
				if (!m_bHitcheckForMouseUp)
					this->OnMouseUp(pEvent->mousex, pEvent->mousey);

				m_bMouseDown = false;
			}
		}
	}
	else if (pEvent->eventType == EVENT_TYPE_MOUSEMOVE)
	{
		if (m_bMouseOver) // Mouse inside control
		{
			this->OnMouseMove(pEvent->mousex, pEvent->mousey);
		}
		else if (!m_bHitcheckForMouseMove) // We don't care about collision
		{
			this->OnMouseMove(pEvent->mousex, pEvent->mousey);
		}
	}

	IControl::ProcessEvent(pEvent);
}
