#ifndef __MBUTTON
#define __MBUTTON

#include <framework/listener/IActionListener.h>
#include <framework/listener/MListenerList.h>
#include "../MyInteractiveWnd.h"
#include "../IWndRenderer.h"
#include "../renderer/MArrowRenderer.h"

class MButton;

/** interface for beeing notified 'bout changes in button model */
class IButtonListener :
	public IListener
{
public:
	
	/** invoked when was button pressed */
	virtual void onButtonPressed( MButton* ptButton ) = 0;

	/** invoked when was button released */
	virtual void onButtonReleased( MButton* ptButton ) = 0;
};

/** (C)2000-2003 MRoc */
class MButton : public MyInteractiveWnd
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

protected:

	/** the button renderer */
	IWndRenderer* ivPtRenderer;

	/** list of button listeners */
	MListenerList ivButtonListener;

	/** list of action listeners */
	MListenerList ivActionListener;

public:

	/** constructor */
	MButton();

	/** constructor with renderer */
	MButton( IWndRenderer* pRenderer );

	/** constructor for icon button */
	MButton( unsigned int bmIdUnpressed, unsigned int bmIdPressed, MColor tColor1, MColor tColor2, MColor bkColor );

	/** constructor for text button */
	MButton( String text, MColor textColor, MColor disabledColor, MColor bkColor );

	/** constructor for arrow button */
	MButton( MArrowRenderer::Direction direction, MColor bkColor );

	/** destructor */
	virtual ~MButton();

	/** paints the button */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/** adds a actionlistener */
	virtual void addActionListener( IActionListener* ptListener );

	/** removes a actionlistener */
	virtual void removeActionListener( IActionListener* ptListener );

	/** adds a buttonlistener */
	virtual void addButtonListener( IButtonListener* ptListener );

	/** removes a buttonlistener */
	virtual void removeButtonListener( IButtonListener* ptListener );

	/** if this method returns true, the renderer paints a sunken button */
	virtual bool isSunken();

	/** query superclass */
	virtual void* getInterface( const String& className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance */
	static MObject* createInstance();

	/** returns the renderer */
	virtual IWndRenderer* getRenderer();

protected:

	/** invoked when the button was clicked, to be overwritten */
	virtual void clicked();

	/** fires a action notify */
	virtual void fireActionPerformed();

	/** fires a button pressed notify */
	virtual void fireButtonPressed();

	/** fires a button released notify */
	virtual void fireButtonReleased();

	/** invoked when mouse button pressed */
	virtual void onMouseDown( MMouseEvent* ptEvent );

	/** invoked when mouse button released */
	virtual void onMouseUp( MMouseEvent* ptEvent );
};

#endif