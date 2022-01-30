// (C)2002-2003 MRoc hifiShock

#ifndef __MFrameWnd
#define __MFrameWnd

#include "../MTopWnd.h"
#include "MMenu.h"

class MRootPane;

/** a top level window with a native border, titlebar and system menu... */
class MFrameWnd :
	public MTopWnd
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the embedded root pane */
	MRootPane* ivPtRootPane;

	/** a window menu */
	MMenu* ivPtMenu;

public:

	/** constructor */
	MFrameWnd();

	/** destructor */
	virtual ~MFrameWnd();

	/** creates the native window */
	virtual bool create( MRect rect, MTopWnd *parentWnd );

	/** adds a child to the window */
	virtual void addChild( MWnd* pWnd );

	/** removes a child to the window */
	virtual void removeChild( MWnd* pWnd );

	/** returns the number of childs */
	virtual unsigned int getChildCount();

	/** returns the number of childs */
	virtual MWnd* getChild( unsigned int index );

	/** process a system event */
	virtual bool processEvent( IUiEvent* pEvent );

	/** returns the client size of the window */
	virtual MSize getClientSize();

	/** sets the window menu */
	virtual void setMenu( MMenu* pMenu );

	/** returns the window menu */
	virtual MMenu* getMenu();

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** creates an instance of this class */
	static MObject* createInstance();

	/** returns, if available, the container's namespace */
	virtual MWndNamespace* getNamespace();

	/** sets the background color */
	virtual void setBkColor( MColor bkColor );

	/** sets the rect */
	virtual void setRect( const MRect& rect );

	/** sets the size */
	virtual void setSize( const MSize& size );

	/** layout components */
	virtual void doLayout();
};

#endif