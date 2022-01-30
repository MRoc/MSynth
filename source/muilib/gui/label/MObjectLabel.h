#ifndef __MObjectLabel
#define __MObjectLabel

#include "../MWnd.h"

class MObjectLabel :
	public MWnd 
{
protected:

	/** the object providing the text to display */
	MObject* ivPtObject;

    /** the text color */
	MColor ivTextColor;

	/** flag for centering the text */
	bool ivCenterText;

public:

	/** constructor */
	MObjectLabel();

	/** constructor */
	MObjectLabel( MObject* pObj, MColor textColor, MColor backGroundColor ); 

	/** destructor */
	virtual ~MObjectLabel();

	/** sets the objects providing the text to display */
	virtual void setObject( MObject* itPtObject );

	/** returns the objects providing the text to display */
	virtual MObject* getObject();

	/** paints the label */
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	/** sets the center text flag */
	virtual void setCenterText( bool center );

	/** returns the center text flag */
	virtual bool getCenterText() const;

	/** sets the foreground color */
	virtual void setFgColor( MColor color );

	/** returns the foreground color */
	virtual MColor getFgColor() const;

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();
};

#endif