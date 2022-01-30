#ifndef __MLayoutCol
#define __MLayoutCol

#include "MLayoutNode.h"
#include <framework/treedocument/ISerializeable.h>
#include <framework/MDefaultFactory.h>
#include <framework/primitive/MInsets.h>

class MLayoutCol :
	public MLayoutNode
{
public:

	static MRtti gvRtti;

	enum Align
	{
		LEFT,
		CENTER,
		RIGHT
	};

protected:

	Align ivAlign;
	MInsets ivInsets;

public:
	MLayoutCol();
	virtual ~MLayoutCol();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual MSize getSize( MWnd* ptWnd, MSize maxSize );
	virtual void doLayout( MWnd* ptWnd, MRect rect );

	virtual String align2str( Align align );
	virtual Align str2align( String str );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	static MObject* createInstance();
};

#endif