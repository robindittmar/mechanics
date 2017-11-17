#include "ColorPickerPopup.h"
#include "ColorPicker.h"

CColorPickerPopup::CColorPickerPopup(int x, int y, CColorPicker* pColorPicker, CWindow* pParentWindow)
	: IControlPopup(x, y, 200, 200, pParentWindow),
	m_bInitialized(false), m_pColorPicker(pColorPicker)
{
	m_iColorFadeTexture = g_pResourceManager->GetTexture(RM_TEXTURE_COLORFADE);

	m_pHueSlider = new CSlider(4, 8, 16, 184, 1.0f, SLIDER_ORIENTATION_VERTICAL, false, 0.0f, 360.0f);
	m_pHueSlider->SetEventHandler(std::bind(&CColorPickerPopup::SetHueValue, this, std::placeholders::_1));
	this->AddChild(m_pHueSlider);
}

CColorPickerPopup::~CColorPickerPopup()
{
}

void CColorPickerPopup::SetHueValue(float fHue)
{
	// TODO: Alpha, S&L
	int r, g, b;
	Utils::HslToRgb(fHue, 1.0f, 1.0f, r, g, b);
	m_pColorPicker->SetValue(Color(255, r, g, b));
}

void CColorPickerPopup::GetAbsolutePosition(int* pX, int* pY)
{
	if (m_pColorPicker)
	{
		m_pColorPicker->GetAbsolutePosition(pX, pY);
	}

	*pX += m_iX;
	*pY += m_iY;
}

void CColorPickerPopup::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	if (!m_bInitialized)
	{
		Color clr = m_pColorPicker->GetValue();
		float h, s, l;
		Utils::RgbToHsl(clr.r(), clr.g(), clr.b(), h, s, l);

		m_pHueSlider->SetValue(h);
		m_bInitialized = true;
	}

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	pSurface->DrawSetColor(g_clrColorPickerPopupContainer);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	// Draw "rainbow" texture below slider
	// TODO: UnknownCheats Thread aufmachen oder Textur einfach auf anderer größe erzeugen
	DrawTexturedRectParms_t drawTexRectParam;
	drawTexRectParam.x0 = x + 4;
	drawTexRectParam.y0 = y + 8;
	drawTexRectParam.x1 = x + 20;
	drawTexRectParam.y1 = y + 192;

	drawTexRectParam.s0 = 0.0f;
	drawTexRectParam.t0 = 0.0f;
	drawTexRectParam.s1 = 1.0f;
	drawTexRectParam.t1 = 0.2555555555555556f;

	pSurface->DrawSetColor(255, 255, 255, 255);
	pSurface->DrawSetTexture(m_iColorFadeTexture);
	pSurface->DrawTexturedRectEx(&drawTexRectParam);

	IControlPopup::Draw(pSurface);
}