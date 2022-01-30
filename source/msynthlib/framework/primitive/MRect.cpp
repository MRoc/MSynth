#include "MRect.h"

/**
 * runtime support
 */
MRtti MRect::gvRtti = MRtti( "MRect", MRect::createInstance );

/**
 * the xml attribute name storing x
 */
const String MRect::ATTR_X = "x";

/**
 * the xml attribute name storing y
 */
const String MRect::ATTR_Y = "y";

/**
 * the xml attribute name storing width
 */
const String MRect::ATTR_WIDTH = "width";

/**
 * the xml attribute name storing height
 */
const String MRect::ATTR_HEIGHT = "height";

/**
 * default constructor
 */
MRect::MRect() : ivX( 0 ), ivY( 0 ), ivWidth( 0 ), ivHeight( 0 )
{
}

/**
 * default constructor
 */
MRect::MRect( const MRect& rect ) :
	ivX( rect.ivX ), ivY( rect.ivY ), ivWidth( rect.ivWidth ), ivHeight( rect.ivHeight )
{
}

/**
 * constructor
 */
MRect::MRect( const MPoint& p, const MSize& s ) :
	ivX( p.ivX ), ivY( p.ivY ), ivWidth( s.ivWidth ), ivHeight( s.ivHeight )
{
}

/**
 * constructor
 */
