#include "Selectbox.h"
#include "Window.h"

CSelectbox::CSelectbox(int x, int y, int w, int h, const char* label, int selection)
	: IControl(x, y, w, h)
{
	m_iCountOptions = 0;
	m_iSelection = selection;

	m_bPopupInitialized = false;
	m_pPopup = new CSelectboxPopup(0, h, this);

	m_pLabel = new CLabel(0, -18, w, 20, label, RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	m_pSelectionLabel = new CLabel(SELECTBOX_PADDING, 0, w, h);

	this->AddChild(m_pLabel);
	this->AddChild(m_pSelectionLabel);
}

CSelectbox::~CSelectbox()
{
	if (m_pPopup)
		delete m_pPopup;

	CSelectboxItem* pCurrent;
	for(std::vector<CSelectboxItem*>::iterator it = m_vOptions.begin(); it != m_vOptions.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete pCurrent;
	}
}

void CSelectbox::OnClicked()
{
	CWindow* pWindow = (CWindow*)this->GetParentWindow();

	if (!m_bPopupInitialized)
	{
		m_pPopup->SetParentWindow(pWindow);
		m_bPopupInitialized = true;
	}

	pWindow->SetPopup(m_pPopup);
}

void CSelectbox::Draw(ISurface* pSurface)
{
	if (!m_bIsVisible)
		return;

	int x = 0, y = 0;
	this->GetAbsolutePosition(&x, &y);

	Color clrOutline;

	if (!m_bIsEnabled)
	{
		clrOutline = g_clrControlDisabled;
	}
	else if (m_bMouseOver)
	{
		clrOutline = g_clrSelectboxBackgroundOutlineHover;
	}
	else
	{
		clrOutline = g_clrSelectboxBackgroundOutline;
	}

	// Draw box that holds the current selection
	pSurface->DrawSetColor(g_clrSelectboxBackground);
	pSurface->DrawFilledRect(x, y, x + m_iWidth, y + m_iHeight);

	pSurface->DrawSetColor(clrOutline);
	pSurface->DrawOutlinedRect(x, y, x + m_iWidth, y + m_iHeight);

	// Draw labels
	IControl::Draw(pSurface);
}

bool CSelectbox::ShouldDependentOnesBeEnabled(void* pParam)
{
	return (m_vOptions[m_iSelection]->GetId() == (int)pParam);
}

void CSelectbox::SetEnabled(bool bIsEnabled)
{
	IControl::SetEnabled(bIsEnabled);

	if (m_bIsEnabled)
	{
		m_pLabel->SetTextColor(g_clrSelectboxText);
		m_pSelectionLabel->SetTextColor(g_clrSelectboxText);
	}
	else
	{
		m_pLabel->SetTextColor(g_clrControlDisabled);
		m_pSelectionLabel->SetTextColor(g_clrControlDisabled);
	}
}

void CSelectbox::ClearOptions()
{
	m_pPopup->ClearOptions();

	CSelectboxItem* pCurrent;
	for (std::vector<CSelectboxItem*>::iterator it = m_vOptions.begin(); it != m_vOptions.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete pCurrent;
	}

	m_vOptions.clear();
	m_iCountOptions = 0;
}

void CSelectbox::AddOption(int id, const char* text)
{
	CSelectboxItem* pOption = new CSelectboxItem(id, text);
	m_vOptions.push_back(pOption);
	m_pPopup->AddOption(m_iCountOptions, text, pOption->GetContentTextLength());

	m_iCountOptions++;
}

void CSelectbox::EnableControlOnSelected(int id, IControl* p)
{
	if (id >= 0 && id < m_iCountOptions)
	{
		m_vOptions[id]->AddControlToEnable(p);
	}
}

// Selection represents the index (0->first, 1->second, 2->third, etc)
void CSelectbox::SetSelection(int iSelection)
{
	if (iSelection >= 0 && iSelection < m_iCountOptions)
	{
		m_iSelection = iSelection;
		m_pSelectionLabel->SetContentText(m_vOptions[m_iSelection]->GetContentText());
		
		for (int i = 0; i < m_iCountOptions; i++)
		{
			if (i == m_iSelection)
			{
				m_vOptions[i]->EnableDependentControls();
			}
			else
			{
				m_vOptions[i]->DisableDependentControls();
			}
		}

		if (m_pEventHandler)
			m_pEventHandler(m_vOptions[m_iSelection]->GetId());

		this->NotifyDependentOnes();
	}
}

void CSelectbox::SetValue(int iValue)
{
	if (iValue >= 0 && iValue < m_iCountOptions)
	{
		this->SetSelection(m_vOptions[iValue]->GetId());
	}
}