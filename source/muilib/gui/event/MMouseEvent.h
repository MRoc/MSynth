#ifndef __MMouseEvent
#define __MMouseEvent

#include <framework/primitive/MPoint.h>
#include "IUiEvent.h"

class MMouseEvent :
	public IUiEvent
{
public:

	enum MMouseButton
	{
		NEUTRAL,
		BUTTON1,
		BUTTON2
	};

	enum MMouseEventType
	{
		MOUSE_DOWN,
		MOUSE_UP,
		MOUSE_DOUBLECLICK,
		MOUSE_MOVE,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN
	};

protected:

	MPoint ivPoint;
	MMouseButton ivButton;
	MMouseEventType ivType;

	bool ivShiftPressed;
	bool ivControlPressed;

public:

	MMouseEvent( MMouseButton b, MPoint point, MMouseEventType type, bool shift, bool control );
	virtual ~MMouseEvent();

	virtual const int getX();
	virtual const int getY();
	virtual MPoint getPoint();
	virtual const MMouseButton getButton();
	virtual const MMouseEventType getType();
	virtual const MEventType getEventType() const;
	virtual String toString() const;

	virtual bool getShiftPressed();
	virtual bool getControlPressed();
};

#endif