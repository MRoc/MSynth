#include "MFloatEventInspector.h"

INIT_RTTI( MFloatEventInspector, "MFloatEventInspector" );

MFloatEventInspector::MFloatEventInspector() :
	IBarInspector(),
	ivOffsetX( 0 ),
	ivPtBar( 0 ),
	ivPtPartSize( 0 ),
	ivSize( MSize( 0, 0 ) ),
	ivPtContainer( 0 ),
	ivPtCurTarget( 0 )
{
}

MFloatEventInspector::~MFloatEventInspector()
{
	if( ivPtBar )
	{
		ASSERT( ivPtBar->getEventListCollection() );
		ivPtBar->getEventListCollection()->removeListener( this );
		ivPtBar = 0;
	}
}

void MFloatEventInspector::barReleasing()
{
	setBar( 0 );
}

String MFloatEventInspector::getName()
{
	return "floatEvent";
}

void MFloatEventInspector::setBar( MBar* ptBar )
{
	if( ivPtBar )
	{
		ivPtBar->removeBarListener( this );
		ivPtBar->getEventListCollection()->removeListener( this );
	}
	ivPtBar = ptBar;
	if( ivPtBar )
	{
		ivPtBar->addBarListener( this );
		ivPtBar->getEventListCollection()->addListener( this );
	}
}

MBar* MFloatEventInspector::getBar()
{
	return ivPtBar;
}

ToolTipInfo MFloatEventInspector::getToolTip( MPoint point )
{
	ToolTipInfo back;
	back.show = false;
	MFloatEvent* ptEvent = getEvent( point );
	if( ptEvent )
	{
		// set visible flag
		back.show = true;

		// set text
		String str( "" );
		str.Format( "%s %f", getTargetName().getData(), ptEvent->getValue() );
		back.toolTipText = str;

		// set position
		MRect fillRect =
			getBar(
				getIndex( point ),
				ptEvent->getValue() );
		back.toolTipPoint.setX( fillRect.getX() );
		back.toolTipPoint.setY( -20 );
	}
	return back;
}

void MFloatEventInspector::setSize( MSize dimension )
{
	ivSize = dimension;
}

MSize MFloatEventInspector::getSize()
{
	return ivSize;
}

void MFloatEventInspector::setPartSize( MSize* ptSize )
{
	ivPtPartSize = ptSize;
}

MSize* MFloatEventInspector::getPartSize()
{
	return ivPtPartSize;
}

void MFloatEventInspector::setOffsetX( int x )
{
	ivOffsetX = x;
}

int MFloatEventInspector::getOffsetX()
{
	return ivOffsetX;
}

void MFloatEventInspector::paint( IGraphics* ptGraphics, const MRect& rect )
{
	int index = 0;
	IEventListCollection* ptCollection = ivPtBar->getEventListCollection();
	if( ptCollection->hasEventList( ivPtCurTarget ) )
	{
		IAutomationEventList* ptEventList = ptCollection->getEventList( ivPtCurTarget );
		unsigned int count = ptEventList->getLength();
		for( unsigned int i=0;i<count;i++ )
		{
			MFloatEvent* ptEvent = (MFloatEvent*) ptEventList->getItem( i );
			if( ptEvent )
			{	
				FP value = ptEvent->getValue();
				MRect rect = getBar( i, getValue( i ) );
				int col = (int) (255.0f * (1.0f - value));
				MColor barColor = MColorMap::create( col, col, 255 );
				ptGraphics->fillRect( rect, barColor );
				ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
			}
		}
	}
}

bool MFloatEventInspector::onMouseDown( MMouseEvent* pEvent )
{
	bool back = false;
	MFloatEvent *ptEvent = getEvent( pEvent->getPoint() );
	if( ptEvent != 0 )
	{
		ptEvent->setValue(
			FP(ivSize.getHeight() - pEvent->getY() ) / FP(ivSize.getHeight()) );
		back = true;
	}
	return back;
}

bool MFloatEventInspector::onMouseUp( MMouseEvent* pEvent )
{
	return false;
}

