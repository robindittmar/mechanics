#include "ColorPickerPopup.h"

CColorPickerPopup::CColorPickerPopup(int x, int y, CColorPicker* pColorPicker, CWindow* pParentWindow) : IControl(x, y)
{
	m_iColorFadeTexture = g_pResourceManager->GetTexture(RM_TEXTURE_COLORFADE);

	m_pColorPicker = pColorPicker;
	m_pParentWindow = pParentWindow;
}

CColorPickerPopup::~CColorPickerPopup()
{
}

void CColorPickerPopup::OnMouseUp(int mx, int my)
{
}

void CColorPickerPopup::GetAbsolutePosition(int* pX, int* pY)
{
}

void CColorPickerPopup::ProcessEvent(CInputEvent* pEvent)
{
}

void CColorPickerPopup::Draw(ISurface* pSurface)
{
}