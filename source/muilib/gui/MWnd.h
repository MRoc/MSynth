#ifndef __MWnd
#define __MWnd

#include "event/IUiEvent.h"
#include "IGraphics.h"
#include <framework/primitive/MRect.h>

/**
 * the superclass of all windows
 * that have a position, size, scrollpos
 * and a parent window.
 *
 * the window is able to receive events
 */
class MWnd :
	public MObject,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the parent window */
	MWnd* ivPtParent;

	/** the position of the window related to it's parent */
	MRect ivRect;

	/** the window's scrollposition */
	MPoint ivScrollPos;

	/** the is visible flag */
	bool ivVisible;

	/** the enabled flag */
	bool ivEnabled;

	/** the background color */
	MColor ivBkColor;

public:

	/** default constructor */
	MWnd();

	/** destructor */
	virtual ~MWnd();

	/** returns wether this is a native control or not (for msg routing) */
	virtual bool isNative() const;

	/** process a system event */
	virtual bool processEvent( IUiEvent* pEvent );

	/** sets the window's position */
	virtual void setRect( const MRect &rect );

	/** sets the scroll offset */
	virtual void setScrollPos( const MPoint &offset );

	/** sets the window's point */
	virtual void setPoint( const MPoint& point );

	/** sets the window's size */
	virtual void setSize( const MSize& size );

	/** returns the window's size */
	virtual MSize getSize() const;

	/** returns the window's upper left point */
	virtual MPoint getPoint() const;

	/** returns the window's position */
	virtual MRect getRect() const;

	/** returns the scroll position */
	virtual MPoint getScrollPos() const;

	/** returns the window's x coordinate */
	virtual int getX() const;

	/** returns the window's x coordinate */
	virtual int getY() const;

	/** returns the window's width */
	virtual int getWidth() const;

	/** returns the window's height */
	virtual int getHeight() const;

	/** sets the window visible state */
	virtual void setVisible( bool visible );

	/** returns true if the window is visible */
	virtual bool getVisible() const;

	/** set enable flag */
	virtual void setEnabled( bool enable );

	/** returns the enable flag */
	virtual bool getEnabled() const;

	/** sets the background color */
	virtual void setBkColor( MColor color );

	/** returns the background color */
	virtual MColor getBkColor() const;

	/** returns the parent window */
	virtual MWnd* getParent() const;

	/** returns the top window */
	virtual MWnd* getTopParent() const;

	/** paint the window. the rect should be in window coordinates */
	virtual void paint( IGraphics* pGraphics, const MRect& rect );

	/** repaints the given rectangle. the rect should be in window coordinates */
	virtual void repaint( const MRect& rect );

	/** repaint this component */
	virtual void repaint();

	/**  returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** forward declaration */
	friend class MWndCollection;

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();

protected:

	/** sets the parent window */
	virtual void setParent( MWnd* pParent );
};

#endif