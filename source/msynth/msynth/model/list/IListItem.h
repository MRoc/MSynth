#ifndef __IListItem
#define __IListItem

#include "../../../framework/MTypes.h"

class IListItem
{
public:

	virtual void		setIndex( int index );
	virtual int			getIndex();

	virtual void		setPrev( IListItem* ptListItem );
	virtual IListItem*	getPrev();

	virtual void		setNext( IListItem* ptListItem );
	virtual IListItem*	getNext();

	virtual int			compareTo( IListItem* ptListItem );

	virtual void		release();
};

#endif