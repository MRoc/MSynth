#ifndef __MArrangeChannelControl
#define __MArrangeChannelControl

#include <gui/MWndCollection.h>
#include <gui/button/MButton.h>
#include "../instrument/MInstrumentEditor.h"
#include "../instrument/MInstrumentEditorFactory.h"
#include "../control/MBoolGui.h"
#include "../../model/interfaces/MInstrument.h"

class MArrangeChannelControl :
	public MWndCollection,
	public IFrameWndListener
{
protected:

	class MFinalizeListener;
	class MButtonListener;

	MInstrument*		ivPtInstrument;
	MInstrumentEditor*	ivPtEditor;
	MBoolGui*			ivMuteGui;
	MButton*			ivPtButtonEdit;

	bool				ivIsSelected;

	MColor				ivBkColorSelected;
	MSize				ivPartSize;

	MFinalizeListener*	ivPtListener;
	MButtonListener*	ivPtButtonListener;

	MListenerList		ivActionListener;

public:

	FULL_RTTI_SUPER( MArrangeChannelControl, "MArrangeChannelControl", MWndCollection );

	MArrangeChannelControl( MInstrument* pInstrument );
	virtual ~MArrangeChannelControl();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );

	void				doLayout();

	void				setInstrument( MInstrument* ptChannel );
	MInstrument*		getInstrument();

	void				setSelected( bool isSelected );
	bool				getSelected();

	virtual void		setBkColor( MColor color );

	void				setBkColorSelected( MColor bkColorSelected );
	MColor				getBkColorSelected();

	void				updateFromModel();

	void				CloseChildFrame();
	void				StoreEditorPosition();

	void				addActionListener( IActionListener* ptListener );
	void				removeActionListener( IActionListener* ptListener );

	virtual void onClose( MTopWnd* pWnd );
	virtual void onDestroy( MTopWnd* pWnd );

protected:

	virtual bool processEvent( IUiEvent* pEvent );

	friend class MFinalizeListener;
	class MFinalizeListener :
		public MObject,
		public IFinalizeListener,
		public IControlListener
	{
	protected:
		MInstrument* ivPtInstrument;
		MArrangeChannelControl* ivPtControl;
	public:
		FULL_RTTI( MArrangeChannelControl::MFinalizeListener, "MArrangeChannelControl::MFinalizeListener" );
		MFinalizeListener( MInstrument* ptInstrument, MArrangeChannelControl* ptControl );
		virtual ~MFinalizeListener();
		/**
		 * called if the object is deleted, after recievieing this message,
		 * the object MUST deregister itself as listener because the observed
		 * object will be deleted, if not there it will result in an endless loop!!!
		 **/
		virtual void objectTerminated( void* obj );
		virtual void valueChanged( MControlListenerEvent *anEvent );
	};

	friend class MButtonListener;
	/**
	 * handles incoming events from the editor button
	 */
	class MButtonListener :
		public MObject,
		public IActionListener
	{
	protected:
		MArrangeChannelControl* ivPtControl;
	public:
		FULL_RTTI( MArrangeChannelControl::MButtonListener, "MArrangeChannelControl::MButtonListener" );
		MButtonListener( MArrangeChannelControl* ptControl );
		virtual ~MButtonListener();
		virtual void onActionPerformed( void* ptSrc );
	};
};

#endif
