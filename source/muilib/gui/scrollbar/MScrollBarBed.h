#ifndef __MScrollBarBed
#define __MScrollBarBed

#include "../MyInteractiveWnd.h"
#include "MScrollBarModel.h"
#include "../timer/MTimer.h"

class MScrollBarBed :
	public MyInteractiveWnd,
	public IActionListener,
	public IScrollBarListener
{
public:

    enum ScrollBarType
	{
		VERTICAL,
		HORIZONTAL
	};

	enum TrackingType
	{
		NO_TRACKING,
		INCREMENTING,
		DECREMENTING
	};

	enum Component
	{
		NO_COMPONENT,
		BED,
		SLIDER
	};

protected:

	/**
	 * the sleep time used in the inc/dec thread
	 */
	static const int SLEEP_TIME;

	MScrollBarModel* ivPtModel;
	ScrollBarType ivType;
	TrackingType ivTrackingMode;

	MRect ivSlider;
	MPoint ivButtonDownPoint;
	int ivLastSliderPosition;
	Component ivCurrentComponent;

	MTimer ivTimer;

public:

	FULL_RTTI_SUPER( MScrollBarBed, "MScrollBarBed", MyInteractiveWnd );

	MScrollBarBed( MScrollBarModel* ptModel, ScrollBarType type );
	virtual ~MScrollBarBed();

	virtual void doLayout();
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

protected:

	bool updateMouseComponent( MPoint p );

	virtual void onMouseDown( MMouseEvent* ptEvent );
	virtual void onMouseUp( MMouseEvent* ptEvent );
	virtual void onMouseMove( MMouseEvent* ptEvent );
	virtual void onMouseHover( MHoverEvent* anEvent );
	virtual void onMouseLeave( MHoverEvent* anEvent );

	virtual void valueChanged( void* source );
	virtual void onActionPerformed( void* ptSrc );
};
#endif