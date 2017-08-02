#include "IControl.h"

IControl::IControl(int x, int y, int w, int h)
{
	m_bIsEnabled = true;

	m_iX = x;
	m_iY = y;
	m_iWidth = w;
	m_iHeight = h;
}

IControl::~IControl()
{
}
