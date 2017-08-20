#include "IControl.h"

IControl::IControl(int x, int y, int w, int h)
{
	m_bIsEnabled = true;

	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;
	m_pParent = NULL;
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

void IControl::ProcessEvent(CInputEvent* pEvent)
{
	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->ProcessEvent(pEvent);
	}
}

void IControl::Draw(ISurface* pSurface)
{
	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(pSurface);
	}
}

void IControl::GetAbsolutePosition(int* pX, int* pY)
{
	if(m_pParent)
	{
		m_pParent->GetAbsolutePosition(pX, pY);
	}

	*pX += m_iX;
	*pY += m_iY;
}

bool IControl::AddChild(IControl* pControl)
{
	pControl->Parent(this);
	m_pChildren.push_back(pControl);
	return true;
}
