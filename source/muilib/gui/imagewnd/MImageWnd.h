#ifndef __MImageWnd
#define __MImageWnd

#include "../MWnd.h"

/** a window only displaying a image */
class MImageWnd :
	public MWnd
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the image to display */
	MImage ivImage;

public:

	/** constructor */
	MImageWnd();

	/** destructor */
	virtual ~MImageWnd();

	/** paints */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;
};

#endif