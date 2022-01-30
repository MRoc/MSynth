/*

	MDCControl (C)2001-2003 MRoc hifiShock

	Represents a abstract control not inheritanced
	from CWnd, this control needs a given device-context
	to paint.

	It provides methods for the 2 backgroundcolors,
	selectioning and positioning.

*/

#ifndef __MDCControl
#define __MDCControl

#include <framework/MObject.h>
#include <framework/primitive/MRect.h>
#include <gui/IGraphics.h>

class MDCControl :
	public MObject
{
public:

	static MRtti gvRtti;

protected:

	static const int HOVER;
	static const int SELECTED;

	unsigned int		ivState;

	MRect				ivRect;
	MColor				ivBkColor;
	MColor				ivBkColorSelected;

	MPoint				ivPoint;

public:

	MDCControl();
	virtual ~MDCControl();

	virtual IRtti*		getRtti() const;
	virtual void*		getInterface( const String &className ) const;

	virtual void		setSelected( bool isSelected );
	virtual bool		getSelected();

	virtual void		setHover( bool isHover );
	virtual bool		getHover();

	virtual void		setPosition( MRect rect );
	virtual MRect		getPosition();
	virtual void		moveControl( MPoint point );

	virtual void		setBkColor( MColor bkColor );
	virtual MColor		getBkColor();
	virtual void		setBkColorSelected( MColor bkColorSelected );
	virtual MColor		getBkColorSelected();

	virtual void		setPoint( MPoint point );
	virtual MPoint		getPoint();

	virtual void		paintControl( IGraphics* ptGraphics ) = 0;
};

class MDCControlComparator
{
public:

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 ) = 0;
};

class MDefaultDCComparator : public MDCControlComparator
{
public:

	enum SortType
	{
		STD_LO_PRIOR,
		STD_HI_PRIOR,
		REV_LO_PRIOR,
		REV_HI_PRIOR
	};

protected:

	SortType ivSortMode;

public:

	MDefaultDCComparator( SortType type ) :
		ivSortMode( type )
	{
	}

	virtual ~MDefaultDCComparator()
	{
	}

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 ) = 0;
};

#endif