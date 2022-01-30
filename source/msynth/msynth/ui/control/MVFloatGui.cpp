#include "MVFloatGui.h"
#include <gui/MApp.h>

INIT_RTTI( MVFloatGui::MControlListener, "MVFloatGui::MControlListener" );

MVFloatGui::MVFloatGui( MFloatControl* ptFloatControl, MColor bkColor )
	: MControlGui( ptFloatControl, bkColor ),
	ivRange( 100 ),
	ivValue( 50 ),
	ivPtBkBitmap( 0 ),
	ivPtKnobBitmap( 0 ),
	ivPtListener( 0 )
{
	ASSERT( ivPtControl );

	ivPtBkBitmap = new MImage();
	ivPtBkBitmap->loadImage( IDB_MIXER_LINE );
	
	ivPtKnobBitmap = new MImage();
	ivPtKnobBitmap->loadImage( IDB_MIXER_KNOB );

	ivRange = ivPtBkBitmap->getHeight() - ivPtKnobBitmap->getHeight();

	ivPtControl = ptFloatControl;

	ivValue = Round( ((MFloatControl*)ivPtControl)->getNormalizedValue() * float(ivRange) );

	int height = ivPtBkBitmap->getHeight();
	int width = ivPtBkBitmap->getWidth() > ivPtKnobBitmap->getWidth() ? 
				ivPtBkBitmap->getWidth() : ivPtKnobBitmap->getWidth();
	setSize( MSize( width+2, height+2 ) );

	ivPtListener = new MControlListener( this );
}

MVFloatGui::MVFloatGui( unsigned int bmId, unsigned int bmIdDisabled, MFloatControl* ptFloatControl, MColor bkColor )
	: MControlGui( ptFloatControl, bkColor ),
	ivRange( 100 ),
	ivValue( 50 ),
	ivPtBkBitmap( 0 ),
	ivPtKnobBitmap( 0 )
{
	ASSERT( ivPtControl );

	ivPtBkBitmap = new MImage();
	ivPtBkBitmap->loadImage( bmId );
	
	ivPtKnobBitmap = new MImage();
	ivPtKnobBitmap->loadImage( bmIdDisabled );

	ivRange = ivPtBkBitmap->getHeight() - ivPtKnobBitmap->getHeight();

	ivPtControl = ptFloatControl;

	ivValue = Round( ((MFloatControl*)ivPtControl)->getNormalizedValue() * float(ivRange) );

	int height = ivPtBkBitmap->getHeight();
	int width = ivPtBkBitmap->getWidth() > ivPtKnobBitmap->getWidth() ? 
				ivPtBkBitmap->getWidth() : ivPtKnobBitmap->getWidth();
	setSize( MSize( width+2, height+2 ) );

	ivPtListener = new MControlListener( this );
}

MVFloatGui::~MVFloatGui()
{
	SAFE_DELETE( ivPtListener );
	SAFE_DELETE( ivPtBkBitmap );
	SAFE_DELETE( ivPtKnobBitmap );
}

void MVFloatGui::onMouseDown( MMouseEvent* ptEvent )
{
	MControlGui::onMouseDown( ptEvent );
	ivLastPoint = ptEvent->getPoint();
}

void MVFloatGui::onMouseMove( MMouseEvent* ptEvent )
{
	MControlGui::onMouseMove( ptEvent );
	if( getMouseDownLeft() && (ptEvent->getY() != ivLastPoint.getY()) )
	{
		ivValue += ( ivLastPoint.getY() - ptEvent->getY() );

		if( ivValue > ivRange )
			ivValue = ivRange;
		else if( ivValue < 0 )
			ivValue = 0;

		ivLastPoint = ptEvent->getPoint();
		setControlValue( float(ivValue) / float(ivRange) );
		repaint();
		onToolTip( true );
	}
}

