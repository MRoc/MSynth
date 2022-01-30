#include "MStringGui.h"
#include <framework/io/MLogger.h>
#include <Richedit.h>

INIT_RTTI( MStringGui, "MStringGui" );


/**
 * constructor
 */
MStringGui::MStringGui( MStringControl* ptControl, MColor bkColor )
	: MControlGui( ptControl, bkColor ),
	ivPtTextField( 0 ),
	ivStarted( false )
{
	ivPtTextField = new MTextField();
	ivPtTextField->setText( ptControl->getValue() );
	ivPtTextField->getModel()->addListener( this );
	addChild( ivPtTextField );
}

/**
 * destructor
 */
MStringGui::~MStringGui()
{
	ASSERT( ivPtControl );

	ivStarted = false;
	ivPtTextField->getModel()->removeListener( this );
	removeChild( ivPtTextField );
	SAFE_DELETE( ivPtTextField );
}

void MStringGui::valueChanged( MControlListenerEvent *anEvent )
{
	switch( anEvent->getEventType() )
	{
	case MControlListenerEvent::CONTROL_EVENT_VALUE_CHANGED:
		if( ivPtTextField->getText() != ((MStringControl*)ivPtControl)->getValue() )
		{
			TRACE( "incoming text: %s\n", ((MStringControl*)ivPtControl)->getValue().getData() );
			ivPtTextField->getModel()->setText( ((MStringControl*)ivPtControl)->getValue() );
		}
		break;
	default:
		MControlGui::valueChanged( anEvent );
		break;
	}
}


/** invoked when textfield model changed */
void MStringGui::onModelChanged( const MTextFieldModelEvent* pEvent )
{
	String text = ivPtTextField->getText();
	if( text != ((MStringControl*)ivPtControl)->getValue() )
		((MStringControl*)ivPtControl)->setValue( ivPtTextField->getText() );
}

void MStringGui::doLayout()
{
	if( ivPtTextField )
		ivPtTextField->setRect( MRect( MPoint(), getRect().getSize() ) );
}

/**
 * enables/disable the window
 */
void MStringGui::setEnabled( bool value )
{
	MControlGui::setEnabled( value );
	if( ivPtTextField )
		ivPtTextField->setEnabled( value );
}

void MStringGui::paint( IGraphics* pG, const MRect& rect )
{
	pG->fillRect( rect, MColorMap::BLACK );
	pG->drawLine( 0, 0, rect.getWidth(), rect.getHeight(), MColorMap::WHITE );
}