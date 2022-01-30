#ifndef __MOnePoleFilter
#define __MOnePoleFilter

#include "MTransformer.h"

/**
 * implementation of a one pole filter
 *
 * y(n) = g * x(n) + r*y(n-1) )
 *
 * g = 1 / (1+|a|)
 *
 */
class MOnePoleFilter :
	public MTransformer
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the amount of Z^-1 added to the output function
	 */
	MFloatControl* ivPtRControl;

	/**
	 * the Z^-1 buffer
	 */
	FP* ivArRZ;

public:

	/** 
	 * constructor
	 */
	MOnePoleFilter();

	/**
	 * destructor
	 */
	virtual ~MOnePoleFilter();

	/**
	 * creates all buffers required by the effect
	 */
	virtual bool create( unsigned int channelCount );

	/**
	 * process event
	 */
	virtual void processEvent( MEvent* ptEvent );

	/**
	 * renders the given amount of data
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	/**
	 * returns the number of controls
	 */
	virtual unsigned int getControlCount();

	/** 
	 * returns the control at the specified index
 	 */
	virtual IControl* getControl( unsigned int index );

	/**
	 * returns the name of the transformer
	 */
	virtual String getName();

	/**
	 * return the short name of the filter
	 */
	virtual String getShortName();

	/** 
 	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * creates a instance of this class
	 */
	static MObject *createInstance();
};

#endif