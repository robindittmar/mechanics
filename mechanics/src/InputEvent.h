#ifndef __GUIEVENT_H__
#define __GUIEVENT_H__

#define EVENT_TYPE_NONE			0
#define EVENT_TYPE_KEYBOARD		1
#define EVENT_TYPE_MOUSE		2
#define EVENT_TYPE_MOUSEMOVE	3
#define EVENT_TYPE_CHARACTER	4

#define EVENT_BTNDIR_NONE		-1
#define EVENT_BTNDIR_DOWN		0
#define EVENT_BTNDIR_UP			1

#define EVENT_BTN_LMOUSE			(1 << 0)
#define EVENT_BTN_RMOUSE			(1 << 1)
#define EVENT_BTN_MMOUSE			(1 << 2)
#define EVENT_BTN_TOGGLEMENU		(1 << 3)
#define EVENT_BTN_THIRDPERSON		(1 << 4)
#define EVENT_BTN_SWITCHREALFAKE	(1 << 5)
#define EVENT_BTN_DETACH			(1 << 31)

class CInputEvent
{
public:
	CInputEvent(
		int eventType = EVENT_TYPE_NONE,
		int mousex = 0,
		int mousey = 0,
		int button = -1,
		int buttonDirection = EVENT_BTNDIR_NONE,
		char character = '\0'
	);
	~CInputEvent();

	int eventType;
	int mousex, mousey;
	int button;
	int buttonDirection; // Down = 1, Up = 0
	char character;
private:
};

#endif // __GUIEVENT_H__