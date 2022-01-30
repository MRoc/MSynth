#ifndef __MRadioButtonRenderer
#define __MRadioButtonRenderer

#include "../IWndRenderer.h"

class MRadioButtonRenderer :
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

	/** the color of disabled text */
	MColor ivTextColorDisabled;

	/* the text */
	String ivText;

	/** constructor */
	MRadioButtonRenderer();

public:

	/** constructor */
	MRadioButtonRenderer( String text, MColor textColor, MColor textColorDisabled  );

	/** destructor */
	virtual ~MRadioButtonRenderer();

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

	/** sets the disabled font color */
	virtual void setTextColorDisabled( MColor color );

	/** returns the disabled font color */
	virtual MColor getTextColorDisabled() const;

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