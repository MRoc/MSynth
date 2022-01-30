#ifndef __MArrangeChannelContainer
#define __MArrangeChannelContainer

#include <framework/MTypes.h>
#include <gui/scrollbar/MSimpleScrollWnd.h>
#include "MArrangeChannelControl.h"
#include "../../model/MSong.h"

class MArrangeChannelContainer : public MWndCollection
{
protected:

	class MButtonListener;

	MSong*						ivPtSong;
	MSize						ivPartSize;

	bool						ivMouseDragged;
	int							ivLastIndex;
	MButtonListener*			ivPtButtonListener;

	MArrangeChannelControl*		ivPtSelectedControl;
	std::vector<MArrangeChannelControl*> ivPtChannelControls;

public:

	MArrangeChannelContainer( MSong* ptSong );
	virtual ~MArrangeChannelContainer();

	virtual void	closeChildFrames();
	virtual void	storeChildFramePosition();

	void			builtGui();
	void			updateFromModel();
	void			doLayout();

	void			setSong( MSong *ptSong );
	MSong*			getSong();

	void			SetSelectedChannel( int selectedIndex );
	int				GetSelectedChannel();

	void			setPartSize( MSize size );
	MSize			getPartSize();

	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );

	void			onChannelAdded();
	void			onChannelRemoved( unsigned int index );
	void			onChannelsSwapped( unsigned int index1, unsigned int index2 );

protected:

	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseMove( MMouseEvent* anEvent );

	int				getIndex( MArrangeChannelControl* ptControl );

	void			createChannelAt( unsigned int index );
	void			deleteChannelAt( unsigned int index );

	friend class MButtonListener;
	class MButtonListener :
		public MObject,
		public IActionListener
	{
	protected:
		MArrangeChannelContainer* ivPtControl;
	public:
		FULL_RTTI( MArrangeChannelContainer::MButtonListener, "MArrangeChannelContainer::MButtonListener" );
		MButtonListener( MArrangeChannelContainer* ptControl );
		virtual ~MButtonListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif