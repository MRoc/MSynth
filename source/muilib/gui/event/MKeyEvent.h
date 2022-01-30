#ifndef __MKeyEvent
#define __MKeyEvent

#include "IUiEvent.h"

class MKeyEvent :
	public IUiEvent
{
public:

	/** 
	 * type of a key event
	 */
	enum MKeyEventType
	{
		KEY_DOWN,
		KEY_INTERMEDIATE,
		KEY_UP
	};

	/** 
	 * platform independent scancode
	 */
	enum MVirtualKey
	{
		MVK_0,
		MVK_1,
		MVK_2,
		MVK_3,
		MVK_4,
		MVK_5,
		MVK_6,
		MVK_7,
		MVK_8,
		MVK_9,
		MVK_A,
		MVK_B,
		MVK_C,
		MVK_D,
		MVK_E,
		MVK_F,
		MVK_G,
		MVK_H,
		MVK_I,
		MVK_J,
		MVK_K,
		MVK_L,
		MVK_M,
		MVK_N,
		MVK_O,
		MVK_P,
		MVK_Q,
		MVK_R,
		MVK_S,
		MVK_T,
		MVK_U,
		MVK_V,
		MVK_W,
		MVK_X,
		MVK_Y,
		MVK_Z,
		MVK_F1,
		MVK_F2,
		MVK_F3,
		MVK_F4,
		MVK_F5,
		MVK_F6,
		MVK_F7,
		MVK_F8,
		MVK_F9,
		MVK_F10,
		MVK_F11,
		MVK_F12,
		MVK_F13,
		MVK_F14,
		MVK_F15,
		MVK_F16,
		MVK_F17,
		MVK_F18,
		MVK_F19,
		MVK_F20,
		MVK_F21,
		MVK_F22,
		MVK_F23,
		MVK_F24,
		MVK_ALT,
		MVK_CAPS_LOCK,
		MVK_CONTROL,
		MVK_DECIMAL,			// .
		MVK_DELETE,				// [del]
		MVK_END,				// [end]
		MVK_CLEAR,				// [clear - numlock+5]
		MVK_ENTER,				// [return]
		MVK_ESCAPE,				// [esc]
		MVK_HOME,				// [home]
		MVK_INSERT,				// [insert]
		MVK_TAB,				// [tab]
		MVK_BACK_SPACE,			// [backspace]

		MVK_SHIFT,				// [shift]
		MVK_SPACE,				// [space]

		MVK_PASTE,				// [paste]

		MVK_PRINTSCREEN,		// [print]
		MVK_SCROLL,				// [scroll]
		MVK_BREAK,				// [pause]

		MVK_PAGE_DOWN,			// [page down]
		MVK_PAGE_UP,			// [page up]

		MVK_NUM_LOCK,			// [num lock]
		MVK_KP_DIVIDE,			// keypad /
		MVK_KP_MULTIPLY,		// keypad *
		MVK_KP_ADD,				// keypad +
		MVK_KP_SUBTRACT,		// keypad -
		MVK_KP_DECIMAL,			// keypad decimal
		MVK_KP_1,				// [keypad 1]
		MVK_KP_2,				// [keypad 2]
		MVK_KP_3,				// [keypad 3]
		MVK_KP_4,				// [keypad 4]
		MVK_KP_5,				// [keypad 5]
		MVK_KP_6,				// [keypad 6]
		MVK_KP_7,				// [keypad 7]
		MVK_KP_8,				// [keypad 8]
		MVK_KP_9,				// [keypad 9]
		MVK_KP_0,				// [keypad 0]

		MVK_UP,					// [cursor up]
		MVK_DOWN,				// [cursor down]
		MVK_LEFT,				// [cursor left]
		MVK_RIGHT,				// [cursor right]

		MVK_OEM_1,				// ';:' for US
		MVK_OEM_PLUS,			// '+' any country
		MVK_OEM_COMMA,			// ',' any country
		MVK_OEM_MINUS,			// '-' any country
		MVK_OEM_PERIOD,			// '.' any country
		MVK_OEM_2,				// '/?' for US
		MVK_OEM_3,				// '`~' for US
		MVK_OEM_4,				//  '[{' for US
		MVK_OEM_5,				//  '\|' for US
		MVK_OEM_6,				//  ']}' for US
		MVK_OEM_7,				//  ''"' for US

		MVK_BRACKETS,			// <>|

		//microsoft keys:
		MVK_LWIN,
		MVK_RWIN,
		MVK_APPS
	};

protected:

	MKeyEventType ivType;
	MVirtualKey ivScancode;
	unsigned int ivCharacter;

public:

	MKeyEvent( MKeyEventType type, MVirtualKey scancode, unsigned int character );
	virtual ~MKeyEvent();

	virtual unsigned int getKeyInt();
	virtual char getKeyChar();
	virtual MVirtualKey getScanCode();
	virtual MKeyEventType getType();
	virtual const IUiEvent::MEventType getEventType() const;
	virtual String toString() const;

	static MVirtualKey convert( unsigned int win32ScanCode );
};

#endif