#include "Canvas.h"

CCanvas::CCanvas(int x, int y, int w, int h, Color backgroundColor) : IControl(x, y, w, h)
{
	m_clrBackground = backgroundColor;
}

CCanvas::~CCanvas()
{
}

void CCanvas::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	if(m_clrBackground.a() > 0)
	{
		int x = 0, y = 0;
		this->GetAbsolutePosition(&x, &y);

		pSurface->DrawSetColor(m_clrBackground);
		pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);
	}

	IControl::Draw(pSurface);
}
