#include "InputEvent.h"

CInputEvent::CInputEvent(
	int eventType,
	int mousex,
	int mousey,
	int button,
	int buttonDirection,
	char character
)
{
	this->eventType = eventType;
	this->mousex = mousex;
	this->mousey = mousey;
	this->button = button;
	this->buttonDirection = buttonDirection;
	this->character = character;
}

CInputEvent::~CInputEvent()
{
}
