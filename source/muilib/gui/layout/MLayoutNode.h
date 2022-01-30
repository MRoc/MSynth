#ifndef __MLayoutNode
#define __MLayoutNode

#include "MLayoutLeaf.h"

class MLayoutNode :
	public MLayoutLeaf
{
public:

	static MRtti gvRtti;

protected:

	MObjectList ivChilds;

public:

	MLayoutNode();
	virtual ~MLayoutNode();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual void addChild( MLayoutLeaf* ptChild );
	virtual void removeChild( MLayoutLeaf* ptChild );
	virtual unsigned int getChildCount();
	virtual MLayoutLeaf* getChild( unsigned int index );

	virtual MLayoutLeaf* hasSizing( SizingType type );
};

#endif