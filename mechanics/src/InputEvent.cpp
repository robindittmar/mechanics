#include "InputEvent.h"

CInputEvent::CInputEvent()
{
	this->Clear();
}

CInputEvent::~CInputEvent()
{
}

void CInputEvent::Clear()
{
	eventType = EVENT_TYPE_NONE;
	mousex = 0;
	mousey = 0;
	mousex_old = 0;
	mousey_old = 0;
	buttons = 0;
	buttonProperties = 0;
}

bool CInputEvent::DidMouseMove()
{
	return (mousex != mousex_old || mousey != mousey_old);
}