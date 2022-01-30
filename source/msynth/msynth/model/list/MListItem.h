/**
 * (c)2001-2003 MRoc
 */

#ifndef __MListItem
#define __MListItem

//#include "IListItem.h"

/**
 * base class for a list item used by the linked list
 */
class MListItem /*:
	public IListItem*/
{
protected:

	MListItem			*ivPrev,
						*ivNext;

	int					ivIndex;

public:

	MListItem();
	MListItem( int index );
	MListItem( MListItem* ptItem );
	virtual ~MListItem();

	virtual void		setIndex( int index );
	virtual int			getIndex();

	virtual void		setPrev( MListItem* ptListItem );
	virtual MListItem*	getPrev();

	virtual void		setNext( MListItem* ptListItem );
	virtual MListItem*	getNext();

	virtual int			compareTo( MListItem* ptListItem );

	virtual void		release();
};

#endif