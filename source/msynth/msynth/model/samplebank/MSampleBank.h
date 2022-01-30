#ifndef __MSampleBank
#define __MSampleBank

#include <framework/mobject.h>
#include <framework/list/mobjectlist.h>
#include "MSampleBankEntry.h"

class MSampleBank :
	private MObjectList
{
public:

	static const char* CLASS_NAME;

public:

	MSampleBank();
	virtual ~MSampleBank();

	virtual String getClassName();

	virtual String toString();

	virtual void addSample( MSampleBankEntry* ptSample );
	virtual void removeSample( MSampleBankEntry* ptSample );

	virtual unsigned int getSampleCount();
	virtual MSampleBankEntry* getSample( unsigned int index );

	virtual void import( String filename );
	virtual void export( String filename );

	virtual void clearButLeaveSamples();
};

#endif