MRect::MRect( const MPoint& p1, const MPoint& p2 )
	: ivX( 0 ), ivY( 0 ), ivWidth( 0 ), ivHeight( 0 )
{
	int x1 = p1.ivX;
	int y1 = p1.ivY;
	int x2 = p2.ivX;
	int y2 = p2.ivY;

	if( x1 > x2 )
	{
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	if( y1 > y2 )
	{
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	ivX = x1;
	ivY = y1;
	ivWidth = x2-x1;
	ivHeight = y2-y1;
}

/**
 * constructor
 */
MRect::MRect( int x, int y, int width, int height ) :
	ivX( x ), ivY( y ), ivWidth( width ), ivHeight( height )
{
}

/**
 * destructor
 */
MRect::~MRect()
{
}

/**
 * returns a string representation
 */
String MRect::toString() const
{
	String back("");
	back.Format(
		"%s=%i %s=%i %s=%i %s=%i",
		ATTR_X.getData(),
		ivX,
		ATTR_Y.getData(),
		ivY,
		ATTR_WIDTH.getData(),
		ivWidth,
		ATTR_HEIGHT.getData(),
		ivHeight );
	return back;
}

/**
 * returns the class name for runtime purpose
 */
IRtti* MRect::getRtti() const
{
	return &gvRtti;
}

/**
 * querries for a specified interface
 */
void* MRect::getInterface( const String &className ) const
{
	if( className == gvRtti.getClassName() )
		return (void*) ((MRect*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * sets the coordinate x
 */
void MRect::setX( int x )
{
	ivX = x;
}

/**
 * returns the coordinate x
 */
const int MRect::getX() const
{
	return ivX;
}

/**
 * returns the coordinate x right
 */
const int MRect::getRight() const 
{
	return (int) (ivX + ivWidth);
}

/**
 * sets the coordinate y
 */
void MRect::setY( int y )
{
	ivY = y;
}

/**
 * returns the coordinate y
 */
const int MRect::getY() const 
{
	return ivY;
}

/**
* returns the coordinate y bottom
*/
const int MRect::getBottom() const
{
	return (int) (ivY + ivHeight);
}

/**
 * sets the width of this rect
 */
void MRect::setWidth( int width )
{
	ivWidth = width;
}

/**
 * returns the width of this rect
 */
const int MRect::getWidth() const
{
	return ivWidth;
}

/**
 * sets the height of this rect
 */
void MRect::setHeight( int height )
{
	ivHeight = height;
}

/**
 * returns the height
 */
const int MRect::getHeight() const
{
	return ivHeight;
}

/**
 * sets the point
 */
void MRect::setPoint( const MPoint& p )
{
	ivX = p.ivX;
	ivY = p.ivY;
}

/**
 * returns the point at the upper left corner
 */
MPoint MRect::getPoint() const
{
	return MPoint( ivX, ivY );
}

/**
 * sets the size
 */
void MRect::setSize( const MSize& s )
{
	ivWidth = s.ivWidth;
	ivHeight = s.ivHeight;
}

/**
 * returns the size of the rectangle
 */
MSize MRect::getSize() const
{
	return MSize( ivWidth, ivHeight );
}

/**
 * returns true if the given point is contained in
 * the rectangle
 */
bool MRect::contains( const MPoint& p ) const
{
	return
		p.ivX > ivX &&
		p.ivX < getRight() &&
		p.ivY > ivY &&
		p.ivY < getBottom();
}

/**
 * returns a point in the center of the rect
 */
MPoint MRect::getCenter() const
{
	return
		MPoint(
			ivX + ivWidth / 2,
			ivY + ivHeight / 2 );
}

/**
 * returns the point at the bottom right corner
 */
MPoint MRect::getBottomRight() const
{
	return
		MPoint(
			ivX + ivWidth,
			ivY + ivHeight );
}

/**
 * save as tree node
 */
MTreeNode* MRect::save()
{
	MTreeNode* ptBack = new MTreeNode( ELEM_OBJECT );
	ptBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( ATTR_X, MInt::toString( getX() ) );
	ptBack->setAttribute( ATTR_Y, MInt::toString( getY() ) );
	ptBack->setAttribute( ATTR_WIDTH, MInt::toString( getWidth() ) );
	ptBack->setAttribute( ATTR_HEIGHT, MInt::toString( getHeight() ) );
	return ptBack;
}

/**
 * load from tree node
 */
void MRect::load( MTreeNode* ptNode )
{
	if( ptNode )
	{
		if( // NEW STYLE
			ptNode->hasAttribute( ATTR_X ) ||
			ptNode->hasAttribute( ATTR_Y ) ||
			ptNode->hasAttribute( ATTR_WIDTH ) ||
			ptNode->hasAttribute( ATTR_WIDTH ) )
		{
			ivX = MInt::parse( ptNode->getAttribute( ATTR_X ) );
			ivY = MInt::parse( ptNode->getAttribute( ATTR_Y ) );
			ivWidth = MInt::parse( ptNode->getAttribute( ATTR_WIDTH ) );
			ivHeight = MInt::parse( ptNode->getAttribute( ATTR_HEIGHT ) );
		}
		else if( // DEPRECATED
			ptNode->hasAttribute( "left" ) ||
			ptNode->hasAttribute( "right" ) ||
			ptNode->hasAttribute( "top" ) ||
			ptNode->hasAttribute( "bottom" ) )
		{
			TRACE( "<mrect::load warning=\"xml format is depricated!\"/>\n" );
			ivX = MInt::parse( ptNode->getAttribute( "left" ) );
			ivY = MInt::parse( ptNode->getAttribute( "top" ) );
			ivWidth = MInt::parse( ptNode->getAttribute( "right" ) ) - ivX;
			ivHeight = MInt::parse( ptNode->getAttribute( "bottom" ) ) - ivY;
		}
	}
}

/**
 * create an instance of a MRect
 */
MObject* MRect::createInstance()
{
	return new MRect();
}

/**
 * returns true if the given rect overlaps this one
 */
bool MRect::overlap( const MRect &a ) const
{
	return
		ivX<a.ivX+a.ivWidth &&
		ivY<a.ivY+a.ivHeight &&
		a.ivX<ivX+ivWidth &&
		a.ivY<ivY+ivHeight;
}

static inline int _max(int a,int b){ return a>b?a:b; }
static inline int _min(int a,int b){ return a<b?a:b; }

/**
 * Intersection between rectangles
 */
MRect operator*(const MRect& r1,const MRect& r2)
{
	int xx=_max(r1.ivX,r2.ivX);
	int ww=_min(r1.ivX+r1.ivWidth,r2.ivX+r2.ivWidth)-xx;
	int yy=_max(r1.ivY,r2.ivY);
	int hh=_min(r1.ivY+r1.ivHeight,r2.ivY+r2.ivHeight)-yy;
	return MRect(xx,yy,ww,hh);
}

/**
 * union of two rectangles
 */
MRect operator+(const MRect& r1, const MRect& r2)
{
	int xx=_min(r1.ivX,r2.ivX);
	int ww=_max(r1.ivX+r1.ivWidth,r2.ivX+r2.ivWidth)-xx;
	int yy=_min(r1.ivY,r2.ivY);
	int hh=_max(r1.ivY+r1.ivHeight,r2.ivY+r2.ivHeight)-yy;
	return MRect(xx,yy,ww,hh);
}

// Union ivWidtivHeightitivHeight rectangle
MRect& MRect::operator+=(const MRect &r)
{
  ivWidth=_max(ivX+ivWidth,r.ivX+r.ivWidth);
  ivX=_min(ivX,r.ivX);
  ivWidth-=ivX;
  ivHeight=_max(ivY+ivHeight,r.ivY+r.ivHeight);
  ivY=_min(ivY,r.ivY);
  ivHeight-=ivY;
  return *this;
 }


// Intersection ivWidtivHeightitivHeight rectangle
MRect& MRect::operator*=(const MRect &r)
{
	ivWidth=_min(ivX+ivWidth,r.ivX+r.ivWidth);
	ivX=_max(ivX,r.ivX);
	ivWidth-=ivX;
	ivHeight=_min(ivY+ivHeight,r.ivY+r.ivHeight);
	ivY=_max(ivY,r.ivY);
	ivHeight-=ivY;
	return *this;
}

MRect operator+(const MRect& r, const MPoint& p)
{
	return MRect( r.ivX+p.getX(), r.ivY+p.getY(), r.ivWidth, r.ivHeight );
}
MRect operator-(const MRect& r, const MPoint& p)
{
	return MRect( r.ivX-p.getX(), r.ivY-p.getY(), r.ivWidth, r.ivHeight );
}
MRect& MRect::operator+=(const MPoint &p)
{
	ivX+=p.ivX; ivY+=p.ivY;
	return *this;
}
MRect& MRect::operator-=(const MPoint &p)
{
	ivX-=p.ivX; ivY-=p.ivY;
	return *this;
}
MRect operator+(const MRect& r, const MSize& p)
{
	return MRect( r.ivX, r.ivY, r.ivWidth+p.getWidth(), r.ivHeight+p.getHeight() );
}
MRect operator-(const MRect& r, const MSize& p)
{
	return MRect( r.ivX, r.ivY, r.ivWidth-p.getWidth(), r.ivHeight-p.getHeight() );
}
MRect& MRect::operator+=(const MSize &p)
{
	ivWidth+=p.ivWidth; ivHeight+=p.ivHeight;
	return *this;
}
MRect& MRect::operator-=(const MSize &p)
{
	ivWidth-=p.ivWidth; ivHeight-=p.ivHeight;
	return *this;
}