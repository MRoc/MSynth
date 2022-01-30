/*

	MInstrumentEditor (C)2001-2002 MRoc hifiShock

*/

#include "MInstrumentEditor.h"
#include <gui/MApp.h>

INIT_RTTI( MInstrumentEditor::MInstrumentListener, "MInstrumentEditor::MInstrumentListener" );

MInstrumentEditor::MInstrumentEditor() :
	ivPtInstrument( 0 ),
	ivPtListener( 0 )
{
	setActionOnClose( MFrameWnd::DESTROY );
}

MInstrumentEditor::~MInstrumentEditor()
{
	SAFE_DELETE( ivPtListener );
}

bool MInstrumentEditor::create( MRect rect, MTopWnd *parentWnd )
{
	// try to restore window position
	MRect rRect = rect;
	if( ivPtProperties )
	{
		MRect* ptRect = (MRect*) ivPtProperties->get( "windowposition" );
		if( ptRect )
			rRect = *ptRect;
	}

	return MFrameWnd::create(
			rRect,
			MApp::getInstance()->getMainWnd() );
}

void MInstrumentEditor::doLayout()
{
	MFrameWnd::doLayout();
	doLayout( false );
}

void MInstrumentEditor::updateFromModel()
{
	setText( ivPtInstrument->getChannelName() );
}

void MInstrumentEditor::setVisible( bool value )
{
	if( ivPtInstrument )
		setProperties( ivPtInstrument->getProperties() );
	updateFromModel();
	MFrameWnd::setVisible( value );
}

void MInstrumentEditor::setInstrument( MInstrument* ptInstrument )
{
	if( ivPtInstrument )
		SAFE_DELETE( ivPtListener );
	ivPtInstrument = ptInstrument;
	if( ivPtInstrument )
		ivPtListener = new MInstrumentListener( ivPtInstrument, this );
}

MInstrument* MInstrumentEditor::getInstrument()
{
	return ivPtInstrument;
}

MInstrumentEditor::MInstrumentListener::MInstrumentListener( MInstrument* ptInstrument, MInstrumentEditor* ptEditor ) :
	ivPtInstrument( ptInstrument ),
	ivPtEditor( ptEditor )
{
	ivPtInstrument->addFinalizeListener( this );
	((MStringControl*)ivPtInstrument->getControl( MDefaultMixerChannel::NAME ))->addControlListener( this );
}

MInstrumentEditor::MInstrumentListener::~MInstrumentListener()
{
	ivPtInstrument->removeFinalizeListener( this );
	((MStringControl*)ivPtInstrument->getControl( MDefaultMixerChannel::NAME ))->removeControlListener( this );
}

/**
 * called if the object is deleted, after recievieing this message,
 * the object MUST deregister itself as listener because the observed
 * object will be deleted, if not there it will result in an endless loop!!!
 */
void MInstrumentEditor::MInstrumentListener::objectTerminated( void* obj )
{
	ASSERT( ((MInstrument*)obj) == ivPtInstrument );
	ivPtEditor->setInstrument( 0 );
}

void MInstrumentEditor::MInstrumentListener::valueChanged( MControlListenerEvent *anEvent )
{
	if( anEvent->getSource() == ivPtInstrument->getControl( MDefaultMixerChannel::NAME ) )
		ivPtEditor->setText( ((MStringControl*)ivPtInstrument->getControl( MDefaultMixerChannel::NAME ))->getValue() );
}