bool MFloatEventInspector::onMouseMoved( MMouseEvent* pEvent )
{
	bool back = false;
	MPoint point = pEvent->getPoint();
	MFloatEvent *ptEvent = getEvent( point );
	if( ptEvent )
	{
		if( point.getY() < 0 )
			point.setY( 0 );
		else if( point.getY() > (int)ivSize.getHeight() )
			point.setY( ivSize.getHeight() );
		ptEvent->setValue(
			FP(ivSize.getHeight() - point.getY()) / FP(ivSize.getHeight()) );
		back = true;
	}
	return back;
}

bool MFloatEventInspector::getForwardEvents()
{
	return false;
}

void MFloatEventInspector::release()
{
	delete this;
}

unsigned int MFloatEventInspector::getIndex( MPoint point )
{
	return (point.getX() + ivOffsetX) / ivPtPartSize->getWidth();
}

MFloatEvent* MFloatEventInspector::getEvent( MPoint point )
{
	MFloatEvent* ptBack = 0;

	IEventListCollection* ptELC = ivPtBar->getEventListCollection();
	if( ptELC )
	{
		if( ptELC->hasEventList( ivPtCurTarget ) )
		{
			unsigned int index = getIndex( point );
			if( index < ptELC->getLength() )
			{
				IAutomationEventList* ptEL = ptELC->getEventList( ivPtCurTarget );
				if( ptEL )
					ptBack = (MFloatEvent*) ptEL->getItem( index );
			}
		}
	}
	else
		TRACE( "WARNING: IEventListCollection not set in bar!\n" );

	return ptBack;
}

MRect MFloatEventInspector::getBar( int index, FP value )
{
	ASSERT( value >= 0.0f && value <= 1.0f );
	int x = -ivOffsetX + index * ivPtPartSize->getWidth();
	return
		MRect(
			x,
			int(ivSize.getHeight() *  ( 1.0f - value)),
			ivPtPartSize->getWidth() - 1,
			ivSize.getHeight() );
}

FP MFloatEventInspector::getValue( unsigned int index )
{
	FP back = 0.0f;

	IEventListCollection* ptELC = ivPtBar->getEventListCollection();
	if( ptELC )
	{
		if( ptELC->hasEventList( ivPtCurTarget ) )
		{
			IAutomationEventList* ptEL = ptELC->getEventList( ivPtCurTarget );
			MFloatEvent* ptEvent = (MFloatEvent*) ptEL->getItem( index );
			back = ptEvent->getValue();
		}
	}
	else
		TRACE( "WARNING: IEventListCollection not set in bar!\n" );

	return back;
}

String MFloatEventInspector::getTargetName()
{
	String back("undefined");
	IEventListCollection* ptELC = ivPtBar->getEventListCollection();
	if( ptELC )
	{
		if( ptELC->hasEventList( ivPtCurTarget ) )
			back = ptELC->getEventList( ivPtCurTarget )->getTarget()->getName();
	}
	else
	{
		TRACE( "WARNING: IEventListCollection not set in bar!\n" );
		ASSERT( false );
	}
	return back;
}

void MFloatEventInspector::eventAdded( IAutomationEvent* ptEvent )
{
	if( ivPtContainer )
		ivPtContainer->repaint();
}

void MFloatEventInspector::eventRemoved( IAutomationEvent* ptEvent )
{
	if( ivPtContainer )
		ivPtContainer->repaint();
}

void MFloatEventInspector::allEventsRemoved()
{
	if( ivPtContainer )
		ivPtContainer->repaint();
}

void MFloatEventInspector::eventListAdded( IAutomationEventList* ptList )
{
	if( ivPtContainer )
		ivPtContainer->repaint();
}

void MFloatEventInspector::eventListRemoved( IAutomationEventList* ptList )
{
	if( ivPtContainer )
		ivPtContainer->repaint();
}

void MFloatEventInspector::eventListCollectionReleasing()
{
	if( ivPtBar )
		ivPtBar->getEventListCollection()->removeListener( this );
}

void MFloatEventInspector::setContainingWnd( MWnd* ptContainer )
{
	ivPtContainer = ptContainer;
}

MWnd* MFloatEventInspector::getContainingWnd()
{
	return ivPtContainer;
}

void MFloatEventInspector::setCurrentTarget( IEventTarget* ptTarget )
{
	ivPtCurTarget = ptTarget;
}

IEventTarget* MFloatEventInspector::getCurrentTarget()
{
	return ivPtCurTarget;
}

void MFloatEventInspector::setInspectorInformation( MObject* ptObject )
{
}