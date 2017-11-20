#include "InputHandler.h"

CInputHandler::CInputHandler()
{
	m_pGui = CGui::Instance();

	m_mapMouseBtns[VK_LBUTTON] = EVENT_BTN_LMOUSE;
	m_mapMouseBtns[VK_RBUTTON] = EVENT_BTN_RMOUSE;
	m_mapMouseBtns[VK_MBUTTON] = EVENT_BTN_MMOUSE;
}

CInputHandler::~CInputHandler()
{
}

CInputHandler* CInputHandler::Instance()
{
	static CInputHandler instance;
	return &instance;
}

bool CInputHandler::GetInput(CInputEvent* pEvent, bool bCreateMouseEvent)
{
	std::vector<CInputEvent>::iterator it = m_vEvents.begin();
	if (it == m_vEvents.end())
		return false;

	while (it->eventType == EVENT_TYPE_MOUSE && !bCreateMouseEvent)
	{
		it = m_vEvents.erase(it);
		if (it == m_vEvents.end())
			return false;
	}

	memcpy(pEvent, &(*it), sizeof(CInputEvent));
	m_vEvents.erase(it);
	return true;
}

bool CInputHandler::RegisterKey(unsigned short vKey, int eventBtn, bool bOverride)
{
	if(!bOverride)
	{
		if (m_mapKeys.find(vKey) != m_mapKeys.end())
			return false;
	}

	m_mapKeys[vKey] = eventBtn;
	return true;
}

bool CInputHandler::UnregisterKey(int eventBtn)
{
	for (std::unordered_map<unsigned short, int>::iterator it = m_mapKeys.begin(); it != m_mapKeys.end(); it++)
	{
		if (it->second == eventBtn)
		{
			m_mapKeys.erase(it);
			return true;
		}
	}

	return false;
}

void CInputHandler::OnMouseDown(unsigned short vKey, int x, int y)
{
	std::unordered_map<unsigned short, int>::iterator it = m_mapMouseBtns.find(vKey);
	if (it == m_mapMouseBtns.end())
		return;

	m_vEvents.push_back(CInputEvent(EVENT_TYPE_MOUSE, x, y, it->second, EVENT_BTNDIR_DOWN));
}

void CInputHandler::OnMouseMove(int x, int y)
{
	m_vEvents.push_back(CInputEvent(EVENT_TYPE_MOUSEMOVE, x, y, -1, EVENT_BTNDIR_NONE));
}

void CInputHandler::OnMouseUp(unsigned short vKey, int x, int y)
{
	std::unordered_map<unsigned short, int>::iterator it = m_mapMouseBtns.find(vKey);
	if (it == m_mapMouseBtns.end())
		return;

	m_vEvents.push_back(CInputEvent(EVENT_TYPE_MOUSE, x, y, it->second, EVENT_BTNDIR_UP));
}

void CInputHandler::OnKeyDown(unsigned short vKey)
{
	std::unordered_map<unsigned short, int>::iterator it = m_mapKeys.find(vKey);
	if (it == m_mapKeys.end())
		return;

	m_vEvents.push_back(CInputEvent(EVENT_TYPE_KEYBOARD, 0, 0, it->second, EVENT_BTNDIR_DOWN));
}

void CInputHandler::OnKeyUp(unsigned short vKey)
{
	std::unordered_map<unsigned short, int>::iterator it = m_mapKeys.find(vKey);
	if (it == m_mapKeys.end())
		return;

	m_vEvents.push_back(CInputEvent(EVENT_TYPE_KEYBOARD, 0, 0, it->second, EVENT_BTNDIR_UP));
}

void CInputHandler::OnText(char cChar)
{
	m_vEvents.push_back(CInputEvent(EVENT_TYPE_CHARACTER, 0, 0, -1, EVENT_BTNDIR_NONE, cChar));
}
