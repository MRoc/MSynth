/**
 * (c)2001-2003 MRoc
 */

#include "MList.h"

INIT_RTTI( MList, "MList" );

MList::MList() :
	ivPtFirstItem( 0 )
{
}

MList::~MList()
{
	clear();
}

void MList::setFirstItem( MListItem* ptListItem )
{
	if( ptListItem )
		ptListItem->setPrev( 0 );
	ivPtFirstItem = ptListItem;
}

MListItem* MList::getFirstItem()
{
	return ivPtFirstItem;
}

bool MList::isEmpty()
{
	return ivPtFirstItem == 0;
}

void MList::unlink( MListItem* ptListItem )
{
	if( ptListItem == getFirstItem() )
	{
		if( ptListItem->getNext() )
		{
			ivPtFirstItem = ptListItem->getNext();	
			ivPtFirstItem->setPrev( 0 );
		}
		else
		{
			ivPtFirstItem = 0;
		}
	}
	else
	{
		MListUtil::unlink( ptListItem );
	}

	ptListItem->setNext( 0 );
	ptListItem->setPrev( 0 );
}

void MList::deleteItem( MListItem *item )
{
	unlink( item );
	delete item;
}

void MList::clear()
{
	MListItem *ptItem = getFirstItem();

	while( ptItem != 0 )
	{
		MListItem* old = ptItem;
		ptItem = ptItem->getNext();
		delete old;
	}

	setFirstItem( 0 );
}

void MList::insertItem( MListItem *item )
{
	if( getFirstItem() == 0 )
	{
		setFirstItem( item );
		item->setPrev( 0 );
		item->setNext( 0 );
	}
	else
	{
		MListItem* pt = getFirstItem();

		// search insert location note
		while(	item->compareTo( pt ) > 0 &&
				pt->getNext() != 0 )
		{
			pt = pt->getNext();
		}

		if( item->compareTo( pt ) > 0 )
		{
			if( pt->getNext() )
			{ // Insert after pt before nextNote
				MListUtil::link( pt, item, pt->getNext() ); 
			}
			else
			{ // Insert after pt at the end of the list (push back)
				MListUtil::link( pt, item );
				item->setNext( 0 );
			}
		}
		else
		{
			MListItem* prev = pt->getPrev();
			if( prev )
			{ // Insert before pointerToBar
				MListUtil::link( prev, item, pt );
			}
			else
			{ // Insert at First position (push front)
				MListUtil::link( item, getFirstItem() );
				setFirstItem( item );
			}
		}
	}
};
