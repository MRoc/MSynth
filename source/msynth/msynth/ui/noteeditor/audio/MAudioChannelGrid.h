#ifndef __MAudioChannelGrid
#define __MAudioChannelGrid

#include "MAudioTriggerControl.h"
#include "../../dcgrid/MDCGrid.h"
#include "../../../model/audiochannel/MAudioChannel.h"
#include "../../../model/audiochannel/MAudioTriggerList.h"

class MAudioChannelGrid :
	public MDCGrid
{
protected:

	class BpmListener;

	MAudioChannel* ivPtAudioChannel;

	MAudioTriggerList* ivPtTriggers;

	IBpmControl* ivPtBpmControl;

	BpmListener* ivPtBpmListener;

public:

	MAudioChannelGrid( MSize* ptPartSize );
	virtual ~MAudioChannelGrid();

	virtual void	setBpmControl( IBpmControl* ptControl );
	virtual IBpmControl* getBpmControl();

	virtual FP		getPeaksPerPixel();
	virtual void	updateControlPeaksPerPixel();

	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void	updateFromModel();
	virtual void	copy();
	virtual void	paste( int index );
	virtual void	deleteSelected();

	virtual void	setData( MAudioChannel* ptChannel, MAudioTriggerList* ptTriggers );

	virtual void	onMouseDown( MMouseEvent* anEvent );
	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseMove( MMouseEvent* anEvent );

	virtual bool	onSetCursor();

protected:

	virtual void	doLayout();

	virtual int		getMaxRows();
	virtual int		getMaxCols();

	virtual int		getRow( MDCControl* ptControl );
	virtual int		getCol( MDCControl* ptControl );

	virtual int		getColSpan( MDCControl* ptControl );

	void moveControl( MDCControl* _ptControl, int colDiv );

	virtual void	startControlEditing( MDCControl* ptControl );
	virtual bool	updateEditing( MPoint point );
	virtual void	stopControlMoving( MPoint point );
	virtual MDCControl*	createControl( MPoint point );
	virtual MDCControl* createControl( MAudioTrigger* ptTrigger );
	virtual void		deleteControl( MDCControl* ptControl );

	class BpmListener : public IBpmListener
	{
	protected:

		MAudioChannelGrid* ivPtGrid;

	public:

		BpmListener( MAudioChannelGrid* ptGrid );
		virtual ~BpmListener();

		virtual void bpmChanged( unsigned int bpm );
	};
};

#endif