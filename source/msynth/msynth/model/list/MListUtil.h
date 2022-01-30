/*

  MNoteUtil (C)2001 MRoc hifiShock

  Some methods to link and unlink notes

*/

#ifndef __MListUtil
#define __MListUtil

#include "..\list\MListItem.h"

class MListUtil
{
	public:	

		static void	link( MListItem *firstItem, MListItem *secondItem )
		{
			if( firstItem )
			{
				firstItem->setNext( secondItem );
			}
			if( secondItem )
			{
				secondItem->setPrev( firstItem );
			}
		};

		static void	link( MListItem *prevItem, MListItem *item, MListItem *nextItem )
		{
			link( prevItem, item);
			link( item, nextItem );
		};

		static void unlink( MListItem *itemToUnlink )
		{
			if( itemToUnlink->getPrev() )
			{
				itemToUnlink->getPrev()->setNext( itemToUnlink->getNext() );
			}
			if( itemToUnlink->getNext() )
			{
				itemToUnlink->getNext()->setPrev( itemToUnlink->getPrev() );
			}

			itemToUnlink->setPrev( 0 );
			itemToUnlink->setNext( 0 );
		}
};

#endif