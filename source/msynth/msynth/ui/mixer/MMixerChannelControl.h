/*

	MMixerChannelControl (C)2001-2002 MRoc hifiShock

*/

#ifndef __MMixerChannelControl
#define __MMixerChannelControl

#include <framework/MUtil.h>

#include <gui/MWndCollection.h>
#include "../../../resource.h"
#include "../../model/mixer/MDefaultMixerChannel.h"
#include "../control/MBoolGui.h"
#include "../control/MFloatGui.h"
#include "../control/MVFloatGui.h"
#include "../control/MStringGui.h"
#include "../peak/MPeakControl.h"

class MMixerChannelControl :
	public MWndCollection
{
public:

	static int			PREF_WIDTH,
						PREF_HEIGHT;

protected:

	class MMixerChannelListener;

	MStringGui*				ivNameGui;
	MBoolGui*				ivMuteGui;
	MVFloatGui*				ivVolGui;
	MFloatGui*				ivPanGui;
	MBoolGui*				ivEqOnGui;
	MFloatGui*				ivHighGui;
	MFloatGui*				ivLowGui;
	MFloatGui*				ivFxGuis[ 4 ];

	MPeakControl*			ivPtPeakControl;

	MDefaultMixerChannel*	ivPtMixerChannel;

	bool					ivIsMasterChannel;

	MMixerChannelListener*	ivPtListener;

public:

	MMixerChannelControl();
	MMixerChannelControl( MDefaultMixerChannel *ptMixerChannel, bool isMaster );
	~MMixerChannelControl();
	virtual void	paint( IGraphics* ptGraphics,const MRect &rect );

	IMixerChannel	*getMixerChannel();
	void			setMixerChannel( MDefaultMixerChannel *ptMixerChannel );

	void			updateFromModel();

	bool			getIsMasterChannel();

	virtual String	toString();
	
protected:

	friend class MMixerChannelListener;
	class MMixerChannelListener :
		public MObject,
		public IFinalizeListener,
		public IControlListener
	{
	protected:
		MMixerChannelControl* ivPtControl;
		MDefaultMixerChannel* ivPtChannel;
	public:
		FULL_RTTI( MMixerChannelControl::MMixerChannelListener, "MMixerChannelControl::MMixerChannelListener" );
		MMixerChannelListener( MMixerChannelControl* ptControl, MDefaultMixerChannel* ptChannel );
		virtual ~MMixerChannelListener();
		/**
		 * called if the object is deleted, after recievieing this message,
		 * the object MUST deregister itself as listener because the observed
		 * object will be deleted, if not there it will result in an endless loop!!!
		 **/
		virtual void objectTerminated( void* obj );

		virtual void valueChanged( MControlListenerEvent *anEvent );
	};
};

#endif

