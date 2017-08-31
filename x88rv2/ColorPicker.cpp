#include "ColorPicker.h"

CColorPicker::CColorPicker(int x, int y, int w, int h, int a, int r, int g, int b) : IControl(x, y, w, h)
{
	m_iA = a;
	m_iR = r;
	m_iG = g;
	m_iB = b;

	m_iColorFadeTexture = g_pResourceManager->GetTexture(RM_TEXTURE_COLORFADE);
}

CColorPicker::~CColorPicker()
{
}

void CColorPicker::ProcessEvent(CInputEvent* pEvent)
{
}

void CColorPicker::Draw(ISurface* pSurface)
{
	pSurface->DrawSetColor(255, 255, 255, 255);
	pSurface->DrawSetTexture(m_iColorFadeTexture);
	pSurface->DrawTexturedRect(0, 0, 30, CGui::Instance()->ScreenHeight());
}
