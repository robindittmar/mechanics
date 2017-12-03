#ifndef __IINPUTSYSTEM_H__
#define __IINPUTSYSTEM_H__

#include "IAppSystem.h"

#ifndef NULL
#define NULL	0
#endif // NULL

#define INVALID_USER_ID		-1

enum ButtonCode_t;
enum AnalogCode_t;

struct InputEvent_t
{
	int m_nType;				// Type of the event (see InputEventType_t)
	int m_nTick;				// Tick on which the event occurred
	int m_nData;				// Generic 32-bit data, what it contains depends on the event
	int m_nData2;				// Generic 32-bit data, what it contains depends on the event
	int m_nData3;				// Generic 32-bit data, what it contains depends on the event
};

///-----------------------------------------------------------------------------
/// An enumeration describing well-known cursor icons
///-----------------------------------------------------------------------------
enum InputStandardCursor_t
{
	INPUT_CURSOR_NONE = 0,
	INPUT_CURSOR_ARROW,
	INPUT_CURSOR_IBEAM,
	INPUT_CURSOR_HOURGLASS,
	INPUT_CURSOR_CROSSHAIR,
	INPUT_CURSOR_WAITARROW,
	INPUT_CURSOR_UP,
	INPUT_CURSOR_SIZE_NW_SE,
	INPUT_CURSOR_SIZE_NE_SW,
	INPUT_CURSOR_SIZE_W_E,
	INPUT_CURSOR_SIZE_N_S,
	INPUT_CURSOR_SIZE_ALL,
	INPUT_CURSOR_NO,
	INPUT_CURSOR_HAND,

	INPUT_CURSOR_COUNT
};

// Random guess (by Robin)
typedef void* PlatWindow_t;
typedef void* InputCursorHandle_t;

///-----------------------------------------------------------------------------
/// Main interface for input. This is a low-level interface, creating an
/// OS-independent queue of low-level input events which were sampled since
/// the last call to PollInputState. It also contains facilities for cursor
/// control and creation.
///-----------------------------------------------------------------------------
class IInputSystem : public IAppSystem
{
public:
	void ResetInputState();
};

#endif // __IINPUTSYSTEM_H__