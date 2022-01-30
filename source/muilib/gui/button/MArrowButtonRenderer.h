#ifndef __MArrowButtonRenderer
#define __MArrowButtonRenderer

#include <framework/MObject.h>
#include "../IWndRenderer.h"
#include "../renderer/MArrowRenderer.h"

/** renders a button with a arrow in */
class MArrowButtonRenderer :
	public MObject,
	public IWndRenderer,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the arrow direction */
	MArrowRenderer::Direction ivDirection;

	/** constructor */
	MArrowButtonRenderer();

public:

	/** constructor */
	MArrowButtonRenderer( MArrowRenderer::Direction direction );

	/** destructor */
	virtual ~MArrowButtonRenderer();

	/** does the paining for the given compoent */
	virtual void paint( MWnd* pWnd, IGraphics* pG, const MRect& rect );

	/** query for superclass */
	virtual void* getInterface( const String &className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();
};

#endif
