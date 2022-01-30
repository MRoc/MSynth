#ifndef __MTransparentBitmapButtonRenderer
#define __MTransparentBitmapButtonRenderer

#include "../IWndRenderer.h"
#include "../MImage.h"

class MTransparentBitmapButtonRenderer :
	public MObject,
	public IWndRenderer,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** alloc flag, if true, images will be deleted in destructor */
	bool ivAllocated;

	/** the two images, neutral and pressed */
	MImage *ivPtBitmaps[2];

	/** the transparant color in the image */
	MColor ivTransparenceColors[2];

	/** default constructor */
	MTransparentBitmapButtonRenderer();

public:

	/** constructor 1 */
	MTransparentBitmapButtonRenderer(
		MImage* ptBitmap1, MImage* ptBitmap2, MColor transparenceColor1, MColor transparenceColor2 );

	/** constructor 2 */
	MTransparentBitmapButtonRenderer(
		unsigned int id1, unsigned int id2, MColor transparenceColor1, MColor transparenceColor2 );

	/** destructor */
	virtual ~MTransparentBitmapButtonRenderer();

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