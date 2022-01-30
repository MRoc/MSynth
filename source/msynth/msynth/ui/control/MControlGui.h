/*

MControlGui (C)2000-2002 MRoc hifiShock

*/

#ifndef __MControlGui
#define __MControlGui

#include <gui/MWndCollection.h>
#include <gui/MImage.h>
#include <gui/tooltip/MToolTipCtrl.h>
#include <gui/event/MMouseEvent.h>
#include <gui/event/MHoverEvent.h>
#include <gui/event/MKeyEvent.h>
#include <gui/event/MFocusEvent.h>
#include "../../../resource.h"
#include "../../model/control/MDefaultControl.h"
#include "../../model/automation/IEventTarget.h"

class MControlGui :
	public MWndCollection,
	public IControlListener,
	public IEventTargetListener
{
private:

	static const unsigned int INAUTOMATION;

	static const unsigned int TOOL_TIP_TIME;
	static const unsigned int MOUSE_IN;
	static const unsigned int MOUSE_DOWN_LEFT;
	static const unsigned int MOUSE_DOWN_RIGHT;
	static const unsigned int CAPTURING;
	static const unsigned int TOOLTIP;

	unsigned int ivBitState;
	String ivToolTip;

protected:

	IControl* ivPtControl;

public:

	FULL_RTTI_SUPER( MControlGui, "MControlGui", MWndCollection );

	MControlGui( IControl* ptControl, MColor bkColor );
	virtual ~MControlGui();

	virtual void setEnabled( bool bEnable );

	virtual void valueChanged( MControlListenerEvent *anEvent );
	virtual void onEventTargetChange( IEventTarget* pTarget, Type type );


	virtual String getToolTipText();
	virtual void setToolTipText( String toolTip );

	virtual bool getMouseIn() const;
	virtual bool getMouseDownLeft() const;
	virtual bool getMouseDownRight() const;
	virtual bool getMouseDown( MMouseEvent::MMouseButton button ) const;
	virtual bool getCapturing() const;
	virtual bool getShowingTooltip() const;
	virtual bool getIsHovering() const;

protected:

	// bit state functions
	virtual void setMouseIn( bool value );
	virtual void setMouseDownLeft( bool down );
	virtual void setMouseDownRight( bool down );
	virtual void setMouseDown( MMouseEvent::MMouseButton button, bool value );
	virtual void setCapturing( bool value );
	virtual void setShowingTooltip( bool value );

	// event functions
	virtual void onToolTip( bool value);

	virtual void onKeyDown( MKeyEvent* anEvent );
	virtual void onKeyUp( MKeyEvent* anEvent );

	virtual void onMouseDown( MMouseEvent* anEvent );
	virtual void onMouseUp( MMouseEvent* anEvent );
	virtual void onMouseDoubleClick( MMouseEvent* anEvent );
	virtual void onMouseMove( MMouseEvent* anEvent );
	virtual void onMouseHover( MHoverEvent* anEvent );
	virtual void onMouseLeave( MHoverEvent* anEvent );

protected:

	/** the virtual framework event proc */
	virtual bool processEvent( IUiEvent* pEvent );

protected:

	virtual void			paintHighLight( IGraphics* ptGraphics, const MRect &rect );
	virtual void			setControl( IControl* ptControl );
	virtual void			setInAutomation( bool value );
	virtual bool			getInAutomation();
	virtual void			updateTooltip();
	virtual String			toString();
	virtual void			onCommand( unsigned int id );
};

#endif