#include "IControlPopup.h"

IControlPopup::IControlPopup(int x, int y, int w, int h, CWindow* pParentWindow)
	: IControl(x, y, w, h), m_pParentWindow(pParentWindow)
{
}

IControlPopup::~IControlPopup()
{
}

void IControlPopup::ProcessEvent(CInputEvent* pEvent)
{
	if (!m_bIsEnabled || !m_pParentWindow)
		return;

	IControl::ProcessEvent(pEvent);

	if (pEvent->eventType == EVENT_TYPE_MOUSE)
	{
		// Up or down, I don't care
		if (pEvent->button == EVENT_BTN_LMOUSE)
		{
			int x = 0, y = 0;
			this->GetAbsolutePosition(&x, &y);

			//if (!CGui::Instance()->IsMouseInRect(x, y, m_iWidth, m_iHeight))
			if (!m_bMouseOver)
			{
				m_pParentWindow->SetPopup(NULL);
				return;
			}
		}
	}
}
