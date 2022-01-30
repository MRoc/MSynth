#ifndef __MEventPortamento
#define __MEventPortamento

#include "MEventNoteOn.h"

class MEventPortamento :
	public MEventNoteOn
{
protected:

	/** 
	 * the length of the portamento in samples
	 */
	unsigned int ivPortamentoLength;

public:

	/** 
	 * constructor
	 *
	 * frequency - the target frequency in Hz
	 * portamentoLength - the length of the portamento in samples
	 *
	 */
	MEventPortamento( FP frequency, unsigned int portamentoLength );

	/**
	 * destructor
	 */
	virtual ~MEventPortamento();

	/**
	 * returns the length of the portamento in samples
	 */
	virtual unsigned int getPortamentoLength();
};

#endif