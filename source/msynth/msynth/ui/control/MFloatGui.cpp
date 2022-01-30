#include "MFloatGui.h"
#include <gui/MGuiUtils.h>
#include "../../../MSynthConstants.h"
#include <gui/MApp.h>

INIT_RTTI( MFloatGui, "MFloatGui" );

MFloatGui::MFloatGui( MFloatControl* ptFloatControl, MColor bkColor )
	: MControlGui( ptFloatControl, bkColor ),
	ivDrawTransparent( 0 ),
	ivRange( 200 ),
	ivValue( 50 ),
	ivInnerRadius( 10 ),
	ivOuterRadius( 15 )
{
	ASSERT( ivPtControl );

	ivCircleCenter = MPoint(15,15);

	ivBitmapBackGround = new MImage();
	ivBitmapBackGround->loadImage( IDB_BLUE_SCALED );
	
	ivBitmapBackGroundDisabled = new MImage();
	ivBitmapBackGroundDisabled->loadImage( IDB_SKNOB_GRAY );

	ivPtControl = ptFloatControl;

	ivValue = Round( ((MFloatControl*)ivPtControl)->getNormalizedValue() * float(ivRange) );

	ivTextExtend = MFont::getFont()->getTextSize( ivPtControl->getName() );
	short bmWidth = short(ivBitmapBackGround->getWidth());
	short txWidth = short(ivTextExtend.getWidth());
	short width = short((txWidth > bmWidth ? txWidth : bmWidth) + 2);
	short height = short((ivBitmapBackGround->getHeight() + ivTextExtend.getHeight()) + 2);
	ivBitmapOffset = txWidth > bmWidth ? ( txWidth - bmWidth ) / 2 : 0;
	ivTextOffset = txWidth < bmWidth ? ( bmWidth - txWidth ) / 2 : 0;

	setSize( MSize( width, height ) );
}

MFloatGui::MFloatGui( unsigned int bmId, unsigned int bmIdDisabled, MFloatControl* ptFloatControl, MColor bkColor )
	: MControlGui( ptFloatControl, bkColor ),
	ivDrawTransparent( 0 ),
	ivRange( 200 ),
	ivValue( 50 ),
	ivInnerRadius( 10 ),
	ivOuterRadius( 15 )
{
	ASSERT( ivPtControl );

	ivBitmapBackGround = new MImage();
	ivBitmapBackGround->loadImage( bmId );

	ivCircleCenter = MPoint( ivBitmapBackGround->getWidth()/2, ivBitmapBackGround->getHeight()/2 );
	ivInnerRadius = int(0.7f * ivBitmapBackGround->getWidth()/2);
	ivOuterRadius = int(1.0f * ivBitmapBackGround->getWidth()/2);
	
	ivBitmapBackGroundDisabled = new MImage();
	ivBitmapBackGroundDisabled->loadImage( bmIdDisabled );

	ivPtControl = ptFloatControl;

	ivValue = Round( ((MFloatControl*)ivPtControl)->getNormalizedValue() * float(ivRange) );

	ivTextExtend = MFont::getFont()->getTextSize( ivPtControl->getName() );
	short bmWidth = short(ivBitmapBackGround->getWidth());
	short txWidth = short(ivTextExtend.getWidth());
	short width = short((txWidth > bmWidth ? txWidth : bmWidth) + 2);
	short height = short((ivBitmapBackGround->getHeight() + ivTextExtend.getHeight()) + 2);
	ivBitmapOffset = txWidth > bmWidth ? ( txWidth - bmWidth ) / 2 : 0;
	ivTextOffset = txWidth < bmWidth ? ( bmWidth - txWidth ) / 2 : 0;

	setSize( MSize( width, height ) );
}

MFloatGui::~MFloatGui()
{
	SAFE_DELETE( ivBitmapBackGround );
	SAFE_DELETE( ivBitmapBackGroundDisabled );
}

void MFloatGui::onMouseDown( MMouseEvent* ptEvent )
{
	MControlGui::onMouseDown( ptEvent );
	if( getMouseDownLeft() )
		ivLastPoint = MPoint( ptEvent->getX(), ptEvent->getY() );
	onToolTip( true );
}

void MFloatGui::onMouseMove( MMouseEvent* ptEvent )
{
	MControlGui::onMouseMove( ptEvent );

	if( getMouseDownLeft() && (ptEvent->getY() != ivLastPoint.getY() ) )
	{
		ivValue += short( ivLastPoint.getY() - ptEvent->getY() );

		if( ivValue > ivRange )
			ivValue = ivRange;
		else if( ivValue < 0 )
			ivValue = 0;

		ivLastPoint = MPoint( ptEvent->getX(), ptEvent->getY() );
		setControlValue( float(ivValue) / float(ivRange) );
		repaint();
		onToolTip( true );
	}
}

