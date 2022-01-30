#ifndef __MTextFieldRenderer
#define __MTextFieldRenderer

#include "../IWndRenderer.h"
#include <framework/primitive/MInsets.h>

class MTextField;

class MTextFieldRenderer :
	public MObject,
	public IWndRenderer,
	public ISerializeable
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the background color */
	MColor ivBkColor;

	/** the foreground color */
	MColor ivFgColor;

	/** the foreground color for selected text */
	MColor ivFgColorSel;

	/** the background color for selected text */
	MColor ivBgColorSel;

	/** the border color */
	MColor ivBorderColor;

	/** the cursor color */
	MColor ivCursorColor;

	/** the border insets */
	MInsets ivInsets;

	/** the cursor toggle */
	bool ivCursorVisible;

public:

	/** constructor */
	MTextFieldRenderer();

	/** destructor */
	virtual ~MTextFieldRenderer();

	/** does the paining for the given compoent */
	virtual void paint( MWnd* pWnd, IGraphics* pG, const MRect& rect );

	/** sets the border insets */
	virtual void setInsets( MInsets insets );

	/** returns the border insets */
	virtual MInsets getInsets() const;

	/** sets the cursor's visible state */
	virtual void setCursorVisible( bool visible );

	/** returns the cursor's visible flag */
	virtual bool getCursorVisible();

	/** toggles the cursor from on to off and from off to on */
	virtual void toggleCursor();

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

protected:

	/** returns width of given text */
	virtual int getTextWidth( const String& text ) const;
};

#endif