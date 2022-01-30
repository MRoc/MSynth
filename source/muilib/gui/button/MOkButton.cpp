#include "MOkButton.h"

MOkButton::MOkButton( String text, MColor textColor, MColor disabledColor, MColor bkColor ) :
	MButton( text, textColor, disabledColor, bkColor ),
	ivPtToDelete( 0 )
{
}

MOkButton::~MOkButton()
{
}

void MOkButton::onMouseUp( MMouseEvent* ptEvent )
{
	if( ivPtToDelete )
		delete ivPtToDelete;
}

void MOkButton::setOnDelete( MTopWnd* pWnd )
{
	ivPtToDelete = pWnd;
}