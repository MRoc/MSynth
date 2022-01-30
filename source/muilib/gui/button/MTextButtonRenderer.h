#ifndef __MTextButtonRenderer
#define __MTextButtonRenderer

#include "../IWndRenderer.h"

class MTextButtonRenderer :
	public MObject,
	public IWndRenderer,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the text to display */
	String ivText;

	/** the text colors */
	MColor
		ivTextColor,
		ivTextColorDisabled;

	/** default constructor */
	MTextButtonRenderer();

public:

	/** constructor */
	MTextButtonRenderer( String text, MColor textColor, MColor textColorDisabled );

	/** destructor */
	virtual ~MTextButtonRenderer();

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