#include "KeyBinderPopup.h"
#include "KeyBinder.h"

CKeyBinderPopup::CKeyBinderPopup(CKeyBinder* pKeyBinder, CWindow* pParentWindow)
	: IControlPopup(0, 0, 0, 0, pParentWindow),
	m_pKeyBinder(pKeyBinder)
{
	m_pPressKeyLabel = new CLabel(0, 0, 0, 0, "Press desired key to configure ...", RM_FONT_NORMAL, LABEL_ORIENTATION_CENTER);
	this->AddChild(m_pPressKeyLabel);
}

CKeyBinderPopup::~CKeyBinderPopup()
{
}

void CKeyBinderPopup::OnOpen()
{
	CInputHandler::Instance()->SetRawInput(true);
}

void CKeyBinderPopup::OnClose()
{
	CInputHandler::Instance()->SetRawInput(false);
}

void CKeyBinderPopup::ProcessEvent(CInputEvent* pEvent)
{
	if (pEvent->eventType == EVENT_TYPE_KEYBOARD)
	{
		if (pEvent->buttonDirection == EVENT_BTNDIR_DOWN)
		{
			this->OnClose();
			m_pKeyBinder->SetKey(pEvent->button);
			m_pParentWindow->SetPopup(nullptr);
		}
	}

	IControlPopup::ProcessEvent(pEvent);
}

void CKeyBinderPopup::Draw(ISurface* pSurface)
{
	int x, y, w, h;
	m_pParentWindow->GetClientPosition(&x, &y);
	m_pParentWindow->GetClientSize(&w, &h);

	m_pPressKeyLabel->SetBoundaries(x, y, w, h);

	pSurface->DrawSetColor(200, 0, 0, 0);
	pSurface->DrawFilledRect(x, y, x + w, y + h);

	IControlPopup::Draw(pSurface);
}