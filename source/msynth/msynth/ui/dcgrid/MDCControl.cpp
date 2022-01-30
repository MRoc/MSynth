/*

	MDCControl (C)2001 MRoc hifiShock

	Represents a abstract control not inheritanced
	from CWnd, this control needs a given device-context
	to paint.

	It provides methods for the 2 backgroundcolors,
	selectioning and positioning.

*/

#include "MDCControl.h"

MRtti MDCControl::gvRtti = MRtti( "MDCControl", 0 );

const int MDCControl::HOVER = 1;
	
const int MDCControl::SELECTED = 2;

//------------------------------------------------------------------------------------
// + CONSTRUCTION
//------------------------------------------------------------------------------------
MDCControl::MDCControl() :
	ivState( 0 )
{
	setSelected( false );
	setBkColor( MColorMap::create( 220, 220, 240 ) );
	setBkColorSelected( MColorMap::BK_COLOR5 );
	setPosition( MRect(0,0,0,0) );
};

//------------------------------------------------------------------------------------
// + DESTRUCTION
//------------------------------------------------------------------------------------
MDCControl::~MDCControl()
{
};

IRtti* MDCControl::getRtti() const
{
	return &gvRtti;
}

void* MDCControl::getInterface( const String &className ) const
{
	if( className == "MDCControl" )
		return (void*) ((MDCControl*)this);
	else
		return MObject::getInterface( className );
}

//------------------------------------------------------------------------------------
// + COLOR
// The backgroundcolor and the selection-backgroundcolor
//------------------------------------------------------------------------------------
void MDCControl::setBkColor( MColor bkColor )
{
	ivBkColor = bkColor;
};

MColor MDCControl::getBkColor()
{
	return ivBkColor;
};

void MDCControl::setBkColorSelected( MColor bkColorSelected )
{
	ivBkColorSelected = bkColorSelected;
};

MColor MDCControl::getBkColorSelected()
{
	return ivBkColorSelected;
};

//------------------------------------------------------------------------------------
// + SELECTION
// Sets the selection mode
//------------------------------------------------------------------------------------
void MDCControl::setSelected( bool isSelected )
{
	if( isSelected )
		ivState |= SELECTED;
	else if( ivState & SELECTED )
		ivState ^= SELECTED;
};

bool MDCControl::getSelected()
{
	return (ivState & this->SELECTED) != 0;
}

void MDCControl::setHover( bool isHover )
{
	if( isHover )
		ivState |= HOVER;
	else if( ivState & HOVER )
		ivState ^= HOVER;
}

bool MDCControl::getHover()
{
	return (ivState & HOVER );
}

//------------------------------------------------------------------------------------
// + SET/get POSITION
// The position of this control
//------------------------------------------------------------------------------------
void MDCControl::setPosition( MRect rect )
{
	ivRect = rect;
}

MRect MDCControl::getPosition()
{
	return ivRect;
}

void MDCControl::moveControl( MPoint point )
{
	ivRect.setPoint( point );
}

void MDCControl::setPoint( MPoint point )
{
	ivPoint = point;
}

MPoint MDCControl::getPoint()
{
	return ivPoint;
}
