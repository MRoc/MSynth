#ifndef __MOneZeroFilter
#define __MOneZeroFilter

#include "MTransformer.h"

/**
 * implementation of a one zero filter
 *
 * y(n) = g * ( x(n) + a*x(n-1) )
 *
 * g = 1 / (1+|a|)
 *
 */
class MOneZeroFilter :
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
	MFloatControl* ivPtAControl;

	/**
	 * the Z^-1 buffer
	 */
	FP* ivArXZ;

public:

	/** 
	 * constructor
	 */
	MOneZeroFilter();

	/**
	 * destructor
	 */
	virtual ~MOneZeroFilter();

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