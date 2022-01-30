#include "MNativeEvent.h"
#include <framework/primitive/MInt.h>

MNativeEvent::MNativeEvent( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) :
	ivHwnd( hwnd ),
	ivMessage( msg ),
	ivLParam( wparam ),
	ivWParam( lparam )
{
}

MNativeEvent::~MNativeEvent(){}

HWND MNativeEvent::getHwnd() const{ return ivHwnd; }

UINT MNativeEvent::getMessage() const{ return ivMessage; }

WPARAM MNativeEvent::getWParam() const{ return ivWParam; }

LPARAM MNativeEvent::getLParam() const{ return ivLParam; }

const IUiEvent::MEventType MNativeEvent::getEventType() const{ return IUiEvent::MEventType::EVENT_NATIVE; }

void MNativeEvent::setReturnCode( LRESULT returnCode ){ ivReturnCode = returnCode; }

LRESULT MNativeEvent::getReturnCode() const{ return ivReturnCode; }

String MNativeEvent::toString() const
{
	return
		"<iuievent eventtype=\"" +
		IUiEvent::toString( getEventType() ) +
		"\" msg=\"" +
		MInt::toString( (int) getMessage() ) +
		"\" lparam=\"" +
		MInt::toString( (int) getLParam() ) +
		"\" wparam=\"" + 
		MInt::toString( (int) getWParam() ) +
		"\"/>";
}