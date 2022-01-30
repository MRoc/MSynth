#ifndef __MAudioTrigger
#define __MAudioTrigger

#include <framework/map/MInt2ObjMap.h>
#include "MAudioFile.h"
#include "../note/mdefaultnote.h"

class MAudioTrigger :
	public MDefaultNote
{
protected:

	MAudioFile* ivPtFile;

public:

	MAudioTrigger();
	MAudioTrigger( MAudioFile* ptFile );
	MAudioTrigger( MAudioTrigger* ptTrigger );
	virtual ~MAudioTrigger();

	virtual void			setFile( MAudioFile* ptFile );
	virtual MAudioFile*		getFile();

	virtual MDefaultNote*	getCopy();
	virtual MDefaultNote*	createNewInstance();

	virtual bool			isOverlapping( IAbstractNote *note );
	virtual bool			wouldOverlapp( int index, unsigned int length );

	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );
};

#endif