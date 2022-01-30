#ifndef __MLayoutLeaf
#define __MLayoutLeaf

#include <framework/treedocument/ISerializeable.h>
#include "../MWndCollection.h"

class MLayoutLeaf :
	public MObject,
	public ISerializeable
{
public:

	enum SizingType
	{
		FIXED,
		BOTH,
		VERTICAL,
		HORIZONTAL
	};

	static MRtti gvRtti;

protected:

	MLayoutLeaf* ivPtParent;
	SizingType ivSizing;

public:

	MLayoutLeaf();
	virtual ~MLayoutLeaf();

	virtual void setParent( MLayoutLeaf* ptParent );
	virtual MLayoutLeaf* getParent();

	virtual MLayoutLeaf* getRoot();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual MSize getSize( MWnd* ptWnd, MSize maxSize );
	virtual void doLayout( MWnd* ptWnd, MRect rect );

	virtual MLayoutLeaf* hasSizing( SizingType type );

	virtual SizingType getSizing();

	virtual SizingType str2type( String type );
	virtual String type2str( SizingType type );

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