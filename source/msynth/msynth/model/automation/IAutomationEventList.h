#ifndef __IAutomationEventList
#define __IAutomationEventList

#include "IEventTarget.h"

class IAutomationEventList :
	public ISerializeable
{
	public:

		virtual IEventTarget* getTarget() = 0;

		virtual void setLength( unsigned int length ) = 0;
		virtual unsigned int getLength() = 0;
		virtual void scale( unsigned int newLength ) = 0;

		virtual void setItem( IAutomationEvent* ptEvent, unsigned int index ) = 0;
		virtual IAutomationEvent* getItem( unsigned int index ) = 0;
		virtual unsigned int getItemCount() = 0;

		virtual void removeItem( unsigned int index ) = 0;
		virtual void removeItem( IAutomationEvent* ptEvent ) = 0;

		virtual void clear() = 0;
		virtual void glow( IAutomationEventList* ptEventList ) = 0;
		virtual IAutomationEventList* cut( unsigned int index ) = 0;

		virtual void release() = 0;
		virtual IAutomationEventList* clone() = 0;
};

#endif