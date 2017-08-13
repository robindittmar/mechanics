#ifndef __GUIEVENT_H__
#define __GUIEVENT_H__

#define EVENT_TYPE_KEYBOARD	1
#define EVENT_TYPE_MOUSE	2

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