#include "Window.h"

CWindow::CWindow(int x, int y, int w, int h, const char* pTitle)
{
	m_bIsVisible = false;

	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;

	m_pTitle = NULL;
	this->Title(pTitle);
}

CWindow::~CWindow()
{
	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;

		if (p)
		{
			delete p;
		}
	}

	if (m_pTitle)
		delete[] m_pTitle;
}

bool CWindow::AddChild(IControl* pControl)
{
	m_pChildren.push_back(pControl);
	return true;
}

void CWindow::Draw(/*IDirect3DDevice9* pDevice*/)
{
	if (!m_bIsVisible)
		return;
	
	/*D3DRECT rect = {
		m_iX,
		m_iY,
		m_iX + m_iWidth,
		m_iY + m_iHeight
	};

	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(128, 255, 0, 0), 0.0f, 0);*/

	for (std::vector<IControl*>::iterator it = m_pChildren.begin(); it != m_pChildren.end(); it++)
	{
		IControl* p = *it;
		p->Draw(/*pDevice, */m_iX, m_iY);
	}
}