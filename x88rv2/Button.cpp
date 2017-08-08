#include "Button.h"

CButton::CButton(int x, int y, int w, int h, const char* pText) : IControl(x, y, w, h)
{
	m_pContentText = NULL;
	this->ContentText(pText);
}

CButton::~CButton()
{
	if (m_pContentText)
		delete[] m_pContentText;
}

void CButton::ProcessEvents()
{
	// TODO
}

void CButton::Draw(/*IDirect3DDevice9* pDevice, */int xOff, int yOff)
{
	if (!m_bIsEnabled)
		return;

	int offsetX = m_iX + xOff;
	int offsetY = m_iY + yOff;

	/*D3DRECT rect = {
		offsetX,
		offsetY,
		offsetX + m_iWidth,
		offsetY + m_iHeight
	};

	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 255, 255), 1.0f, 0);*/
}
