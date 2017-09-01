#include "IControl.h"

IControl::IControl(int x, int y, int w, int h)
{
	m_bIsEnabled = true;
	m_pParent = NULL;

	m_bHitcheckForMouseMove = true;
	m_bHitcheckForMouseUp = true;

	m_bMouseOver = false;
	m_bMouseDown = false;

	this->SetBoundaries(x, y, w, h);
}

IControl::~IControl()
{
	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;

		if (p)
		{
			delete p;
		}
	}
}

void IControl::OnMouseMove(int mx, int my)
{
}

void IControl::OnMouseDown(int mx, int my)
{
}

void IControl::OnMouseUp(int mx, int my)
{
}

void IControl::OnClicked()
{
}

void IControl::AddChild(IControl* pControl)
{
	pControl->Parent(this);
	m_pChildren.push_back(pControl);
}

void IControl::GetAbsolutePosition(int* pX, int* pY)
{
	if (m_pParent)
	{
		m_pParent->GetAbsolutePosition(pX, pY);
	}

	*pX += m_iX;
	*pY += m_iY;
}

void IControl::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled)
		return;

	CGui* pGui = CGui::Instance();

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		int x = 0, y = 0;
		this->GetAbsolutePosition(&x, &y);

		m_bMouseOver = pGui->IsMouseInRect(x, y, m_iWidth, m_iHeight);
		if (m_bMouseOver) // Mouse inside control
		{
			if (pEvent->buttons & EVENT_BTN_LMOUSE)
			{
				if (pEvent->buttonProperties & EVENT_BTN_LMOUSE) // Button down
				{
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
			else if (pEvent->DidMouseMove())
			{
				this->OnMouseMove(pEvent->mousex, pEvent->mousey);
			}
		}
		else // Mouse outside control
		{
			// Check if MouseDown is set & mouse is released
			if (m_bMouseDown &&
				pEvent->buttons & EVENT_BTN_LMOUSE &&
				!(pEvent->buttonProperties & EVENT_BTN_LMOUSE))
			{
				if (!m_bHitcheckForMouseUp)
					this->OnMouseUp(pEvent->mousex, pEvent->mousey);

				m_bMouseDown = false;
			}

			// If we shall trigger MouseMove events even if we're not over the control
			if (!m_bHitcheckForMouseMove)
			{
				if (pEvent->DidMouseMove())
				{
					this->OnMouseMove(pEvent->mousex, pEvent->mousey);
				}
			}
		}
	}

	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->ProcessEvent(pEvent);
	}
}

void IControl::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(pSurface);
	}
}

void IControl::SetBoundaries(int x, int y, int w, int h)
{
	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;
}
