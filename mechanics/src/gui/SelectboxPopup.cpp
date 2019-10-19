#include "SelectboxPopup.h"
#include "Window.h"
#include "Selectbox.h"

CSelectboxPopup::CSelectboxPopup(int x, int y, CSelectbox* pSelectbox, CWindow* pParentWindow)
	: IControlPopup(x, y, 0, 0, pParentWindow),
	m_pSelectbox(pSelectbox), m_iLenLargestOptionString(-1), m_iLargestOptionStringIndex(-1)
{
}

CSelectboxPopup::~CSelectboxPopup()
{
	CLabel* pCurLabel;
	for (std::vector<CLabel*>::iterator it = m_vOptionLabels.begin(); it != m_vOptionLabels.end(); it++)
	{
		pCurLabel = *it;

		if (pCurLabel)
			delete pCurLabel;
	}
}

void CSelectboxPopup::OnMouseMove(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	m_iSelectedOption = -1;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iCountOptions = m_pSelectbox->GetCountOptions();
	int iCurY = y + SELECTBOX_PADDING;
	for (int i = 0; i < iCountOptions; i++)
	{
		if (pGui->IsMouseInRect(x, iCurY, m_iWidth, m_iRowRenderHeight))
		{
			m_iSelectedOption = i;
			break;
		}

		iCurY += m_iRowRenderHeight;
	}
}

void CSelectboxPopup::OnMouseUp(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iCountOptions = m_pSelectbox->GetCountOptions();
	int iCurY = y + SELECTBOX_PADDING;
	for (int i = 0; i < iCountOptions; i++)
	{
		if (pGui->IsMouseInRect(x, iCurY, m_iWidth, m_iRowRenderHeight))
		{
			m_pSelectbox->SetSelection(i);
			m_pParentWindow->SetPopup(NULL);
			break;
		}

		iCurY += m_iRowRenderHeight;
	}
}

void CSelectboxPopup::GetAbsolutePosition(int* pX, int* pY)
{
	m_pSelectbox->GetAbsolutePosition(pX, pY);
	*pX += m_iX;
	*pY += m_iY;
}

void CSelectboxPopup::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_pSelectbox)
		return;

	IControlPopup::ProcessEvent(pEvent);
}

void CSelectboxPopup::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled || !m_pSelectbox || !m_pParentWindow)
		return;

	int iCountOptions = m_pSelectbox->GetCountOptions();

	if (iCountOptions > 0)
	{
		// Get absolute position
		int x = 0, y = 0;
		this->GetAbsolutePosition(&x, &y);

		// Get size of longest string
		int width, height;
		m_vOptionLabels[m_iLargestOptionStringIndex]->GetTextSize(pSurface, width, height);

		// Save row width/height for event handling
		m_iRowRenderWidth = width + (SELECTBOX_PADDING * 2);
		m_iRowRenderHeight = height + SELECTBOX_PADDING;

		int iSelectboxWidth = m_pSelectbox->Width();

		this->m_iWidth = iSelectboxWidth < m_iRowRenderWidth ? m_iRowRenderWidth : iSelectboxWidth;
		this->m_iHeight = (m_iRowRenderHeight * iCountOptions) + SELECTBOX_PADDING;

		// Render popup box first
		pSurface->DrawSetColor(255, 50, 50, 50);
		pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

		// Render options
		int iCurY = y + SELECTBOX_PADDING;
		for (int i = 0; i < iCountOptions; i++)
		{
			if (m_iSelectedOption == i)
			{
				pSurface->DrawSetColor(g_clrSelectboxPopupSelection);
				pSurface->DrawFilledRect(x, iCurY, x + m_iWidth, iCurY + m_iRowRenderHeight);
				pSurface->DrawSetColor(255, 50, 50, 50);
			}

			m_vOptionLabels[i]->SetBoundaries(x + SELECTBOX_PADDING, iCurY, m_iRowRenderWidth, m_iRowRenderHeight);
			m_vOptionLabels[i]->Draw(pSurface);

			iCurY += height + SELECTBOX_PADDING;
		}
	}

	IControlPopup::Draw(pSurface);
}

void CSelectboxPopup::ClearOptions()
{
	CLabel* pCurLabel;
	for (std::vector<CLabel*>::iterator it = m_vOptionLabels.begin(); it != m_vOptionLabels.end(); it++)
	{
		pCurLabel = *it;

		if (pCurLabel)
			delete pCurLabel;
	}

	m_vOptionLabels.clear();

	m_iLenLargestOptionString = -1;
	m_iLargestOptionStringIndex = -1;
}

void CSelectboxPopup::AddOption(int curIdx, const char* text, int len)
{
	m_vOptionLabels.push_back(new CLabel(0, 0, 0, 0, text));

	if (len > m_iLenLargestOptionString)
	{
		m_iLenLargestOptionString = len;
		m_iLargestOptionStringIndex = curIdx;
	}
}

void CSelectboxPopup::AddOptionW(int curIdx, const wchar_t* text, int len)
{
	CLabel* pLabel = new CLabel();
	pLabel->SetContentTextW(text);

	m_vOptionLabels.push_back(pLabel);

	if (len > m_iLenLargestOptionString)
	{
		m_iLenLargestOptionString = len;
		m_iLargestOptionStringIndex = curIdx;
	}
}