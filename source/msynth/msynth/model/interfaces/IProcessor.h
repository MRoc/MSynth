/**
 * (C)2000-2003 MRoc
 */

#ifndef __IProcessor
#define __IProcessor

#include "../interfaces/MSoundBuffer.h"
#include "../event/MEvent.h"

/**
 * super interface of all objects processing audio data
 */
class IProcessor
{
public:
	/**
	 * fire a event into the processor
	 */
	virtual void processEvent( MEvent* ptEvent ) = 0;

	/**
	 * processes the given data
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt) = 0;
};

#endif