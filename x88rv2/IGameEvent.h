//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#ifndef __IGAMEEVENTS_H__
#define __IGAMEEVENTS_H__

//-----------------------------------------------------------------------------
// Purpose: Engine interface into global game event management
//-----------------------------------------------------------------------------

/*
The GameEventManager keeps track and fires of all global game events. Game events
are fired by game.dll for events like player death or team wins. Each event has a
unique name and comes with a KeyValue structure providing informations about this
event. Some events are generated also by the engine.
Events are networked to connected clients and invoked there to. Therefore you
have to specify all data fields and there data types in an public resource
file which is parsed by server and broadcasted to it's clients. A typical game
event is defined like this:
"game_start"				// a new game starts
{
"roundslimit"	"long"		// max round
"timelimit"		"long"		// time limit
"fraglimit"		"long"		// frag limit
"objective"		"string"	// round objective
}
All events must have unique names (case sensitive) and may have a list
of data fields. each data field must specify a data type, so the engine
knows how to serialize/unserialize that event for network transmission.
Valid data types are string, float, long, short, byte & bool. If a
data field should not be broadcasted to clients, use the type "local".
*/


#define MAX_EVENT_NAME_LENGTH	32		// max game event name length
#define MAX_EVENT_BITS			9		// max bits needed for an event index
#define MAX_EVENT_NUMBER		(1<<MAX_EVENT_BITS)		// max number of events allowed
#define MAX_EVENT_BYTES			1024	// max size in bytes for a serialized event

#define NULL					0

typedef unsigned long long uint64;

class KeyValues;
class CGameEvent;

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char *GetName() const = 0;	// get event name

	virtual bool  IsReliable() const = 0; // if event handled reliable
	virtual bool  IsLocal() const = 0; // if event is never networked
	virtual bool  IsEmpty(const char *keyName = NULL) = 0; // check if data field exists

	// Data access
	virtual bool  GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int   GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual uint64 GetUint64(const char *keyName = NULL, uint64 defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;
	virtual const wchar_t * GetWString(char const *keyName = NULL, const wchar_t *defaultValue = L"") = 0;

	virtual void Unknown() = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUint64(const char *keyName, uint64 value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
};

#endif // __IGAMEEVENT_H__