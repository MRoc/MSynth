#ifndef __MLayoutRoot
#define __MLayoutRoot

#include "MLayoutWnd.h"
#include <framework/map/MStr2ObjMap.h>

/**
 * a layout root node embedding a id2wnd map for all childcontrols
 */
class MLayoutRoot :
	public MLayoutNode
{
public:

	/**
	 * runtime typeinfo
	 */
	static MRtti gvRtti;

protected:

	/**
	 * maps a control id onto the specific control lower in the tree
	 */
	MWndNamespace* ivNamespace;

public:

	MLayoutRoot();
	virtual ~MLayoutRoot();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual void setNamespace( MWndNamespace* pNamespace );
	virtual MWnd* getWnd( const String &id );
	virtual void registerWnd( MWnd* pWnd, const String &id );

	static MObject* createInstance();
};

#endif