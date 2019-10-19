#include "IControl.h"
#include "../Application.h"

IControl::IControl(int x, int y, int w, int h)
	: IDependable((EnableDependency_t)&IControl::SetEnabled),
	m_bIsVisible(true), m_bIsEnabled(true), m_pParent(nullptr)
{
	this->SetBoundaries(x, y, w, h);
}

IControl::~IControl()
{
	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;

		if (p)
			delete p;
	}
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

CWindow* IControl::GetParentWindow()
{
	if (m_pParent == NULL)
		return (CWindow*)this;

	return m_pParent->GetParentWindow();
}

void IControl::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsVisible)
		return;

	for(std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		(*it)->ProcessEvent(pEvent);
	}
}

void IControl::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(pSurface);
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
