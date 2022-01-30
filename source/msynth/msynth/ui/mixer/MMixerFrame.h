/*

	MMixerFrame (C)2001 MRoc hifiShock

*/

#ifndef __MMixerFrame
#define __MMixerFrame

#include "MMixerScrollView.h"
#include "MMixerChannelControl.h"

#include "../../../resource.h"
#include <gui/scrollbar/MScrollPane.h>
#include <gui/framewnd/MScrolledFrameWnd.h>

#include "../../model/Sequencer.h"
#include "../../model/mixer/MDefaultMixerChannel.h"
#include "../../model/mixer/IMixer.h"

class MMixerFrame : public MScrolledFrameWnd
{
protected:

	class MMixerListener;
	
	MDefaultMixer*		ivPtMixer;
	MMixerScrollView*	ivPtScrollView;
	MMixerListener*		ivPtListener;

public:

	MMixerFrame();
	virtual ~MMixerFrame();

	virtual bool	create( MRect rect, MTopWnd *parentWnd );

	void			setMixer( MDefaultMixer *ptMixer );
	MDefaultMixer*	getMixer();

	void			updateGui();
	void			updateFromModel();
	
	virtual void	doLayout();

protected:

	void			autoSize();					// Sizes the Frame to the best SIZE
	
	friend class MMixerListener;
	class MMixerListener :
		public MObject,
		public IMixerListener
	{
	protected:

		MDefaultMixer* ivPtMixer;
		MMixerFrame* ivPtEditor;

	public:

		FULL_RTTI( MMixerFrame::MMixerListener, "MMixerFrame::MMixerListener" );
		MMixerListener( MDefaultMixer* ptMixer, MMixerFrame* ptEditor );
		virtual ~MMixerListener();

		virtual void onMixerModelChanged();
		virtual void onMixerChannelAdded( unsigned int index );
		virtual void onMixerChannelRemoved( unsigned int index );
		virtual void onMixerChannelSwapped( unsigned int index1, unsigned int index2 );
	};

};

#endif
