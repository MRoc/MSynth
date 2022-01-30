#ifndef __MAbstractInstrumentListener
#define __MAbstractInstrumentListener

#include <framework/listener/IListener.h>

class MInstrumentEvent
{
	protected:

		int ivMessageType;
		int ivValue, ivValue2;

	public:

		MInstrumentEvent( int messageType )
		{
			ivMessageType = messageType;
			ivValue = -1;
			ivValue2 = -1;
		};

		MInstrumentEvent( int messageType, int value )
		{
			ivMessageType = messageType;
			ivValue = value;
			ivValue2 = -1;
		};

		MInstrumentEvent( int messageType, int value, int value2 )
		{
			ivMessageType = messageType;
			ivValue = value;
			ivValue2 = value2;
		};

		~MInstrumentEvent()
		{
		};

		int getMessageType()
		{
			return ivMessageType;
		};

		int getValue()
		{
			return ivValue;
		};

		int getValue2()
		{
			return ivValue2;
		};
};

class IInstrumentListener :
	public IListener
{
	public:

		virtual void stateChanged( MInstrumentEvent anEvent ) = 0;
		virtual String getInstrumentListenerName() = 0;
};

#endif