void MFloatGui::onMouseDoubleClick( MMouseEvent* ptEvent )
{
	MControlGui::onMouseDoubleClick( ptEvent );
	if( ptEvent->getButton() == MMouseEvent::BUTTON1 &&
		getEnabled() )
	{
		((MFloatControl*)ivPtControl)->setValue(
			((MFloatControl*)ivPtControl)->getDefaultValue() );
	}
	onToolTip( true );
}

void MFloatGui::setControlValue( FP value )
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

void MFloatGui::paint( IGraphics* ptGraphics, const MRect &rect )
{
	MImage* ptImage = 0;
	if( getEnabled() )
	{
		ptImage = ivBitmapBackGround;
		DrawBitmap( ivBitmapOffset + 1, 1, ptGraphics, ptImage );
		DrawPointer( ivBitmapOffset + 1, 1, ptGraphics );
	}
	else
	{
		ptImage = ivBitmapBackGroundDisabled;
		DrawBitmap( ivBitmapOffset + 1, 1, ptGraphics, ptImage );
	}

	DrawText( ivTextOffset + 1, ptImage->getHeight() + 1, ptGraphics );

	/*if( this->getShowingTooltip() || getInAutomation() )
		paintHighLight( ptGraphics, rect );*/
}

void MFloatGui::DrawText( int x, int y, IGraphics* ptGraphics )
{
	ptGraphics->drawText(
		ivPtControl->getName(),
		MRect( x, y, this->ivTextExtend.getWidth(), ivTextExtend.getHeight() ),
		MColorMap::FG_COLOR3 );
}

void MFloatGui::DrawPointer( int x, int y, IGraphics* ptGraphics )
{
	FP t = ( PI * 1.5f ) / ivRange * ivValue;
	FP alpha1 = (PI * 0.75f) + t;
	FP alpha2 = (PI * 0.75f + 0.5f) + t;
	FP alpha3 = (PI * 0.75f - 0.5f) + t;

	int ll = x + ivCircleCenter.getX();
	int tt = y + ivCircleCenter.getY();
	
	MPoint points[3];
	points[0].setX( ll + long( cos(alpha1) * ivOuterRadius ) );
	points[0].setY( tt + long( sin(alpha1) * ivOuterRadius ) );
	points[1].setX( ll + long( cos(alpha2) * ivInnerRadius ) );
	points[1].setY( tt + long( sin(alpha2) * ivInnerRadius ) );
	points[2].setX( ll + long( cos(alpha3) * ivInnerRadius ) );
	points[2].setY( tt + long( sin(alpha3) * ivInnerRadius ) );
	
	ptGraphics->poly(
		points,
		3,
		MColorMap::FG_COLOR2,
		MColorMap::FG_COLOR3 );
}

void MFloatGui::DrawBitmap( int x, int y, IGraphics* ptGraphics, MImage* ptBitmap )
{
	if( ivDrawTransparent )
		ptGraphics->drawImageTransparent(
			ptBitmap,
			x,
			y,
			MColorMap::create(172,173,172) );
	else
		ptGraphics->drawImage(
			ptBitmap,
			x,
			y );
}

int MFloatGui::Round( float a )
{
	float rest = a - float(floor( a ));
	if( rest > 0.5 )
		return int( ceil( a ) );
	else
		return int( floor( a ) );
}

String	MFloatGui::getToolTipText()
{
	return ((MFloatControl*)ivPtControl)->toDisplayString();
}

bool MFloatGui::onSetCursor()
{
	HCURSOR cursor = LoadCursor( NULL, IDC_SIZENS );
	if( cursor == 0 )
		MLogger::logError( "MFloatGui::onSetCursor: LoadCursor failed" );
	else
		SetCursor( cursor );
	return cursor != 0;
}

void MFloatGui::valueChanged( MControlListenerEvent *anEvent )
{
	switch( anEvent->getEventType() )
	{
	case MControlListenerEvent::CONTROL_EVENT_VALUE_CHANGED:
		MApp::getInstance()->doWorkInGuiThread( this );
		break;
	default: MControlGui::valueChanged( anEvent );
	}
}

String MFloatGui::getClassName()
{
	return "MFloatGui";
}

void MFloatGui::run()
{
	ivValue = Round( ((MFloatControl*)ivPtControl)->getNormalizedValue() * float(ivRange) );
	updateTooltip();
	repaint();
}