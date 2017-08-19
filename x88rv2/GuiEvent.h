#ifndef __GUIEVENT_H__
#define __GUIEVENT_H__

#define EVENT_TYPE_KEYBOARD	1
#define EVENT_TYPE_MOUSE	2

#define BTN_LMOUSE			(1 << 0);
#define BTN_RMOUSE			(1 << 1);
#define BTN_MMOUSE			(1 << 2);
#define BTN_TOGGLEMENU		(1 << 3);

class CGuiEvent
{
public:
	CGuiEvent();
	~CGuiEvent();

	int EventType() { return m_iEventType; }
	void EventType(int iEventType) { m_iEventType = iEventType; }

	
private:
	int m_iEventType;
	int mousex, mousey;
	int buttons;
};

#endif // __GUIEVENT_H__