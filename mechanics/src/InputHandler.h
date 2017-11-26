#ifndef __INPUTHANDLER_H__
#define __INPUTHANDLER_H__

// Std Lib
#include <Windows.h>
#include <unordered_map>
#include <vector>

// Custom
#include "Gui.h"
#include "InputEvent.h"

#define TEXTINPUT_BACKSPACE	8

class CInputHandler
{
public:
	static CInputHandler* Instance();

	/// <summary>
	/// Retrieves one of the events that might've stored up since our last processing.
	/// (once called the event is removed from the queue)
	/// </summary>
	/// <param name="pEvent">Where to write the event to</param>
	/// <param name="bCreateMouseEvent">Wether or not we're interested in mouse events (if not, they're cleared)</param>
	/// <returns>If an event was found (false if queue is empty)</returns>
	bool GetInput(CInputEvent* pEvent, bool bCreateMouseEvent = true);

	/// <summary>
	/// Register a translation from Virutal Key Code to an Event defintion
	/// </summary>
	/// <param name="vKey">Virtual Key Code</param>
	/// <param name="eventBtn">Event Code</param>
	/// <param name="bOverride">Wether or not to override the VKCode if it already exists</param>
	/// <returns>false if override=false and already exists</returns>
	bool RegisterKey(unsigned short vKey, int eventBtn, bool bOverride = false);
	
	/// <summary>
	/// Unregister Event button
	/// </summary>
	/// <param name="eventBtn">Event Code</param>
	/// <returns>Wether the event code has been registered before</returns>
	bool UnregisterKey(int eventBtn);

	void OnMouseDown(unsigned short vKey, int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(unsigned short vKey, int x, int y);
	
	void OnMouseWheel(short delta, int x, int y);

	void OnKeyDown(unsigned short vKey);
	void OnKeyUp(unsigned short vKey);
	void OnText(char cChar);
private:
	CGui* m_pGui;

	std::unordered_map<unsigned short, int> m_mapKeys;
	std::unordered_map<unsigned short, int> m_mapMouseBtns;

	std::vector<CInputEvent> m_vEvents;

	CInputHandler();
	CInputHandler(const CInputHandler&) = delete;
	~CInputHandler();
};

#endif // __INPUTHANDLER_H__