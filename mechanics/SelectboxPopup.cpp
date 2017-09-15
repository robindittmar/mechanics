#include "SelectboxPopup.h"
#include "Window.h"
#include "Selectbox.h"

CSelectboxPopup::CSelectboxPopup(int x, int y, CSelectbox* pSelectbox, CWindow* pParentWindow) : IControl(x, y)
{
	m_iLenLargestOptionString = -1;
	m_iLargestOptionStringIndex = -1;

	m_pSelectbox = pSelectbox;
	m_pParentWindow = pParentWindow;
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

void CSelectboxPopup::OnMouseUp(int mx, int my)
{
	CGui* pGui = CGui::Instance();

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	int iCountOptions = m_pSelectbox->GetCountOptions();
	int iCurY = y;
	for (int i = 0; i < iCountOptions; i++)
	{
		if (pGui->IsMouseInRect(x, iCurY, m_iRowRenderWidth, m_iRowRenderHeight))
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
	if (!m_bIsEnabled || !m_pSelectbox || !m_pParentWindow)
		return;

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		// Up or down, I don't care
		if (pEvent->buttons & EVENT_BTN_LMOUSE)
		{
			int x = 0, y = 0;
			this->GetAbsolutePosition(&x, &y);

			if (!CGui::Instance()->IsMouseInRect(x, y, m_iWidth, m_iHeight))
			{
				m_pParentWindow->SetPopup(NULL);
			}
		}
	}

	IControl::ProcessEvent(pEvent);
}

void CSelectboxPopup::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled || !m_pSelectbox || !m_pParentWindow)
		return;

	int iCountOptions = m_pSelectbox->GetCountOptions();

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
		m_vOptionLabels[i]->SetBoundaries(x + SELECTBOX_PADDING, iCurY, m_iRowRenderWidth, m_iRowRenderHeight);
		m_vOptionLabels[i]->Draw(pSurface);

		iCurY += height + SELECTBOX_PADDING;
	}
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
