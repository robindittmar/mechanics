#include "IControl.h"
#include "Application.h"

IControl::IControl(int x, int y, int w, int h)
{
	m_bIsVisible = true;
	m_bIsEnabled = true;
	m_pParent = NULL;

	m_pTooltip = NULL;

	m_bHitcheckForMouseMove = true;
	m_bHitcheckForMouseUp = true;
	m_bCanHaveFocus = false;

	m_bMouseOver = false;
	m_bMouseDown = false;

	this->SetBoundaries(x, y, w, h);
}

IControl::~IControl()
{
	m_vDependentOnes.clear();
	m_vDependencies.clear();

	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;

		if (p)
		{
			delete p;
		}
	}

	if (m_pTooltip)
		delete m_pTooltip;
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
	if (!m_bIsVisible)
		return;

	CApplication* pApp = CApplication::Instance();
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

			if (m_pTooltip)
			{
				m_fTimeLastMouseMovement = pApp->GlobalVars()->curtime;
				m_pTooltip->SetPosition(pEvent->mousex + TOOLTIP_DISTANCE_TO_MOUSE_X, pEvent->mousey + TOOLTIP_DISTANCE_TO_MOUSE_Y);
			}
		}
		else if (!m_bHitcheckForMouseMove)
		{
			this->OnMouseMove(pEvent->mousex, pEvent->mousey);
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
	if (!m_bIsVisible)
		return;

	if (m_pTooltip && m_bMouseOver)
	{
		CApplication* pApp = CApplication::Instance();
		if (pApp->GlobalVars()->curtime - m_fTimeLastMouseMovement > TOOLTIP_TIME_TO_DISPLAY)
		{
			CWindow* pParentWindow = (CWindow*)this->GetParentWindow();
			pParentWindow->SetTooltip(m_pTooltip);
		}
	}

	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(pSurface);
	}
}

void IControl::SetTooltipText(const char* p)
{
	if (!m_pTooltip)
		m_pTooltip = new CTooltip();

	m_pTooltip->SetText(p);
}

IControl* IControl::GetParentWindow()
{
	if (m_pParent == NULL)
		return this;

	return m_pParent->GetParentWindow();
}

void IControl::AddDependency(IControl* pDependency, void* pParam, bool bReverse)
{
	assert(pDependency != nullptr);

	m_vDependencies.push_back({ pDependency, pParam, bReverse });
	pDependency->AddDependentOne(this);
}

void IControl::AddDependentOne(IControl* pDependentOne)
{
	assert(pDependentOne != nullptr);

	m_vDependentOnes.push_back(pDependentOne);
}

void IControl::EvaluateDependencies()
{
	std::unordered_set<IControl*> m_setControls;

	bool bEnable = true;
	for (std::vector<ControlDependency_t>::iterator it = m_vDependencies.begin(); it != m_vDependencies.end(); it++)
	{
		if (m_setControls.find(it->control) != m_setControls.end())
			continue;

		// If its == reverse, we don't want it to be enabled
		if (it->control->ShouldDependentOnesBeEnabled(it->param) == it->reverse)
		{
			bEnable = false;

			// Iterate over remaining dependencies; if there are multiple conditions for the same
			// control it is treated as an 'OR' condition, instead of an 'AND'
			// (They will be skipped on the outer iteration anyways, since the unordered_set
			// keeps that from happening)
			std::vector<ControlDependency_t>::iterator itCopy = it + 1;
			for (;itCopy != m_vDependencies.end(); itCopy++)
			{
				// Only same control we already checked
				if (it->control != itCopy->control)
					continue;

				// If any of the other conditions tell us, that we should enable the control
				if (itCopy->control->ShouldDependentOnesBeEnabled(itCopy->param) != itCopy->reverse)
				{
					bEnable = true;
					break;
				}
				
			}

			if (!bEnable)
				break;
		}

		m_setControls.emplace(it->control);
	}

	this->SetEnabled(bEnable);
}

void IControl::NotifyDependentOnes()
{
	for (std::vector<IControl*>::iterator it = m_vDependentOnes.begin(); it != m_vDependentOnes.end(); it++)
	{
		(*it)->EvaluateDependencies();
	}
}

void IControl::SetPosition(int x, int y)
{
	m_iX = x;
	m_iY = y;
}

void IControl::SetBoundaries(int x, int y, int w, int h)
{
	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;
}
