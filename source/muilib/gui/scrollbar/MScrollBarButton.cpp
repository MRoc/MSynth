#include "MScrollBarButton.h"

/**
 * the sleep time used in the inc/dec thread
 */
const int MScrollBarButton::SLEEP_TIME = 50;

/**
 * the sleep time used after the button
 * was pressed the first time
 */
const int MScrollBarButton::FIRST_SLEEP_TIME = 300;

/**
 * constructor
 */
MScrollBarButton::MScrollBarButton(
	MArrowRenderer::Direction direction,
	ScrollBarButtonType type,
	MScrollBarModel* ptModel )
	:
	MButton( direction, MColorMap::BK_COLOR3 ),
	ivPtModel( ptModel ),
	ivButtonType( type ),
	ivState( NEUTRAL )
{
	ivTimer.addListener( this );
}

/**
 * destructor
 */
MScrollBarButton::~MScrollBarButton()
{
	ivTimer.removeListener( this );
}

/**
 * invoked when the mouse was pressed
 */
void MScrollBarButton::onMouseDown( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseDown( ptEvent );
	if( ! ivTimer.isRunnning() )
	{
		ivState = FIRST;
		ivTimer.startTimer( 0 );
	}
}

/**
 * invoked when the mouse was released
 */
void MScrollBarButton::onMouseUp( MMouseEvent* ptEvent )
{
	MyInteractiveWnd::onMouseUp( ptEvent );
	if( ivTimer.isRunnning() )
		ivTimer.stopTimer();
}

/**
 * does a inc- or dec on the model depending on button's direction
 */
void MScrollBarButton::track()
{
	if( ivButtonType == DECREMENT_BUTTON )
		ivPtModel->dec();
	else if( ivButtonType == INCREMENT_BUTTON )
		ivPtModel->inc();
}

/**
 * the timer callback
 */
void MScrollBarButton::onActionPerformed( void* ptSrc )
{
	switch( ivState )
	{
	case FIRST:
		{
			track();
			ivTimer.stopTimer();
			ivTimer.startTimer( FIRST_SLEEP_TIME );
			ivState = SECOND;
		}
		break;
	case SECOND:
		{
			track();
			ivTimer.stopTimer();
			ivTimer.startTimer( SLEEP_TIME );
			ivState = THIRD;
		}
		break;
	case THIRD:
		{
			track();
		}
		break;
	}
}

/**
 * query interface
 */
void* MScrollBarButton::getInterface( const String &className ) const
{
	if( className == "MScrollBarButton" )
		return (void*) ((MScrollBarButton*)this);
	else if( className == "IActionListener" )
		return (void*) ((IActionListener*)this);
	else
		return MButton::getInterface( className );

}