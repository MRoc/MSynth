#ifndef __MStringGui
#define __MStringGui

#include <framework/MUtil.h>
#include <gui/textfield/MTextField.h>
#include <gui/textfield/MTextFieldModel.h>
#include "MControlGui.h"
#include "../../model/control/MStringControl.h"

/**
 * implementation of a edit string gui
 */
class MStringGui :
	public MControlGui,
	public ITextFieldModelListener
{
protected:

	/**
	 * the embedded textfield
	 */
	MTextField* ivPtTextField;

	/**
	 * when this flag is true, changes in the embedded
	 * textfield are routed into the string control.
	 */
	bool ivStarted;
	
public:

	/**
	 * constructor
	 */
	MStringGui(  MStringControl* ptControl, MColor bkColor );

	/**
	 * destructor
	 */
	virtual ~MStringGui();

	/**
	 * invoked when string model changed
	 */
	virtual void valueChanged( MControlListenerEvent *anEvent );

	/**
	 * enables/disable the window
	 */
	virtual void setEnabled( bool value );

	virtual void paint( IGraphics* pG, const MRect& rect );

	FULL_RTTI_SUPER( MStringGui, "MStringGui", MControlGui );

protected:

	virtual void doLayout();

	/** invoked when textfield model changed */
	virtual void onModelChanged( const MTextFieldModelEvent* pEvent );
};

#endif