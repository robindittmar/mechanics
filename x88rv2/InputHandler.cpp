#include "InputHandler.h"

CInputHandler::CInputHandler()
{
	m_pGui = CGui::Instance();

	oldMouseX = 0;
	oldMouseY = 0;

	m_mapMouseBtns[VK_LBUTTON] = EVENT_BTN_LMOUSE;
	m_mapMouseBtns[VK_RBUTTON] = EVENT_BTN_RMOUSE;
	m_mapMouseBtns[VK_MBUTTON] = EVENT_BTN_MMOUSE;

	m_mapMouseBtnStates[VK_LBUTTON] = false;
	m_mapMouseBtnStates[VK_RBUTTON] = false;
	m_mapMouseBtnStates[VK_MBUTTON] = false;
}

CInputHandler::~CInputHandler()
{
}

void CInputHandler::CreateInput(CInputEvent* pEvent, bool bCreateMouseEvent)
{
	// Check mouse
	if(bCreateMouseEvent)
	{
		if(m_pGui->GetMousePos())
		{
			bool bStateChange = this->CheckKeyChanges(m_mapMouseBtns, m_mapMouseBtnStates, pEvent);
			if(m_pGui->MouseX() != oldMouseX || m_pGui->MouseY() != oldMouseY || bStateChange)
			{
				pEvent->eventType = EVENT_TYPE_MOUSE;
				pEvent->mousex = m_pGui->MouseX();
				pEvent->mousey = m_pGui->MouseY();
				pEvent->mousex_old = oldMouseX;
				pEvent->mousey_old = oldMouseY;
				
				oldMouseX = m_pGui->MouseX();
				oldMouseY = m_pGui->MouseY();
				// Can't have mouse & keyboard event at the same time :P
				return;
			}
		}
	}

	// Check keyboard
	if(this->CheckKeyChanges(m_mapKeys, m_mapKeyStates, pEvent))
	{
		pEvent->eventType = EVENT_TYPE_KEYBOARD;
	}
}

bool CInputHandler::RegisterKey(unsigned short vKey, int eventBtn, bool bOverride)
{
	if(!bOverride)
	{
		if (m_mapKeys.find(vKey) != m_mapKeys.end())
			return false;
	}

	m_mapKeys[vKey] = eventBtn;
	m_mapKeyStates[vKey] = false;
	return true;
}

bool CInputHandler::CheckKeyChanges(std::map<unsigned short, int>& mapKeys, std::map<unsigned short, bool>& mapStates, CInputEvent* pEvent)
{
	bool bStateChange = false;
	unsigned short vKeyCode;
	int evnt;
	std::map<unsigned short, int>::iterator it;

	for(it = mapKeys.begin(); it != mapKeys.end(); it++)
	{
		vKeyCode = it->first;

		if(GetAsyncKeyState(vKeyCode) & 0x8000)
		{
			if(!mapStates[vKeyCode])
			{
				mapStates[vKeyCode] = true;
				bStateChange = true;

				evnt = it->second;
				pEvent->buttons |= evnt;
				pEvent->buttonProperties |= evnt;
			}
		}
		else
		{
			if(mapStates[vKeyCode])
			{
				mapStates[vKeyCode] = false;
				bStateChange = true;
				
				evnt = it->second;
				pEvent->buttons |= evnt;
				pEvent->buttonProperties &= ~(evnt);
			}
		}
	}

	return bStateChange;
}