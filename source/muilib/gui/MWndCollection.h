#ifndef __MWndCollection
#define __MWndCollection

#include "MWnd.h"
#include "MWndNamespace.h"

/**
 * a window with child windows
 */
class MWndCollection :
	public MWnd
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** list of childs */
	std::vector<MWnd*> ivChilds;

	/** a window namespace, can be null */
	MWndNamespace* ivPtNamespace;

public:

	/** constuctor */
	MWndCollection();

	/** destructor */
	virtual ~MWndCollection();

	/** adds a child to the window */
	virtual void addChild( MWnd* pWnd );

	/** removes a child to the window */
	virtual void removeChild( MWnd* pWnd );

	/** returns the number of childs */
	virtual unsigned int getChildCount();

	/** returns the number of childs */
	virtual MWnd* getChild( unsigned int index );

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** sets size and does layout */
	virtual void setSize( MSize size );

	/** sets rect and does layout */
	virtual void setRect( MRect rect );

	/** paint the window. the rect should be in window coordinates */
	virtual void paint( IGraphics* pGraphics, const MRect& rect );

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();

	/** returns, if available, the container's namespace */
	virtual MWndNamespace* getNamespace();

	/** does the layout */
	virtual void doLayout(){};

protected:

	/** returns the index of the given window */
	virtual int getIndex( MWnd* pWnd );
};

#endif