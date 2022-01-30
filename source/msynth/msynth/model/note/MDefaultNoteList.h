#ifndef __MDefaultNoteList
#define __MDefaultNoteList

#include <framework/treedocument/ISerializeable.h>
#include "../../ui/clipboard/ICopyCutPaste.h"

#include "MDefaultNote.h"
#include "MAbstractNoteList.h"

#include "../list/MList.h"
#include "../list/MListUtil.h"
#include "../interfaces/MInstrument.h"
#include "../MAbstractInstrumentListener.h"

class MInstrument;

class MDefaultNoteList :
	public MList,
	public IAbstractNoteList,
	public ISerializeable,
	public ICopyCutPaste
{
protected:

	unsigned int				ivMaxLength;

public:

	MDefaultNoteList();
	MDefaultNoteList( unsigned int maxLength );
	MDefaultNoteList( MDefaultNoteList* ptNoteList );
	virtual ~MDefaultNoteList();

	virtual void				applyDynamics( unsigned int index, FP value ) = 0;

	virtual void				updateFromInstrument( MInstrument* ptInstrument );

	virtual unsigned int		getMaxLength();
	virtual void				setMaxLength( unsigned int maxLength );

	virtual void				changeResolution( unsigned int nexMaxLength );
	virtual void				changeMaxLength( unsigned int newMaxLength );

	virtual bool				isFree( unsigned int index );

	virtual MListItem*			getItemAt( unsigned int index );
	virtual MListItem*			getItemStopsAt( unsigned int index );
	virtual void				insertAt( MList* ptList, unsigned int index, unsigned int newLength );
	virtual MDefaultNoteList*	cutAt( unsigned int index );

	virtual IAbstractNote*		createDefaultNote() = 0;

	virtual MColor				getPreferedColor() = 0;					

	virtual IInstrumentListener* getInstrumentListener();

	virtual MDefaultNoteList*	createNewInstance() = 0;

	virtual int					getTypeId() = 0;
	virtual ICopyCutPaste*		getCopy() = 0;

	virtual	void				release();

	virtual bool				hasVolumeControl();
};

#endif