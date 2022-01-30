/*

  (C) 2001 MRoc hifiShock

*/

#ifndef __IAbstractNoteList
#define __IAbstractNoteList

#include "MAbstractNote.h"

class IAbstractNoteList
{
	public:

		virtual int					getTypeId() = 0;

		virtual unsigned int		getMaxLength() = 0;
		virtual void				setMaxLength( unsigned int maxLength ) = 0;

		virtual IAbstractNote*		createDefaultNote() = 0;
};
#endif
