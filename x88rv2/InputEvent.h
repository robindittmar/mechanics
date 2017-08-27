#ifndef __GUIEVENT_H__
#define __GUIEVENT_H__

#define EVENT_TYPE_NONE		0
#define EVENT_TYPE_KEYBOARD	1
#define EVENT_TYPE_MOUSE	2

#define EVENT_BTN_LMOUSE			(1 << 0)
#define EVENT_BTN_RMOUSE			(1 << 1)
#define EVENT_BTN_MMOUSE			(1 << 2)
#define EVENT_BTN_TOGGLEMENU		(1 << 3)
#define EVENT_BTN_THIRDPERSON		(1 << 4)
#define EVENT_BTN_DETACH			(1 << 31)

class CInputEvent
{
public:
	CInputEvent();
	~CInputEvent();

	void Clear();

	int eventType;
	int mousex, mousey;
	int buttons;
	int buttonProperties; // Down = 1, Up = 0
private:
};

#endif // __GUIEVENT_H__