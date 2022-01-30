/**
 * (C)2001-2003 MRoc
 */

#include "MMixerScrollView.h"
#include <framework/performance/MClock.h>

MMixerScrollView::MMixerScrollView() :
	MWndCollection(),
	ivPtMixer( 0 )
{
	ivBkColor = MColorMap::BK_COLOR1;
}

MMixerScrollView::~MMixerScrollView()
{
	deleteControls();
}

void MMixerScrollView::deleteControls()
{
	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++)
	{
		removeChild( ivPtChannelControls[ i ] );
		delete ivPtChannelControls[ i ];
	}
	ivPtChannelControls.clear();
}

void MMixerScrollView::createControl( unsigned int index )
{
	MMixerChannelControl* ptNewControl = new MMixerChannelControl(
		(MDefaultMixerChannel*) ivPtMixer->getMixerChannel(index),
		false );

	ivPtChannelControls.insert(
		ivPtChannelControls.begin() + index,
		ptNewControl );

	ptNewControl->setRect(
		MRect(
			2 + index * (MMixerChannelControl::PREF_WIDTH + 2),
			2,
			MMixerChannelControl::PREF_WIDTH,
			MMixerChannelControl::PREF_HEIGHT )  );
	addChild( ptNewControl );

	ptNewControl->updateFromModel();
}

void MMixerScrollView::createMasterControl()
{
	unsigned int count = ivPtMixer->getChannelCount();
	MMixerChannelControl* ptMasterControl =
		new MMixerChannelControl(
			ivPtMixer,
			true );

	ivPtChannelControls.push_back( ptMasterControl );
	ptMasterControl->setBkColor( MColorMap::BK_COLOR5 );
	ptMasterControl->setRect(
		MRect(
			2 + count * MMixerChannelControl::PREF_WIDTH+2,
			2,
			MMixerChannelControl::PREF_WIDTH,
			MMixerChannelControl::PREF_HEIGHT
			) );
	addChild( ptMasterControl );
	ptMasterControl->updateFromModel();
}

void MMixerScrollView::updateGui()
{
	ASSERT( ivPtMixer != 0 );

	setVisible( false );

	setScrollPos( MPoint(0,0) );
	deleteControls();
	int count = ivPtMixer->getChannelCount();
	for( int i=0;i<count;i++)
		createControl( i ); 
	createMasterControl();

	setVisible( true );
}

void MMixerScrollView::updateFromModel()
{
	MLogger::logMessage( "<mmixerscrollview::updatefrommodel/>\n" );

	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		ivPtChannelControls[i]->updateFromModel();
}

MSize MMixerScrollView::calculateScrollViewSize()
{
	int width = ivPtChannelControls.size() * (MMixerChannelControl::PREF_WIDTH+2) + 2;
	int height = MMixerChannelControl::PREF_HEIGHT + 4;
	return MSize( width, height );
}

void MMixerScrollView::setMixer( MDefaultMixer *ptMixer )
{
	ivPtMixer = ptMixer;
}

MDefaultMixer* MMixerScrollView::getMixer()
{
	return ivPtMixer;
}

void MMixerScrollView::onChannelAdded( unsigned int index )
{
	createControl( index );
	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=index;i<count;i++ )
		layoutControl( i, true );
}

void MMixerScrollView::onChannelRemoved( unsigned int index )
{
	MMixerChannelControl* ptTemp = ivPtChannelControls[ index ];
	ivPtChannelControls.erase( ivPtChannelControls.begin() + index );
	removeChild( ptTemp );
	delete ptTemp;
	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=index;i<count;i++ )
		layoutControl( i, true );
}

void MMixerScrollView::onChannelsSwapped( unsigned int index1, unsigned int index2 )
{
	MMixerChannelControl* ptTemp = ivPtChannelControls[ index1 ];
	ivPtChannelControls[ index1 ] = ivPtChannelControls[ index2 ];
	ivPtChannelControls[ index2 ] = ptTemp;
	layoutControl( index1, true );
	layoutControl( index2, true );
}

void MMixerScrollView::layoutControl( int index, bool redraw )
{
	MPoint curScrPos = getScrollPos();
	MMixerChannelControl* ptControl = ivPtChannelControls[ index ];
	int x = 2 + index * (MMixerChannelControl::PREF_WIDTH+2);
	int y = 2;
	ptControl->setRect( MRect( MPoint( x, y ), ptControl->getSize() ) );
}

void MMixerScrollView::doLayout()
{
	unsigned int count = ivPtChannelControls.size();
	for( unsigned int i=0;i<count;i++ )
		layoutControl( i, false );
	repaint();
}
