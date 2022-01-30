/**
 * (C)2000-2003 MRoc
 */

#ifndef __MCOUNTER
#define __MCOUNTER

#include "../MWnd.h"
#include "../MImage.h"

class MCounter :
	public MWnd
{
private:

	static String gvDefaultImages[10];

	unsigned int ivValue, ivRange;
	MImage ivBitmaps[10];

public:

	FULL_RTTI_SUPER( MCounter, "MCounter", MWnd );

	MCounter();
	MCounter( unsigned int ivRange );
	MCounter( unsigned int ivRange, unsigned int *ar_bitmapIDs );
	MCounter( unsigned int ivRange, String *arBitmapResources );
	virtual ~MCounter();

	virtual void setRange( unsigned int range );
	virtual unsigned int getRange() const;

	virtual void setValue( unsigned int value );
	virtual unsigned int getValue() const;

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	virtual MSize getPreferredSize();

	/** store ui state into treenode */
	virtual MTreeNode* save();

	/** load ui state from treenode */
	virtual void load( MTreeNode* pNode );

	static MObject* createInstance()
	{
		return new MCounter();
	}
};

#endif