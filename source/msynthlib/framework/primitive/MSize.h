#ifndef __MSize
#define __MSize

#include "MInt.h"

/**
 * implementation of a size or dimension
 * object having a width and a height
 */
class MSize :
	public MObject,
	public ISerializeable
{
public:

	/** runtime support */
	static MRtti gvRtti;

protected:

	/** the width attribute tag used for xml serialisation */
	static const String ATTR_WIDTH;

	/** the height attribute tag used for xml serialisation */
	static const String ATTR_HEIGHT;

	/** stores the width */
	int ivWidth;

	/** stores the height */
	int ivHeight;

public:

	/** default constructor */
	MSize();

	/** default constructor */
	MSize( const MSize& toCopy );

	/** constructor */
	MSize( int width, int height );

	/** destructor */
	virtual ~MSize();

	/** returns a string representation */
	virtual String toString() const;

	/** queries for a specified interface */
	virtual void* getInterface( const String &className ) const;

	/** returns the class name for runtime purpose */
	virtual IRtti* getRtti() const;

	/** sets the stored width */
	virtual void setWidth( int width );

	/** returns the stored width */
	virtual const int getWidth() const;

	/** sets the stored height */
	virtual void setHeight( int height );

	/** returns the stored height */
	virtual const int getHeight() const;

	/** adds value to the height */
	virtual MSize& addY( int value );

	/** adds value to the width */
	virtual MSize& addX( int value );

	/** adds wx to the width, adds hy to the height */
	virtual MSize& add( int wx, int hy );

	/** serialize as tree node */
	virtual MTreeNode* save();

	/**load from treenode */
	virtual void load( MTreeNode* node );

	/** creates an instance of this class */
	static MObject* createInstance();

	friend class MRect;
};

#endif