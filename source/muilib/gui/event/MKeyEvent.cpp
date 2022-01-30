#include "MKeyEvent.h"
#include <framework/io/MLogger.h>
#include <framework/primitive/MInt.h>
#include <framework/MNativeTypes.h>

MKeyEvent::MKeyEvent( MKeyEventType type, MVirtualKey scancode, unsigned int character ) :
	ivType( type ),
	ivScancode( scancode ),
	ivCharacter( character )
{
}

MKeyEvent::~MKeyEvent()
{
}

unsigned int MKeyEvent::getKeyInt()
{
	return ivCharacter;
}

char MKeyEvent::getKeyChar()
{
	return (char)ivCharacter;
}

MKeyEvent::MVirtualKey MKeyEvent::getScanCode()
{
	return ivScancode;
}

MKeyEvent::MKeyEventType MKeyEvent::getType()
{
	return ivType;
}

const IUiEvent::MEventType MKeyEvent::getEventType() const
{
	return IUiEvent::EVENT_KEY;
}

MKeyEvent::MVirtualKey MKeyEvent::convert( unsigned int win32ScanCode )
{
	switch( win32ScanCode )
	{
		case VK_CONTROL: return MKeyEvent::MVK_CONTROL;
		case VK_MENU: return MKeyEvent::MVK_ALT;
		case VK_BACK: return MKeyEvent::MVK_BACK_SPACE;
		case VK_TAB: return MKeyEvent::MVK_TAB;
		case VK_RETURN: return MKeyEvent::MVK_ENTER;
		case VK_SHIFT: return MKeyEvent::MVK_SHIFT;
		case VK_PAUSE: return MKeyEvent::MVK_BREAK;
		case VK_CAPITAL: return MKeyEvent::MVK_CAPS_LOCK;
		case VK_ESCAPE: return MKeyEvent::MVK_ESCAPE;
		case VK_SPACE: return MKeyEvent::MVK_SPACE;
		case VK_PRIOR: return MKeyEvent::MVK_PAGE_UP;
		case VK_NEXT: return MKeyEvent::MVK_PAGE_DOWN;
		case VK_END: return MKeyEvent::MVK_END;
		case VK_HOME: return MKeyEvent::MVK_HOME;
		case VK_CLEAR: return MKeyEvent::MVK_CLEAR;
		case VK_LEFT: return MKeyEvent::MVK_LEFT;
		case VK_UP: return MKeyEvent::MVK_UP;
		case VK_RIGHT: return MKeyEvent::MVK_RIGHT;
		case VK_DOWN: return MKeyEvent::MVK_DOWN;
		case VK_SNAPSHOT: return MKeyEvent::MVK_PRINTSCREEN;
		case VK_INSERT: return MKeyEvent::MVK_INSERT;
		case VK_DELETE: return MKeyEvent::MVK_DELETE;
		case VK_NUMPAD0: return MKeyEvent::MVK_KP_0;
		case VK_NUMPAD1: return MKeyEvent::MVK_KP_1;
		case VK_NUMPAD2: return MKeyEvent::MVK_KP_2;
		case VK_NUMPAD3: return MKeyEvent::MVK_KP_3;
		case VK_NUMPAD4: return MKeyEvent::MVK_KP_4;
		case VK_NUMPAD5: return MKeyEvent::MVK_KP_5;
		case VK_NUMPAD6: return MKeyEvent::MVK_KP_6;
		case VK_NUMPAD7: return MKeyEvent::MVK_KP_7;
		case VK_NUMPAD8: return MKeyEvent::MVK_KP_8;
		case VK_NUMPAD9: return MKeyEvent::MVK_KP_9;
		case VK_MULTIPLY: return MKeyEvent::MVK_KP_MULTIPLY;
		case VK_ADD: return MKeyEvent::MVK_KP_ADD;
		case VK_SUBTRACT: return MKeyEvent::MVK_KP_SUBTRACT;
		case VK_DIVIDE: return MKeyEvent::MVK_KP_DIVIDE;
		case VK_DECIMAL: return MKeyEvent::MVK_KP_DECIMAL;
		case VK_F1: return MKeyEvent::MVK_F1;
		case VK_F2: return MKeyEvent::MVK_F2;
		case VK_F3: return MKeyEvent::MVK_F3;
		case VK_F4: return MKeyEvent::MVK_F4;
		case VK_F5: return MKeyEvent::MVK_F5;
		case VK_F6: return MKeyEvent::MVK_F6;
		case VK_F7: return MKeyEvent::MVK_F7;
		case VK_F8: return MKeyEvent::MVK_F8;
		case VK_F9: return MKeyEvent::MVK_F9;
		case VK_F10: return MKeyEvent::MVK_F10;
		case VK_F11: return MKeyEvent::MVK_F11;
		case VK_F12: return MKeyEvent::MVK_F12;
		case VK_F13: return MKeyEvent::MVK_F13;
		case VK_F14: return MKeyEvent::MVK_F14;
		case VK_F15: return MKeyEvent::MVK_F15;
		case VK_F16: return MKeyEvent::MVK_F16;
		case VK_F17: return MKeyEvent::MVK_F17;
		case VK_F18: return MKeyEvent::MVK_F18;
		case VK_F19: return MKeyEvent::MVK_F19;
		case VK_F20: return MKeyEvent::MVK_F20;
		case VK_F21: return MKeyEvent::MVK_F21;
		case VK_F22: return MKeyEvent::MVK_F22;
		case VK_F23: return MKeyEvent::MVK_F23;
		case VK_F24: return MKeyEvent::MVK_F24;
		case VK_NUMLOCK: return MKeyEvent::MVK_NUM_LOCK;
		case VK_SCROLL: return MKeyEvent::MVK_SCROLL;
		case VK_OEM_1: return MKeyEvent::MVK_OEM_1;
		case VK_OEM_PLUS: return MKeyEvent::MVK_OEM_PLUS;
		case VK_OEM_COMMA: return MKeyEvent::MVK_OEM_COMMA;
		case VK_OEM_MINUS: return MKeyEvent::MVK_OEM_MINUS;
		case VK_OEM_PERIOD: return MKeyEvent::MVK_OEM_PERIOD;
		case VK_OEM_2: return MKeyEvent::MVK_OEM_2;
		case VK_OEM_3: return MKeyEvent::MVK_OEM_3;
		case VK_OEM_4: return MKeyEvent::MVK_OEM_4;
		case VK_OEM_5: return MKeyEvent::MVK_OEM_5;
		case VK_OEM_6: return MKeyEvent::MVK_OEM_6;
		case VK_OEM_7: return MKeyEvent::MVK_OEM_7;
		case VK_OEM_102: return MKeyEvent::MVK_BRACKETS;

		// microsoft keys...
		case VK_LWIN: return MKeyEvent::MVK_LWIN;
		case VK_RWIN: return MKeyEvent::MVK_RWIN;
		case VK_APPS: return MKeyEvent::MVK_APPS;

		default:
			if( win32ScanCode >= 65 && win32ScanCode <= 90 )
				return (MKeyEvent::MVirtualKey) ( ((int)MKeyEvent::MVK_A) + (((int)win32ScanCode) - 65) );
			else if( win32ScanCode >= 97 && win32ScanCode <= 122 )
				return (MKeyEvent::MVirtualKey) ( ((int)MKeyEvent::MVK_A) + (((int)win32ScanCode) - 97) );
			else if( win32ScanCode >= 48 && win32ScanCode <= 57 )
				return (MKeyEvent::MVirtualKey) ( ((int)MKeyEvent::MVK_0) + (((int)win32ScanCode) - 48) );
			break;
	};

	MLogger::logWarning(
		"<mkeyevent::convert msg=\"unknown scancode\" value=\"%u\"/>\n", (unsigned int)win32ScanCode );

	return MKeyEvent::MVK_0;
}

String MKeyEvent::toString() const
{
	String back = "<iuievent eventtype=\"" + IUiEvent::toString( getEventType() ) + "\" type=\"";
	switch( ivType )
	{
	case KEY_DOWN: back += "KEY_DOWN"; break;
	case KEY_INTERMEDIATE: back += "KEY_INTERMEDIATE"; break;
	case KEY_UP: back += "KEY_UP"; break;
	}
	back +=
		"\" scancode=\"" + MInt::toString( ivScancode ) +
		"\" character=\"" + MInt::toString( ivCharacter ) + "/>";

	return back;
}