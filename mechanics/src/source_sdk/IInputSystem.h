#ifndef __IINPUTSYSTEM_H__
#define __IINPUTSYSTEM_H__

#include "IAppSystem.h"

#ifndef NULL
#define NULL	0
#endif // NULL

#define INVALID_USER_ID		-1

enum ButtonCode_t : int // TODO NICHT VOLLSTÄNDIG!!!!!!
{
    BUTTON_CODE_INVALID = -1,
    BUTTON_CODE_NONE = 0,

    KEY_FIRST = 0,

    KEY_NONE = KEY_FIRST,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_PAD_0,
    KEY_PAD_1,
    KEY_PAD_2,
    KEY_PAD_3,
    KEY_PAD_4,
    KEY_PAD_5,
    KEY_PAD_6,
    KEY_PAD_7,
    KEY_PAD_8,
    KEY_PAD_9,
    KEY_PAD_DIVIDE,
    KEY_PAD_MULTIPLY,
    KEY_PAD_MINUS,
    KEY_PAD_PLUS,
    KEY_PAD_ENTER,
    KEY_PAD_DECIMAL,
    KEY_LBRACKET,
    KEY_RBRACKET,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_BACKQUOTE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_BACKSLASH,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_ENTER,
    KEY_SPACE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_CAPSLOCK,
    KEY_NUMLOCK,
    KEY_ESCAPE,
    KEY_SCROLLLOCK,
    KEY_INSERT,
    KEY_DELETE,
    KEY_HOME,
    KEY_END,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_BREAK,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LALT,
    KEY_RALT,
    KEY_LCONTROL,
    KEY_RCONTROL,
    KEY_LWIN,
    KEY_RWIN,
    KEY_APP,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_CAPSLOCKTOGGLE,
    KEY_NUMLOCKTOGGLE,
    KEY_SCROLLLOCKTOGGLE,

    KEY_LAST = KEY_SCROLLLOCKTOGGLE,
    KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

    // Mouse
    MOUSE_FIRST = KEY_LAST + 1,

    MOUSE_LEFT = MOUSE_FIRST,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_4,
    MOUSE_5,
    MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up
    MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

    MOUSE_LAST = MOUSE_WHEEL_DOWN,
    MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1
};
enum AnalogCode_t : int;

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
    bool IsButtonDown(ButtonCode_t code);
};

#endif // __IINPUTSYSTEM_H__