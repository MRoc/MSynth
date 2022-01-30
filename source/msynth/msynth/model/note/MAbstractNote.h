/*
	(C)2001 MRoc hifiShock

	Base class of all notes
*/

#ifndef __IAbstractNote
#define __IAbstractNote

#include "../list/MListItem.h"

class IAbstractNote
{
public:
	
	virtual void			setStartsAt( int startsAt ) = 0;
	virtual int				getStartsAt() = 0;

	virtual void			setDuration( unsigned int duration ) = 0;
	virtual unsigned int	getDuration() = 0;

	virtual bool			isOverlapping( IAbstractNote *note ) = 0;
	virtual bool			wouldOverlapp( int index, unsigned int length ) = 0;
};
#endif