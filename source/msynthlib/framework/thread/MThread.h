#ifndef __MThread
#define __MThread

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>

/**
 * win32 implementation of a thread
 */
class MThread
{
public:

	static const int TIMEOUT_FOR_TERMINATION;

	enum Priority
	{
		PRIORITY_TIME_CRITICAL,
		PRIORITY_HIGHEST,
		PRIORITY_ABOVE_NORMAL,
		PRIORITY_NORMAL,
		PRIORITY_BELOW_NORMAL,
		PRIORITY_LOWEST,
		PRIORITY_IDLE
	};

protected:

	SECURITY_ATTRIBUTES ivSecurityAttr;
	DWORD ivThreadId;
	HANDLE ivThreadHandle;
	Priority ivPriority;
	bool ivIsRunning;

public:

	MThread();
	virtual ~MThread();

	virtual bool start();
	virtual bool stop();

	virtual void run();

	virtual void setPrority( MThread::Priority p );

protected:

	static DWORD WINAPI threadProc( LPVOID lpParameter );

	virtual bool updatePriority();
};

#endif