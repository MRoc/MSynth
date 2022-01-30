#include "MVolumeInspector.h"

MVolumeInspector::MVolumeInspector() :
	ivPtBar( 0 ),
	ivPtPartSize( 0 ),
	ivSize( MSize( 0, 0 ) )
{
}

MVolumeInspector::~MVolumeInspector()
{
}

String MVolumeInspector::getName()
{
	return "Volume";
}

void MVolumeInspector::setBar( MBar* ptBar )
{
	ivPtBar = ptBar;
}

MBar* MVolumeInspector::getBar()
{
	return ivPtBar;
}

ToolTipInfo MVolumeInspector::getToolTip( MPoint point )
{
	ToolTipInfo back;
	back.show = false;
	MDefaultSynthNote* ptNote = getNote( point );
	if( ptNote )
	{
		// set visible flag
		back.show = true;

		// set text
		String str( "" );
		str.Format( "Volume %f", ptNote->getVolume() );
		back.toolTipText = str;

		// set position
		MRect fillRect =
			getBar(
				ptNote->getIndex(),
				ptNote->getVolume() );
		back.toolTipPoint.setX( fillRect.getX() );
		back.toolTipPoint.setY( -20 );
	}
	return back;
}

MDefaultSynthNote* MVolumeInspector::getNote( MPoint point )
{
	int index = point.getX() / ivPtPartSize->getWidth();
	return (MDefaultSynthNote*) ivPtBar->getNoteList()->getItemAt( index );
}

void MVolumeInspector::setSize( MSize dimension )
{
	ivSize = dimension;
}

MSize MVolumeInspector::getSize()
{
	return ivSize;
}

void MVolumeInspector::setPartSize( MSize* ptSize )
{
	ivPtPartSize = ptSize;
}

MSize* MVolumeInspector::getPartSize()
{
	return ivPtPartSize;
}

void MVolumeInspector::release()
{
	delete this;
}

void MVolumeInspector::paint( IGraphics* ptGraphics, const MRect& rect )
{
	MDefaultSynthNote *ptNote =
		(MDefaultSynthNote*) ivPtBar->getNoteList()->getFirstItem();
	while( ptNote != 0 )
	{
		MRect fillRect = getBar( ptNote->getIndex(), ptNote->getVolume() );

		int col = 255 - ((int)(ptNote->getVolume() * 255.0f));
		MColor barColor = MColorMap::create( col, col, 255 );
		ptGraphics->fillRect( fillRect, barColor );
		ptGraphics->drawRect( fillRect, MColorMap::FG_COLOR3 );

		ptNote = (MDefaultSynthNote*) ptNote->getNext();
	}
}

//-----------------------------------------------------------------------------
// # GetBar
// returns the rect starting at index x and with the
// height of the window multiplied with the given value
//-----------------------------------------------------------------------------
MRect MVolumeInspector::getBar( int index, FP value )
{
	ASSERT( value >= 0.0f && value <= 1.0f );
	int x = index * ivPtPartSize->getWidth();
	return
		MRect(
			x,
			int(ivSize.getHeight() *  ( 1.0f - value)),
			ivPtPartSize->getWidth() - 1,
			ivSize.getHeight() );
}

bool MVolumeInspector::onMouseDown( MMouseEvent* pEvent )
{
	bool back = false;
	MDefaultSynthNote *ptNote = (MDefaultSynthNote*) getNote( pEvent->getPoint() );
	if( ptNote != 0 )
	{
		FP volume = FP(ivSize.getHeight() - pEvent->getY()) / FP(ivSize.getHeight());
		ptNote->setVolume( volume );
		back = true;
	}
	return back;
}

bool MVolumeInspector::onMouseUp( MMouseEvent* pEvent )
{
	return false;
}

bool MVolumeInspector::onMouseMoved( MMouseEvent* pEvent )
{
	bool back = false;
	MPoint point = pEvent->getPoint();
	MDefaultSynthNote* ptNote = getNote( point );
	if( ptNote )
	{
		if( point.getY() < 0 )
			point.setY( 0 );
		else if( point.getY() > (int)ivSize.getHeight() )
			point.setY( ivSize.getHeight() );
		FP volume = FP(ivSize.getHeight() - point.getY()) / FP(ivSize.getHeight());
		ptNote->setVolume( volume );
		back = true;
	}
	return back;
}

bool MVolumeInspector::getForwardEvents()
{
	return true;
}

void MVolumeInspector::setContainingWnd( MWnd* ptContainer )
{
	ivPtContainer = ptContainer;
}

MWnd* MVolumeInspector::getContainingWnd()
{
	return ivPtContainer;
}

void MVolumeInspector::setInspectorInformation( MObject* ptObject )
{
}