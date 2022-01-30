#ifndef __MDefaultSynthNote
#define __MDefaultSynthNote

#include "MDefaultNote.h"

class MDefaultSynthNote : public MDefaultNote
{
protected:

	FP ivVolume;
	int ivValue;

public:

	MDefaultSynthNote();
	MDefaultSynthNote( MDefaultSynthNote* ptNote );

	virtual ~MDefaultSynthNote();

	// The value this note contains
	virtual void			setValue( int value );
	virtual int				getValue();

	// The volume this note is playes
	virtual FP				getVolume();
	virtual void			setVolume( FP volume );

	virtual bool			isOverlapping( IAbstractNote *note );
	virtual bool			wouldOverlapp( int index, unsigned int length );

	virtual bool			needsFalldown();
	virtual bool			existsOverlappingAtEnd();

	virtual MTreeNode*		save();
	virtual void			load( MTreeNode* node );

	virtual MDefaultNote* getCopy();

	virtual MDefaultNote*	createNewInstance();
};
#endif