void MVFloatGui::onMouseDoubleClick( MMouseEvent* ptEvent )
{
	MControlGui::onMouseDoubleClick( ptEvent );
	if( ptEvent->getButton() == MMouseEvent::BUTTON1 &&
		getEnabled() )
	{
		((MFloatControl*)ivPtControl)->setValue( ((MFloatControl*)ivPtControl)->getDefaultValue() );
	}
	onToolTip( true );
}

void MVFloatGui::setControlValue( FP value )
{
	((MFloatControl*)ivPtControl)->setNormalizedValue( value );

	IEventTarget* pTarget = (IEventTarget*) ivPtControl->getInterface( "IEventTarget" );
	MTargetRegistry* ptReg = MTargetRegistry::getInstance();
	if( ptReg->getRecordMode() == MTargetRegistry::RECORD_EVENTS &&
		ptReg->getContext( pTarget ) )
	{
		if( ! ptReg->isInAutomation( pTarget ) )
			ptReg->addToAutomation( pTarget );

		IAutomationEvent* ptEvent =
			((MFloatControl*)ivPtControl)->createEvent();
		ASSERT( ptEvent );

		IAutomationContext* ptContext =
			ptReg->getContext( pTarget );

		if( ptContext )
		{
			IAutomationTracker* ptTracker =
				ptContext->getTracker();
	
			ptTracker->addAutomationEvent(
				ptEvent,
				pTarget,
				MTimeController::getInstance()->getCurrentPattern(),
				MTimeController::getInstance()->getCurrentSystemStep() );
		}
		else
		{
			TRACE( "[no control context found to record event!]\n" );
			ptEvent->release();
		}
	}
}

void MVFloatGui::paint( IGraphics* ptGraphics, const MRect &rect )
{
	int width = rect.getWidth();
	int bkOffsetX = (width - ivPtBkBitmap->getWidth()) / 2;
	ptGraphics->drawImage(
		ivPtBkBitmap,
		bkOffsetX,
		1 );

	if( getEnabled() )
	{
		int knobOffsetX = ( width - ivPtKnobBitmap->getWidth()) / 2;
		int knobOffsetY = 1 + (( ivPtBkBitmap->getHeight() + ivRange - ivPtKnobBitmap->getHeight() ) / 2 - ivValue );
		ptGraphics->drawImage(
			ivPtKnobBitmap,
			knobOffsetX,
			knobOffsetY );
	}

	if( getIsHovering() || getCapturing() || getInAutomation() )
		paintHighLight( ptGraphics, rect );
}

int MVFloatGui::Round( float a )
{
	float rest = a - float(floor( a ));
	if( rest > 0.5 )
		return int( ceil( a ) );
	else
		return int( floor( a ) );
}

String	MVFloatGui::getToolTipText()
{
	return ((MFloatControl*)ivPtControl)->toDisplayString();
}

bool MVFloatGui::onSetCursor()
{
	HCURSOR cursor = LoadCursor( NULL, IDC_SIZENS );
	if( cursor == 0 )
		MLogger::logError( "MVFloatGui::onSetCursor: LoadCursor failed" );
	else
		SetCursor( cursor );
	return cursor != 0;
}

MVFloatGui::MControlListener::MControlListener( MVFloatGui* ptGui ) :
	ivPtGui( ptGui )
{
	ivPtGui->ivPtControl->addControlListener( this );
}

MVFloatGui::MControlListener::~MControlListener()
{
	ivPtGui->ivPtControl->removeControlListener( this );
}

void MVFloatGui::MControlListener::valueChanged( MControlListenerEvent *anEvent )
{
	MApp::getInstance()->doWorkInGuiThread( this );
}

void MVFloatGui::MControlListener::run()
{
	ivPtGui->ivValue =
		ivPtGui->Round(
			((MFloatControl*)ivPtGui->ivPtControl)->getNormalizedValue() * float(ivPtGui->ivRange) );
	ivPtGui->updateTooltip();
	ivPtGui->repaint();
}