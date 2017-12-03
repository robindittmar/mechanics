#include "KeyBinder.h"

bool CKeyBinder::m_bKeyTranslationMapInitialized = false;
std::unordered_map<unsigned short, wchar_t*> CKeyBinder::m_mapKeys;

CKeyBinder::CKeyBinder(int x, int y, int w, int h, int iEventBtn, const char* pText)
	: IControlTooltip(x, y, w, h), m_bPopupInitialized(false), m_pPopup(false),
	m_iEventButton(iEventBtn), m_pText(nullptr)
{
	m_pPopup = new CKeyBinderPopup(this);

	m_pLabel = new CLabel(0, 0, w, h, m_pText, RM_FONT_NORMAL, LABEL_ORIENTATION_LEFT);
	this->AddChild(m_pLabel);

	this->SetText(pText);

	// Initialize static [Virtual Key Code -> Wide Text] map
	if (!m_bKeyTranslationMapInitialized)
	{
		m_mapKeys[VK_LBUTTON] = L"Mouse 1";
		m_mapKeys[VK_RBUTTON] = L"Mouse 2";
		m_mapKeys[VK_MBUTTON] = L"Mouse 3";
		m_mapKeys[VK_XBUTTON1] = L"Mouse 4";
		m_mapKeys[VK_XBUTTON2] = L"Mouse 5";

		m_bKeyTranslationMapInitialized = true;
	}
}

CKeyBinder::~CKeyBinder()
{
	if (m_pText)
		delete[] m_pText;

	if (m_pPopup)
		delete m_pPopup;
}

void CKeyBinder::OnClicked()
{
	CWindow* pParentWindow = this->GetParentWindow();

	if (!m_bPopupInitialized)
	{
		m_pPopup->SetParentWindow(pParentWindow);
		m_bPopupInitialized = true;
	}

	pParentWindow->SetPopup(m_pPopup);
	m_pPopup->OnOpen();
}

bool CKeyBinder::ShouldDependentOnesBeEnabled(void* pParam)
{
	return true;
}

void CKeyBinder::SetEnabled(bool bIsEnabled)
{
	IControl::SetEnabled(bIsEnabled);

	if (m_bIsEnabled)
	{
		m_pLabel->SetTextColor(g_clrKeyBinderText);
	}
	else
	{
		m_pLabel->SetTextColor(g_clrControlDisabled);
	}
}

void CKeyBinder::SetText(const char* pText)
{
	if (m_pText)
		delete[] m_pText;

	int iLen = strlen(pText) + 1;
	m_pText = new char[iLen];
	memcpy(m_pText, pText, iLen);
}

void CKeyBinder::SetKey(unsigned short nKey)
{
	m_nKey = nKey;
	this->GenerateLabelText();

	if (m_iEventButton != -1)
	{
		CInputHandler* pInputHandler = CInputHandler::Instance();
		pInputHandler->UnregisterKey(m_iEventButton);
		pInputHandler->RegisterKey(m_nKey, m_iEventButton);
	}

	if (m_pEventHandler)
		m_pEventHandler(m_nKey);
}

void CKeyBinder::GenerateLabelText()
{
	wchar_t pBuffer[64];
	wchar_t pKeyBuffer[32];

	std::unordered_map<unsigned short, wchar_t*>::iterator it = m_mapKeys.find(m_nKey);
	if (it == m_mapKeys.end())
	{
		UINT uScanCode = MapVirtualKey(m_nKey, MAPVK_VK_TO_VSC);
		if (!uScanCode)
			return;

		LONG lParam = (uScanCode << 16);
		if (!GetKeyNameTextW(lParam, pKeyBuffer, 32))
			return;
	}
	else
	{
		wcscpy(pKeyBuffer, it->second);
	}

	swprintf(pBuffer, 64, L"%S: %s", m_pText, pKeyBuffer);
	m_pLabel->SetContentTextW(pBuffer);
}