#include "ColorPickerPopup.h"
#include "ColorPicker.h"

int CColorPickerPopup::m_iColorFadeTexture = 0;

CColorPickerPopup::CColorPickerPopup(int x, int y, CColorPicker* pColorPicker, CWindow* pParentWindow)
	: IControlPopup(x, y, 234, 232, pParentWindow), m_pColorPicker(pColorPicker), m_bMouseDownInSatValPicker(false)
{
	m_bHitcheckForMouseMove = false;
	m_bHitcheckForMouseUp = false;

	m_iColorFadeTexture = g_pResourceManager->GetTexture(RM_TEXTURE_COLORFADE);

	m_pAlphaLabel = new CLabel(4, 208, 35, 20, "Alpha");
	this->AddChild(m_pAlphaLabel);

	m_pAlphaSlider = new CSlider(40, 208, 185, 20, 0.0f, SLIDER_ORIENTATION_HORIZONTAL, false, 0.0f, 255.0f);
	m_pAlphaSlider->SetDrawValue(false);
	m_pAlphaSlider->SetEventHandler(std::bind(&CColorPickerPopup::SetAlpha, this, std::placeholders::_1));
	this->AddChild(m_pAlphaSlider);

	m_pHueSlider = new CSlider(4, 4, 20, 200, 1.0f, SLIDER_ORIENTATION_VERTICAL, false, 0.0f, 360.0f);
	m_pHueSlider->SetDrawValue(false);
	m_pHueSlider->SetEventHandler(std::bind(&CColorPickerPopup::SetHue, this, std::placeholders::_1));
	this->AddChild(m_pHueSlider);
}

CColorPickerPopup::~CColorPickerPopup()
{
}

void CColorPickerPopup::OnOpen()
{
	Color clr = m_pColorPicker->GetValue();
	float h, s, v;
	Utils::RgbToHsv(clr.r(), clr.g(), clr.b(), h, s, v);

	m_iAlpha = clr.a();
	m_iHue = h;
	m_fSaturation = s;
	m_fValue = v;

	if (m_fSaturation <= 0.0f)
		m_fSaturation = FLT_MIN;

	m_pAlphaSlider->SetValue(m_iAlpha);
	m_pHueSlider->SetValue((float)m_iHue);
}

void CColorPickerPopup::SetAlpha(float fAlpha)
{
	m_iAlpha = (int)fAlpha;
	this->UpdateValue();
}

void CColorPickerPopup::SetHue(float fHue)
{
	m_iHue = fHue;

	// Build SL Texture
	g_pResourceManager->BuildSaturationValueTexture(m_iHue);

	this->UpdateValue();
}

void CColorPickerPopup::SetSaturationAndValue(float fSaturation, float fValue)
{
	if (fSaturation > 1.0f)
		fSaturation = 1.0f;
	else if (fSaturation <= 0.0f)
		fSaturation = FLT_MIN;
	
	if (fValue > 1.0f)
		fValue = 1.0f;
	else if (fValue < 0.0f)
		fValue = 0.0f;

	m_fSaturation = fSaturation;
	m_fValue = fValue;
	this->UpdateValue();
}

void CColorPickerPopup::UpdateValue()
{
	int r, g, b;
	Utils::HsvToRgb(m_iHue, m_fSaturation, m_fValue, r, g, b);
	m_pColorPicker->SetValue(Color(m_iAlpha, r, g, b));
}

void CColorPickerPopup::SetSaturationAndValueByCursorPos(int mx, int my)
{
	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	float fSaturation = (mx - (x + COLORPICKER_SVMAP_X + 1)) / (float)COLORPICKER_SVMAP_W;
	float fValue = 1.0f - ((my - (y + COLORPICKER_SVMAP_Y)) / (float)COLORPICKER_SVMAP_H);

	this->SetSaturationAndValue(fSaturation, fValue);
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

void CColorPickerPopup::OnMouseDown(int mx, int my)
{
	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	m_bMouseDownInSatValPicker = CGui::Instance()->IsMouseInRect(x + COLORPICKER_SVMAP_X, y + COLORPICKER_SVMAP_Y, COLORPICKER_SVMAP_W, COLORPICKER_SVMAP_H);
	if (m_bMouseDownInSatValPicker)
	{
		this->SetSaturationAndValueByCursorPos(mx, my);
	}
}

void CColorPickerPopup::OnMouseMove(int mx, int my)
{
	if (m_bMouseDownInSatValPicker)
	{
		this->SetSaturationAndValueByCursorPos(mx, my);
	}
}

void CColorPickerPopup::OnMouseUp(int mx, int my)
{
	m_bMouseDownInSatValPicker = false;
}

void CColorPickerPopup::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	// === Draw background ===
	pSurface->DrawSetColor(g_clrColorPickerPopupContainer);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);
	// =======================

	// === Draw Hue slider ===
	// Clockwise
	Vertex_t pVertices[4];
	pVertices[0].Init(Vector2D(x + 4, y + 4), Vector2D(0.0f, 0.0f));
	pVertices[1].Init(Vector2D(x + 24, y + 4), Vector2D(1.0f, 0.0f));
	pVertices[2].Init(Vector2D(x + 24, y + 204), Vector2D(1.0f, 1.0f));
	pVertices[3].Init(Vector2D(x + 4, y + 204), Vector2D(0.0f, 1.0f));

	pSurface->DrawSetColor(255, 255, 255, 255);
	pSurface->DrawSetTexture(m_iColorFadeTexture);
	pSurface->DrawTexturedPolygon(4, pVertices, false);
	// =======================
	
	// === Draw Saturation & Lightness block ===
	pVertices[0].Init(Vector2D(x + COLORPICKER_SVMAP_X, y + COLORPICKER_SVMAP_Y), Vector2D(0.0f, 0.0f));
	pVertices[1].Init(Vector2D(x + COLORPICKER_SVMAP_X + COLORPICKER_SVMAP_W, y + COLORPICKER_SVMAP_Y), Vector2D(1.0f, 0.0f));
	pVertices[2].Init(Vector2D(x + COLORPICKER_SVMAP_X + COLORPICKER_SVMAP_W, y + COLORPICKER_SVMAP_Y + COLORPICKER_SVMAP_H), Vector2D(1.0f, 1.0f));
	pVertices[3].Init(Vector2D(x + COLORPICKER_SVMAP_X, y + COLORPICKER_SVMAP_Y + COLORPICKER_SVMAP_H), Vector2D(0.0f, 1.0f));

	pSurface->DrawSetTexture(g_pResourceManager->GetTexture(RM_TEXTURE_SVFADE));
	pSurface->DrawTexturedPolygon(4, pVertices, false);
	// =======================

	// === Draw Saturation & Lightness selection ===
	int sat = x + COLORPICKER_SVMAP_X + (m_fSaturation * COLORPICKER_SVMAP_W);
	int val = y + COLORPICKER_SVMAP_Y + ((1.0f - m_fValue) * COLORPICKER_SVMAP_H);

	Color clrValue = m_pColorPicker->GetValue();
	clrValue.SetAlpha(255);

	pSurface->DrawSetColor(clrValue);
	pSurface->DrawFilledCircle(sat, val, 10, 16);
	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawOutlinedCircle(sat, val, 10, 16);
	// =======================

	IControlPopup::Draw(pSurface);
}