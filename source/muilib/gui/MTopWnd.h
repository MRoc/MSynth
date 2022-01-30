#ifndef __MTopWnd
#define __MTopWnd

#include <framework/map/MProperties.h>
#include <framework/primitive/MRect.h>
#include <framework/primitive/MPoint.h>

#include "MWndCollection.h"
#include "MImage.h"
#include "framewnd/MIcon.h"
#include "MTopWndImpl.h"

class MTopWnd;
class IFrameWndListener :
		public IListener
{
public:
	virtual void onClose( MTopWnd* pWnd ) = 0;
	virtual void onDestroy( MTopWnd* pWnd ) = 0;
};

/**
 * a platform toplevel window.
 * only toplevel windows should be added as child.
 * the content window is set by setContentPane
 */
class MTopWnd :
	public MWndCollection
{
public:

	/** runtime type info */
	static MRtti gvRtti;

	/** definition of action when window was closed */
	enum ActionOnClose
	{
		NOTHING,
		DESTROY,
		HIDE,
		QUIT
	};

protected:

	/** the native implementation, bridge pattern */
	MTopWndImpl* ivImpl;

	/** list of listeners, notified 'bout closing... */
	MListenerList ivListeners;

	/** the window's preferred size */
	MSize ivPreferredSize;

	/** the window's minimum size */
	MSize ivMinimumSize;

	/** the window's maximum size */
	MSize ivMaximumSize;

	/** the action what should be done when window was closed */
	ActionOnClose ivActionOnClose;

	/** the window properties, can be null, stores e.g. position */
	MProperties* ivPtProperties;

public:

	/** contructor */
	MTopWnd();

	/** destructor */
	virtual ~MTopWnd();

	/** creates the native window */
	virtual bool create( MRect rect, MTopWnd *parentWnd );

	/** process a system event */
	virtual bool processEvent( IUiEvent* pEvent );

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** repaints the given rectangle. the rect should be in window coordinates */
	virtual void repaint( const MRect& rect );

	/** returns the native implementation */
	virtual MTopWndImpl* getImpl();

	/** sets a native icon */
	virtual void setIcon( MIcon* icon );

	/** returns the native icon */
	virtual MIcon* getIcon();

	/** sets the window's rect */
	virtual void setRect( const MRect &rect );

	/** sets the window's position */
	virtual void setPoint( const MPoint& point );

	/** sets the window's size */
	virtual void setSize( const MSize& size );

	/** returns size of boundary of the native frame, if available */
	virtual MSize getFrameBoundary();

	/** visibles the native window */
	virtual void setVisible( bool visible );

	/** converts a client coordinate to screen */
	virtual MPoint convertClient2Screen( const MPoint& point );

	/** sets the window's title */
	virtual void setText( const String& text );

	/** returns the windows's title */
	virtual String getText() const;

	/** sets the action on close */
	virtual void setActionOnClose( ActionOnClose actionOnClose );

	/** returns the action on close */
	virtual ActionOnClose getActionOnClose();

	/** sets the minimum size */
	virtual void setMinimumSize( MSize size );

	/** sets the maximum size */
	virtual void setMaximumSize( MSize size );

	/** sets the preferred size */
	virtual void setPreferredSize( MSize size );

	/** set the window's properties */
	virtual void setProperties( MProperties* ptProps );

	/** adds a listener to the window */
	virtual void addListener( IFrameWndListener* pListener );

	/** removes a listener from the window */
	virtual void removeListener( IFrameWndListener* pListener );

	/** centers the window on screen */
	virtual void centerWindow();

	/** stores the window position in properties */
	virtual void storeWindowPosition();

	/** returns the window position from propertiens */
	virtual bool restoreWindowPosition();

protected:

	/** invoked when windows closed */
	virtual void onClose();

	/** invoked when native command reached */
	virtual void onCommand( unsigned int id );

	/** sets the preferred client size */
	virtual void setPreferredClientSize( MSize size );

	/** returns the storred and clipped rect from properties */
	virtual MRect getStoredClippedRect();

	friend class MApp;
};

#endif