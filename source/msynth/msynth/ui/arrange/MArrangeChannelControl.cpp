#include "MArrangeChannelControl.h"
#include <gui/dialog/MMessageBox.h>
#include <gui/MGuiUtils.h>
#include "../../../resource.h"
#include <framework/resource/text/MTextResource.h>

INIT_RTTI( MArrangeChannelControl, "MArrangeChannelControl" );
INIT_RTTI( MArrangeChannelControl::MFinalizeListener, "MArrangeChannelControl::MFinalizeListener" );
INIT_RTTI( MArrangeChannelControl::MButtonListener, "MArrangeChannelControl::MButtonListener" );

MArrangeChannelControl::MArrangeChannelControl( MInstrument* pInstrument )
	: MWndCollection(),
	ivPtInstrument( 0 ),
	ivPtEditor( 0 ),
	ivMuteGui( 0 ),
	ivPtButtonEdit( 0 ),
	ivIsSelected( false ),
	ivPtListener( 0 ),
	ivPtButtonListener( 0 )
{
	unsigned int bitmapId = (unsigned int) pInstrument->getBitmapId( 0 );
	ivMuteGui =
		new MBoolGui(
			((MBoolControl*)(pInstrument->getControl( MDefaultMixerChannel::MUTE ))),
			MColorMap::BK_COLOR3 );
	ivMuteGui->setRect( MRect( 0, 0, 35, 26 ) );
	addChild( ivMuteGui );

	ivPtButtonEdit = new MButton(
		bitmapId,
		bitmapId,
		MColorMap::create( 255, 0, 0 ),
		MColorMap::create( 255, 0, 0 ),
		MColorMap::BK_COLOR3 );
	ivPtButtonEdit->setRect( MRect( 0, 0, 26, 26 )  );
	ivPtButtonEdit->setToolTipText( MTextResource::getInstance()->getString("TT_ARRANGE_EDIT_INSTRUMENT_BUTTON") );
	addChild( ivPtButtonEdit );

	setInstrument( pInstrument );

	doLayout();
	updateFromModel();

	ivPtButtonListener = new MButtonListener( this );
}

MArrangeChannelControl::~MArrangeChannelControl()
{
	ivActionListener.clear();

	SAFE_DELETE( ivPtButtonListener );
	SAFE_DELETE( ivPtListener );

	if( ivPtEditor )
	{
		delete ivPtEditor;
		ivPtEditor = 0;
	}

	if( ivPtInstrument )
		ivPtInstrument = 0;
}

void MArrangeChannelControl::doLayout()
{
	if( ivMuteGui &&
		ivPtButtonEdit )
	{
		int cWidth = 35 + 26 + 2;
		int x = getWidth() - cWidth;
		int y = (getHeight()-26) / 2;

		ivMuteGui->setRect(
			MRect(
				x,
				y,
				35,
				26 ) );

		x += 35;
		ivPtButtonEdit->setRect(
			MRect(
				x,
				y,
				26,
				26 ) );
	}
}

void MArrangeChannelControl::paint( IGraphics* ptGraphics, const MRect &rect )
{
	if( ivIsSelected )
		MGuiUtils::paintSunkenBorder(
			ptGraphics,
			MRect( 0, 0, rect.getWidth()-1, rect.getHeight()-1 ),
			ivBkColor );
	else
		MGuiUtils::paintRaisedBorder(
			ptGraphics,
			MRect( 0, 0, rect.getWidth()-1, rect.getHeight()-1 ),
			ivBkColor );

	if( ivPtInstrument )
		ptGraphics->drawText(
			ivPtInstrument->getChannelName(),
			MRect( 5, 2, rect.getWidth() - 10, rect.getHeight() - 4 ),
			ivBkColorSelected );
}

void MArrangeChannelControl::updateFromModel()
{
	ASSERT( ivPtInstrument != 0 );
	repaint();
}

void MArrangeChannelControl::setInstrument( MInstrument *ptInstrument )
{
	if( ivPtInstrument )
		SAFE_DELETE( ivPtListener );

	ivPtInstrument = ptInstrument;

	if( ivPtInstrument )
		ivPtListener = new MFinalizeListener( ivPtInstrument, this );
}

MInstrument *MArrangeChannelControl::getInstrument()
{
	return ivPtInstrument;
}

void MArrangeChannelControl::setSelected( bool isSelected )
{
	ivIsSelected = isSelected;
	repaint();
}

bool MArrangeChannelControl::getSelected()
{
	return ivIsSelected;
}

void MArrangeChannelControl::setBkColorSelected( MColor bkColorSelected )
{
	ivBkColorSelected = bkColorSelected;
}

