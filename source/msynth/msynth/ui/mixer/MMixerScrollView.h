/*

	MMixerScrollView (C)2001 MRoc hifiShock

*/


#ifndef __MMixerScrollView
#define __MMixerScrollView

#include <gui/MWndCollection.h>
#include "MMixerChannelControl.h"
#include "../../model/mixer/MDefaultMixer.h"

typedef std::vector<MMixerChannelControl*> MixerControlList;

class MMixerScrollView :
	public MWndCollection
{
protected:

	MDefaultMixer*		ivPtMixer;
	MixerControlList	ivPtChannelControls;

public:

	MMixerScrollView();
	virtual ~MMixerScrollView();

	void				deleteControls();

	void				setMixer( MDefaultMixer *ptMixer );
	MDefaultMixer*		getMixer();

	MSize				calculateScrollViewSize();

	void				updateGui();
	void				updateFromModel();
	void				onChannelAdded( unsigned int index );
	void				onChannelRemoved( unsigned int index );
	void				onChannelsSwapped( unsigned int index1, unsigned int index2 );

	virtual void		doLayout();

protected:
	
	void				layoutControl( int index, bool redraw );

	void				createControl( unsigned int index );
	void				createMasterControl();
};

#endif
