#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

#include <Windows.h>
#include <map>

#include "Gui.h"
#include "InputEvent.h"

class CInputHandler
{
public:
	CInputHandler();
	~CInputHandler();

	void CreateInput(CInputEvent* pEvent, bool bCreateMouseEvent = true);

	bool RegisterKey(unsigned short vKey, int eventBtn, bool bOverride = false);
private:
	bool CheckKeyChanges(std::map<unsigned short, int>& mapKeys, std::map<unsigned short, bool>& mapStates, CInputEvent* pEvent);

	CGui* m_pGui;

	std::map<unsigned short, int> m_mapKeys;
	std::map<unsigned short, bool> m_mapKeyStates;

	std::map<unsigned short, int> m_mapMouseBtns;
	std::map<unsigned short, bool> m_mapMouseBtnStates;
	int oldMouseX, oldMouseY;
};

#endif // __INPUTHANDLER_H__