/**
 * (c)2001-2003 MRoc
 */

#ifndef __MList
#define __MList

#include <framework/MObject.h>
#include "MListItem.h"
#include "MListUtil.h"

class MList :
	public MObject
{
protected:

	MListItem				*ivPtFirstItem;

public:

	FULL_RTTI( MList, "MList" );

	MList();
	virtual ~MList();

	virtual void			setFirstItem( MListItem* ptListNode );
	virtual MListItem*		getFirstItem();

	virtual bool			isEmpty();
	virtual void			unlink( MListItem* ptListNode );

	virtual void			deleteItem( MListItem *item );

	virtual void			clear();

	virtual bool			isFree( unsigned int index ) = 0;

	virtual void			insertItem( MListItem* ptListNode );
};

#endif

