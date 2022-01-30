#ifndef __MAppImpl
#define __MAppImpl

class IRunnable;
class MTopWnd;

class MAppImpl
{
public:

	/** constructor */
	MAppImpl(){}

	/** destructor */
	virtual ~MAppImpl(){}

	/** sets the quit flag on true */
	virtual void setQuit() = 0;

	/** the message loop, returns the exit code */
	virtual int work() = 0;

	/** the message loop for modal stack, the return code is the code  */
	virtual int modalWork() = 0;

	/** the code behind pToRun will be invoked from ui thread */
	virtual void doWorkInGuiThread( IRunnable* pToRun ) = 0;
};

#endif