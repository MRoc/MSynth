/**
 * (C) 2002-2003 MRoc
 */

#ifndef __MyInteractiveWnd
#define __MyInteractiveWnd

#include "MWnd.h"
#include "../gui/tooltip/MToolTipCtrl.h"
#include "../gui/event/MMouseEvent.h"
#include "../gui/event/MHoverEvent.h"
#include "../gui/event/MKeyEvent.h"
#include "../gui/event/MFocusEvent.h"

class MyInteractiveWnd :
	public MWnd
{
protected:

	static const unsigned int TOOL_TIP_TIME;

	static const unsigned int MOUSE_IN;
	static const unsigned int MOUSE_DOWN_LEFT;
	static const unsigned int MOUSE_DOWN_RIGHT;
	static const unsigned int CAPTURING;
	static const unsigned int TOOLTIP;

	unsigned int ivBitState;
	String ivToolTip;

public:

	MyInteractiveWnd();
	virtual ~MyInteractiveWnd();

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
	virtual void	setMouseIn( bool value );
	virtual void	setMouseDownLeft( bool down );
	virtual void	setMouseDownRight( bool down );
	virtual void	setMouseDown( MMouseEvent::MMouseButton button, bool value );
	virtual void	setCapturing( bool value );
	virtual void	setShowingTooltip( bool value );

	// event functions
	virtual void	onToolTip( bool value);

	virtual void	onKeyDown( MKeyEvent* anEvent );
	virtual void	onKeyUp( MKeyEvent* anEvent );

	virtual void	onMouseDown( MMouseEvent* anEvent );
	virtual void	onMouseUp( MMouseEvent* anEvent );
	virtual void	onMouseDoubleClick( MMouseEvent* anEvent );
	virtual void	onMouseMove( MMouseEvent* anEvent );
	virtual void	onMouseHover( MHoverEvent* anEvent );
	virtual void	onMouseLeave( MHoverEvent* anEvent );

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

protected:

	/**
	 * the virtual window proc
	 */
	virtual bool processEvent( IUiEvent* pEvent );
};

#endif