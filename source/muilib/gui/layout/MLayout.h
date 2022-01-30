#ifndef __MLayout
#define __MLayout

#include <framework/MDefaultFactory.h>
#include <framework/treedocument/ISerializeable.h>
#include "MLayoutRoot.h"

class MLayout :
	public MObject,
	public ISerializeable
{
public:

	static MRtti gvRtti;

protected:

	MLayoutRoot ivRoot;

public:

	MLayout();
	virtual ~MLayout();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual void doLayout( MWndCollection* ptWnd );

	virtual void load( MTreeNode* ptNode );
	virtual MTreeNode* save();

	virtual void load( String fileName );
	virtual void store( String fileName );

	static MObject* createInstance();

	virtual MLayoutRoot* getRoot();
};

#endif