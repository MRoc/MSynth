#ifndef __MLayoutWnd
#define __MLayoutWnd

#include "MLayoutNode.h"

/**
 * a layoutnode that references directly a control
 */
class MLayoutWnd :
	public MLayoutNode
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the ID identifining the control from programm
	 */
	String ivControlId;

	/**
	 * the control's preferres size
	 */
	MSize ivSize;

public:

	MLayoutWnd();
	virtual ~MLayoutWnd();
	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;
	virtual MSize getSize( MWnd* ptWnd, MSize maxSize );
	virtual void doLayout( MWnd* ptWnd, MRect rect );
	virtual void load( MTreeNode* ptNode );
	virtual MTreeNode* save();
	static MObject* createInstance();
	virtual String getId();
	virtual MSize getLoadedSize();
};

#endif