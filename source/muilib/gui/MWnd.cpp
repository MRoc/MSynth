#include "MWnd.h"
#include "MWndManager.h"
#include "MWndUtils.h"
#include <framework/MUtil.h>
#include <framework/io/MLogger.h>
#include <framework/MNativeTypes.h>
#include "MWndNamespace.h"

/** runtime type info */
MRtti MWnd::gvRtti = MRtti( "MWnd", MWnd::createInstance );

/** default constructor */
MWnd::MWnd() :
	ivPtParent( 0 ),
	ivVisible( true ),
	ivEnabled( true ),
	ivBkColor( 0x00000000 )
{
	MWndManager::getInstance()->regWnd( this );
}

/** destructor */
MWnd::~MWnd()
{
	MWndManager::getInstance()->unregWnd( this );
}

/** sets the window's position */
void MWnd::setRect( const MRect &rect )
{
	ivRect = rect;
}

/** sets the scroll offset */
void MWnd::setScrollPos( const MPoint &offset )
{
	ivScrollPos = offset;
}

/** returns the window's position */
MRect MWnd::getRect() const
{
	return ivRect;
}

/** returns the scroll position */
MPoint MWnd::getScrollPos() const
{
	return ivScrollPos;
}

/** sets the window visible state */
void MWnd::setVisible( bool visible )
{
	ivVisible = visible;
}

/** returns true if the window is visible */
bool MWnd::getVisible() const
{
	return ivVisible;
}

/** set enable flag */
void MWnd::setEnabled( bool enable )
{
	ivEnabled = enable;
}

/** returns the enable flag */
bool MWnd::getEnabled() const
{
	return ivEnabled;
}

/** sets the background color */
void MWnd::setBkColor( MColor color )
{
	ivBkColor = color;
}

/** returns the background color */
MColor MWnd::getBkColor() const
{
	return ivBkColor;
}

/** returns the parent window */
MWnd* MWnd::getParent() const
{
	return ivPtParent;
}

/** returns the top window */
MWnd* MWnd::getTopParent() const
{
	const MWnd* pBack = this;
	while( pBack->ivPtParent )
		pBack = pBack->ivPtParent;
	return (MWnd*) pBack; // HACK from const to nothing :-(
}

/** process a system event */
bool MWnd::processEvent( IUiEvent* pEvent )
{
	return true;
}

/** sets the parent window */
void MWnd::setParent( MWnd* pParent )
{
	ivPtParent = pParent;
}

/** paint the window. the rect should be in window coordinates */
void MWnd::paint( IGraphics* pGraphics, const MRect& rect )
{
	pGraphics->fillRect( rect, ivBkColor );
	//pGraphics->drawRect( rect, RGB( 255, 0, 0 ) );
}

/** repaints the given rectangle. the rect should be in window coordinates */
void MWnd::repaint( const MRect& rect )
{
	MRect r = MWndUtils::getClippingRect( this );
	if( r.getWidth() != 0 && r.getHeight() != 0 )
	{
		MWnd* pWnd = getTopParent();
		if( pWnd != this )
			pWnd->repaint( r );
	}
}

/** repaint this component */
void MWnd::repaint()
{
	repaint( ivRect );
}

/** returns the runtime type info */
IRtti* MWnd::getRtti() const
{
	return &gvRtti;
}

/** query interface */
void* MWnd::getInterface( const String& className ) const
{
	if( className == "MWnd" ) return (void*) ((MWnd*)this);
	else if( className == "ISerializeable" ) return (void*) ((ISerializeable*)this);
	else return MObject::getInterface( className );
}

void MWnd::setPoint( const MPoint &point ){ ivRect.setPoint( point );}

/** sets the window's size */
void MWnd::setSize( const MSize &size ){ ivRect.setSize( size );}

/** returns the window's size */
MSize MWnd::getSize() const{ return ivRect.getSize(); }

/** returns the window's upper left point */
MPoint MWnd::getPoint() const{ return ivRect.getPoint();}

/** returns the window's x coordinate */
int MWnd::getX() const{ return ivRect.getX();}

/** returns the window's x coordinate */
int MWnd::getY() const{ return ivRect.getY(); }

/** returns the window's width */
int MWnd::getWidth() const{ return ivRect.getWidth();}

/** returns the window's height */
int MWnd::getHeight() const {return ivRect.getHeight();}

/** returns wether this is a native control or not (for msg routing) */
bool MWnd::isNative() const { return false; };

/** loads from the given tree node */
void MWnd::load( MTreeNode* ptNode )
{
	if( ptNode )
	{
		setRect(
			MRect(
				MInt::parse( ptNode->getAttribute( "x", "0" ) ),
				MInt::parse( ptNode->getAttribute( "y", "0" ) ),
				MInt::parse( ptNode->getAttribute( "width", "0" ) ),
				MInt::parse( ptNode->getAttribute( "height", "0" ) ) ) );

		setScrollPos(
			MPoint(
				MInt::parse( ptNode->getAttribute( "cx", "0" ) ),
				MInt::parse( ptNode->getAttribute( "cy", "0" ) ) ) );

		setVisible( ptNode->getAttribute( "visible" ) == "false" ? false : true );
		setEnabled( ptNode->getAttribute( "enabled" ) == "false" ? false : true );
		setBkColor( MUtil::parseHexString( ptNode->getAttribute( "bgcolor", "0" ) ) );

		if( ptNode->hasAttribute( "id" ) && ! MWndNamespace::regObjNS( this, this, ptNode->getAttribute( "id" ) ) )
			MLogger::logWarning( "failed registering window with id \"%s\"", ptNode->getAttribute( "id" ).getData() );
	}
	else
		MLogger::logWarning(
			"MWnd::load: no node provided\n" );
}

/** stores as tree node */
MTreeNode* MWnd::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );

	pBack->setAttribute( "x", MInt::toString( ivRect.getX() ) );
	pBack->setAttribute( "y", MInt::toString( ivRect.getY() ) );
	pBack->setAttribute( "width", MInt::toString( ivRect.getWidth() ) );
	pBack->setAttribute( "height", MInt::toString( ivRect.getHeight() ) );
	pBack->setAttribute( "cx", MInt::toString( ivScrollPos.getX() ) );
	pBack->setAttribute( "cy", MInt::toString( ivScrollPos.getY() ) );
	pBack->setAttribute( "visible", getVisible() ? "true" : "false" );
	pBack->setAttribute( "enabled", getEnabled() ? "true" : "false" );
	pBack->setAttribute( "bgcolor", MUtil::toHexString( ivBkColor ) );

	return pBack;
}

/** creates an instance of this class */
MObject* MWnd::createInstance()
{
	return new MWnd();
}