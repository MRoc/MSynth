#ifndef __MTEXTBUTTON
#define __MTEXTBUTTON

#include "../IWndRenderer.h"

/** paints a flat text button (C)2000-2003 MRoc hifiShock */
class MFlatTextButtonRenderer :
	public MObject,
	public IWndRenderer,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the text color */
	MColor ivTextColor;

	/* the text */
	String ivText;

	/** constructor */
	MFlatTextButtonRenderer();

public:

	/** constructor */
	MFlatTextButtonRenderer( String text, MColor textColor );

	/** destructor */
	virtual ~MFlatTextButtonRenderer();

	/** renders the button */
	virtual void paint( MWnd* pWnd, IGraphics* ptGraphics, const MRect &rect );

	/** sets the text to render */
	virtual void setText(String text);

	/** returns the text to render */
	virtual String getText() const;

	/** sets the text color */
	virtual void setTextColor( MColor color );

	/** returns the text color */
	virtual MColor getTextColor() const;

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