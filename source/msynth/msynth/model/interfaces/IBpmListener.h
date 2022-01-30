#ifndef __IBpmListener
#define __IBpmListener

class IBpmListener
{
public:

	virtual void bpmChanged( unsigned int bpm ) = 0;
};

class IBpmControl
{
public:

	virtual void setBpm( unsigned int bpm ) = 0;
	virtual unsigned int getBpm() = 0;
	virtual void addBpmListener( IBpmListener* ptListener ) = 0;
	virtual void removeBpmListener( IBpmListener* ptListener ) = 0;
};

#endif