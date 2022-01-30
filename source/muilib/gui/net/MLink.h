#ifndef _MLink
#define _MLink

#include "../MWnd.h"

class MLink : public MWnd
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the url of the link */
	String ivUrl;

	/** set true if link allready visited */
	bool ivVisited;

	/** the neutral color */
	MColor ivNeutralColor;

	/** the color after visted */
	MColor ivVisitedColor;

public:

	/** constructor */
	MLink();

	/** constructor */
	MLink( String url );

	/** destructor */
	virtual ~MLink();

	/** paints the link */
	virtual void paint( IGraphics* pG, const MRect& rect );

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** creates an instance of this class */
	static MObject* createInstance();

protected:

	/** process event... */
	virtual bool processEvent( IUiEvent* pEvent );

	/** should set cusor ;-) */
	virtual bool onSetCursor();
};

#endif
