/**
 * (c)2001-2003 MRoc
 */

#include "MListItem.h"

MListItem::MListItem() :
	ivPrev( 0 ),
	ivNext( 0 ),
	ivIndex( 0 )
{
}

MListItem::MListItem( int index ) :
	ivPrev( 0 ),
	ivNext( 0 ),
	ivIndex( index )
{
}

MListItem::MListItem( MListItem* ptItem ) :
	ivPrev( 0 ),
	ivNext( 0 ),
	ivIndex( ptItem->getIndex() )
{
}

MListItem::~MListItem()
{
}

void MListItem::setIndex( int index )
{
	this->ivIndex = index;
}

int MListItem::getIndex()
{
	return this->ivIndex;
}

void MListItem::setPrev( MListItem* ptListItem )
{
	ivPrev = ptListItem;
}

MListItem* MListItem::getPrev()
{
	return ivPrev;
}

void MListItem::setNext( MListItem* ptListItem )
{
	ivNext = ptListItem;
}

MListItem* MListItem::getNext()
{
	return ivNext;
}

int MListItem::compareTo( MListItem* ptListItem )
{
	return ivIndex - ptListItem->getIndex();
}

void MListItem::release()
{
	delete this;
}