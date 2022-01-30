/*

	MInstrumentEditor (C)2001 MRoc hifiShock

*/

#ifndef __MInstrumentEditor
#define __MInstrumentEditor

#include <framework/primitive/MRect.h>
#include <framework/primitive/MPoint.h>

#include "../../../MAppData.h"
#include <gui/framewnd/MFrameWnd.h>
#include "../../model/interfaces/MInstrument.h"

class MInstrumentEditor :
	public MFrameWnd
{
protected:
	class MInstrumentListener;

	MInstrument*			ivPtInstrument;
	MInstrumentListener*	ivPtListener;

public:

	MInstrumentEditor();
	virtual ~MInstrumentEditor();

	virtual bool			create( MRect rect, MTopWnd *parentWnd );

	virtual void			doLayout();
	virtual void			doLayout( bool autoSize ) = 0;
	virtual void			updateFromModel();

	virtual void			setInstrument( MInstrument* ptInstrument );
	virtual MInstrument*	getInstrument();

	virtual void			setVisible( bool value );

protected:

	/**
	 * handles incoming name changes from the instruments name control.
	 * also handles finalisation events.
	 */
	class MInstrumentListener :
		public MObject,
		public IFinalizeListener,
		public IControlListener
	{
	protected:
		MInstrument* ivPtInstrument;
		MInstrumentEditor* ivPtEditor;
	public:
		FULL_RTTI( MInstrumentEditor::MInstrumentListener, "MInstrumentEditor::MInstrumentListener" );
		MInstrumentListener( MInstrument* ptInstrument, MInstrumentEditor* ptEditor );
		virtual ~MInstrumentListener();

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