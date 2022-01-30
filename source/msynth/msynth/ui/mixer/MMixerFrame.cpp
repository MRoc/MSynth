/*

	MMixerFrame (C)2001-2002 MRoc hifiShock

*/

#include "MMixerFrame.h"

INIT_RTTI( MMixerFrame::MMixerListener, "MMixerFrame::MMixerListener" );

MMixerFrame::MMixerFrame() :
	MScrolledFrameWnd(),
	ivPtMixer( 0 ),
	ivPtScrollView( 0 ),
	ivPtListener( 0 )
{
	MLogger::logMessage( "<mmixerframe::mmixerframe/>\n" );

	ivPtScrollView = new MMixerScrollView();
	ivPtProperties = new MProperties();

	setScrollWnd( ivPtScrollView );
	setActionOnClose( MFrameWnd::HIDE );
}

MMixerFrame::~MMixerFrame()
{
	MLogger::logMessage( "<mmixerframe::~mmixerframe/>\n" );
	setMixer( 0 );
	delete ivPtProperties;
}

void MMixerFrame::setMixer( MDefaultMixer *ptMixer )
{
	if( ivPtMixer )
		SAFE_DELETE( ivPtListener );

	ivPtMixer = ptMixer;
	ivPtScrollView->setMixer( ptMixer );

	if( ivPtMixer )
		ivPtListener = new MMixerListener( ivPtMixer, this );
}

MDefaultMixer* MMixerFrame::getMixer()
{
	return ivPtMixer;
}

bool MMixerFrame::create( MRect rect, MTopWnd *parentWnd )
{
	MLogger::logMessage( "<mmixerframe::create/>\n" );

	bool back =	MScrolledFrameWnd::create( rect, parentWnd );
	if( back )
	{
		setText( "Mixer" );
		autoSize();
	}
	return back;
}

void MMixerFrame::updateGui()
{
	MLogger::logMessage( "<mmixerframe::updategui/>\n" );
	ivPtScrollView->updateGui();
	autoSize();
}

void MMixerFrame::updateFromModel()
{
	MLogger::logMessage( "<mmixerframe::updatefrommodel/>\n" );
	ivPtScrollView->updateFromModel();
}

void MMixerFrame::doLayout()
{
	MScrolledFrameWnd::doLayout();
	if( getChildCount() > 0 )
		((MScrollPane*)getChild(0))->setScrollSize( ivPtScrollView->calculateScrollViewSize() );
}


void MMixerFrame::autoSize()
{
	MSize scrollViewSize = ivPtScrollView->calculateScrollViewSize();

	setMinimumSize( MSize( 0, scrollViewSize.getHeight() + 16 ) );
	setMaximumSize( MSize( scrollViewSize.getWidth(), scrollViewSize.getHeight() + 16 ) );
	setPreferredSize( MSize( scrollViewSize.getWidth(), scrollViewSize.getHeight() + 16 ) );
}

MMixerFrame::MMixerListener::MMixerListener( MDefaultMixer* ptMixer, MMixerFrame* ptEditor ) :
	ivPtMixer( ptMixer ),
	ivPtEditor( ptEditor )
{
	ivPtMixer->addMixerListener( this );
}

MMixerFrame::MMixerListener::~MMixerListener()
{
	ivPtMixer->removeMixerListener( this );
}

void MMixerFrame::MMixerListener::onMixerModelChanged()
{
	ivPtEditor->updateGui();
	ivPtEditor->updateFromModel();
}

void MMixerFrame::MMixerListener::onMixerChannelAdded( unsigned int index )
{
	ivPtEditor->ivPtScrollView->onChannelAdded( index );
	ivPtEditor->autoSize();
}

void MMixerFrame::MMixerListener::onMixerChannelRemoved( unsigned int index )
{
	ivPtEditor->ivPtScrollView->onChannelRemoved( index );
	ivPtEditor->autoSize();
}

void MMixerFrame::MMixerListener::onMixerChannelSwapped( unsigned int index1, unsigned int index2 )
{
	ivPtEditor->ivPtScrollView->onChannelsSwapped( index1, index2 );
}