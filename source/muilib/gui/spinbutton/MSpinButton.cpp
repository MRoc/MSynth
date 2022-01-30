#include "MSpinButton.h"

#include "../MGuiUtils.h"
#include "../renderer/MArrowRenderer.h"
#include "../button/MButton.h"

INIT_RTTI_CREATE( MSpinButton, "MSpinButton" );

MSpinButton::MSpinButton( Orientation orientation ) :
	MWndCollection(),
	ivOrientation( orientation ),
	ivPtModel( new MSpinButtonModel() ),
	ivState( TS_NEUTRAL ),
	ivDirection( TD_NEUTRAL )
{
	ivPtButtons[ 0 ] = 0;
	ivPtButtons[ 1 ] = 0;
	ivBkColor = MColorMap::BK_COLOR3;

	MArrowRenderer::Direction d1, d2;
	if( orientation == VERTICAL )
	{
		d1 = MArrowRenderer::UP;
		d2 = MArrowRenderer::DOWN;
	}
	else
	{
		d1 = MArrowRenderer::LEFT;
		d2 = MArrowRenderer::RIGHT;
	}
	ivPtButtons[ 0 ] = new MButton( d1, ivBkColor );
	ivPtButtons[ 1 ] = new MButton( d2, ivBkColor );
	ivPtButtons[ 0 ]->addButtonListener( this );
	ivPtButtons[ 1 ]->addButtonListener( this );

	addChild( ivPtButtons[ 0 ] );
	addChild( ivPtButtons[ 1 ] );

	ivTimer.addListener( this );
}

MSpinButton::~MSpinButton(void)
{
	ivPtButtons[ 0 ]->removeButtonListener( this );
	ivPtButtons[ 1 ]->removeButtonListener( this );
	ivTimer.removeListener( this );

	SAFE_DELETE( ivPtModel );
}

MSpinButtonModel* MSpinButton::getModel()
{
	return ivPtModel;
}

void MSpinButton::doLayout()
{
	if(
		ivPtButtons[ 0 ] &&
		ivPtButtons[ 1 ] )
	{
		MRect rect = getRect();
		MRect button1, button2;
		switch( ivOrientation )
		{
			case HORIZONTAL:
				button1 = MRect( 0, 0, rect.getWidth() / 2, rect.getHeight() );
				button2 = MRect( rect.getWidth() / 2, 0, rect.getWidth() / 2, rect.getHeight() );
				break;
			case VERTICAL:
				button1 = MRect( 0, 0, rect.getWidth(), rect.getHeight() / 2 );
				button2 = MRect( 0, rect.getHeight() / 2, rect.getWidth(), rect.getHeight() / 2 );
				break;
			default:
				ASSERT( false );
		}
		ivPtButtons[ 0 ]->setRect( button1 );
		ivPtButtons[ 1 ]->setRect( button2 );
	}
}

void MSpinButton::onButtonPressed( MButton* ptButton )
{
	if( ptButton == ivPtButtons[ 0 ] )
		startTracking( TD_DEC );
	else if( ptButton == ivPtButtons[ 1 ] )
		startTracking( TD_INC );
}

void MSpinButton::onButtonReleased( MButton* ptButton )
{
	stopTracking();
}

void MSpinButton::onActionPerformed( void* ptSrc )
{
	if( ivState == TS_PRE )
	{
		ivTimer.stopTimer();
		ivTimer.startTimer( 200 );
	}

	if( ivDirection == TD_INC )
		ivPtModel->setValue( ivPtModel->getValue() + 1 );
	else if( ivDirection == TD_DEC )
		ivPtModel->setValue( ivPtModel->getValue() - 1 );
}

void MSpinButton::startTracking( TrackingDirection direction )
{
	ivDirection = direction;
	ivState = TS_PRE;
	ivTimer.startTimer( 0 );
}

void MSpinButton::stopTracking()
{
	if( ivTimer.isRunnning() )
		ivTimer.stopTimer();
}