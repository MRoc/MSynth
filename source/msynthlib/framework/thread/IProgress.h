#ifndef __IProgress
#define __IProgress

/**
 * defines an interface for a callback from a working thread.
 * the working thread allways says, how long it will take 
 * until finishing the job.
 */
class IProgress
{
public:

	/**
	 * invoked to show the progress of a job
	 * 0.0 <= value <= 1.0
	 */
	virtual void onProgress( float value ) = 0;

	/**
	 * invoked when the job was finished
	 */
	virtual void onFinished() = 0;
};

#endif