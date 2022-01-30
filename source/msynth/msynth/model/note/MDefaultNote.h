/*
	(C)2001-2003 MRoc hifisShock

	Default note is a note only for monophonic synths
*/

#ifndef __MDEFAULTNOTE
#define __MDEFAULTNOTE

#include "MAbstractNote.h"
#include <framework/MUtil.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/primitive/MInt.h>
#include <framework/primitive/MFloat.h>

class MDefaultNote :
	public MListItem,
	public IAbstractNote,
	public ISerializeable
{
protected:

	unsigned int ivDuration;

public:

	MDefaultNote();
	MDefaultNote( MDefaultNote* ptNote );
	virtual ~MDefaultNote();

	virtual void			setStartsAt( int startsAt );
	virtual int				getStartsAt();

	// The duration, the length before release or another note
	virtual void			setDuration( unsigned int duration );
	virtual unsigned int	getDuration();

	// Stops at, calculated from starts at and duration
	virtual int				getStopsAt();
	virtual void			setStopsAt( int stopsAt );

	virtual MDefaultNote*	getCopy() = 0;
	virtual MDefaultNote*	createNewInstance() = 0;
};
#endif