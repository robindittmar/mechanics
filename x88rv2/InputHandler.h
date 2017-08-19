#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

#include <Windows.h>
#include "GuiEvent.h"

class CInputHandler
{
public:
	CInputHandler();
	~CInputHandler();

	void CreateInput(CGuiEvent* pEvent);
private:
};

#endif // __INPUTHANDLER_H__