MColor MArrangeChannelControl::getBkColorSelected()
{
	return ivBkColorSelected;
}

bool MArrangeChannelControl::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::EVENT_MOUSE:
		{
			MMouseEvent* pMouseEvent = (MMouseEvent*) pEvent;
			if( pMouseEvent->getType() == MMouseEvent::MMouseEventType::MOUSE_DOWN &&
				pMouseEvent->getButton() == MMouseEvent::MMouseButton::BUTTON1 )
			{
				unsigned int count = this->ivActionListener.getListenerCount();
				for( unsigned int i=0;i<count;i++ )
					((IActionListener*)ivActionListener.getListener( i ))->onActionPerformed( (void*) this );
			}
		}
		break;
	}
	return MWndCollection::processEvent( pEvent );
}

void MArrangeChannelControl::CloseChildFrame()
{
	if( ivPtEditor )
	{
		MInstrumentEditor* tmp = ivPtEditor;
		ivPtEditor = 0;
		SAFE_DELETE( tmp );
	}
}

void MArrangeChannelControl::StoreEditorPosition()
{
	if( ivPtEditor )
		ivPtEditor->storeWindowPosition();
}

void MArrangeChannelControl::addActionListener( IActionListener* ptListener )
{
	this->ivActionListener.addListener( ptListener );
}

void MArrangeChannelControl::removeActionListener( IActionListener* ptListener )
{
	this->ivActionListener.removeListener( ptListener );
}

MArrangeChannelControl::MFinalizeListener::MFinalizeListener( MInstrument* ptInstrument, MArrangeChannelControl* ptControl ) :
	ivPtInstrument( ptInstrument ),
	ivPtControl( ptControl )
{
	ivPtInstrument->addFinalizeListener( this );
	ivPtInstrument->getControl( MDefaultMixerChannel::NAME )->addControlListener( this );
}

MArrangeChannelControl::MFinalizeListener::~MFinalizeListener()
{
	ivPtInstrument->getControl( MDefaultMixerChannel::NAME )->removeControlListener( this );
	ivPtInstrument->removeFinalizeListener( this );
}

/**
* called if the object is deleted, after recievieing this message,
* the object MUST deregister itself as listener because the observed
* object will be deleted, if not there it will result in an endless loop!!!
*/
void MArrangeChannelControl::MFinalizeListener::objectTerminated( void* obj )
{
	ivPtControl->setInstrument( 0 );
}

void MArrangeChannelControl::MFinalizeListener::valueChanged( MControlListenerEvent *anEvent )
{
	if( anEvent->getSource() == ivPtInstrument->getControl( MDefaultMixerChannel::NAME ) )
		ivPtControl->repaint();
}

void MArrangeChannelControl::onClose( MTopWnd* pWnd )
{
	ivPtEditor = 0;
}

void MArrangeChannelControl::onDestroy( MTopWnd* pWnd )
{
	pWnd->removeListener( this );
	ivPtEditor = 0;
}

MArrangeChannelControl::MButtonListener::MButtonListener( MArrangeChannelControl* ptControl ) :
	ivPtControl( ptControl )
{
	ivPtControl->ivPtButtonEdit->addActionListener( this );
}
MArrangeChannelControl::MButtonListener::~MButtonListener()
{
	ivPtControl->ivPtButtonEdit->removeActionListener( this );
}
void MArrangeChannelControl::MButtonListener::onActionPerformed( void* ptSrc )
{
	if( ivPtControl->ivPtEditor == 0 )
	{
		ivPtControl->ivPtEditor =
			MInstrumentEditorFactory::getInstance()->getEditor(
				ivPtControl->ivPtInstrument->getEditorId() );
		if( ivPtControl->ivPtEditor )
		{
			ivPtControl->ivPtEditor->addListener( ivPtControl );
			ivPtControl->ivPtEditor->setInstrument( ivPtControl->ivPtInstrument );
			ivPtControl->ivPtEditor->create( MRect( 0, 0, 640, 480 ), (MTopWnd*)ivPtControl->getTopParent() );
			ivPtControl->ivPtEditor->setVisible( true );
		}
		else
			MMessageBox::showError( "Could not create editor!" );
	}
	else
	{
		ivPtControl->ivPtEditor->setVisible( true );
	}
}

void MArrangeChannelControl::setBkColor( MColor color )
{
	MWndCollection::setBkColor( color );
	std::vector<MWnd*>::iterator i = ivChilds.begin();
	for( ;i!= ivChilds.end(); ++i )
		(*i)->setBkColor( color );
}