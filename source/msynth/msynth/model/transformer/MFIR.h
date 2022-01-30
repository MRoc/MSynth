#ifndef __MFIR
#define __MFIR

#include <framework/MObject.h>
#include <framework/primitive/MArray.h>
#include <framework/primitive/MCircularBuffer.h>
#include "MTransformer.h"

/**
 * a fir filter filtering 
 */
class MFIR :
	public MTransformer,
	public IControlListener
{
protected:

public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the coeff array
	 */
	MFloatArray ivCoeffs;

	/**
	 * data buffer
	 */
	MFloatCircularBuffer ivBuffer;

	/**
	 * cutoff frequency control
	 */
	MFloatControl* ivPtCutoff;

public:

	/**
	 * constructor
	 */
	MFIR();

	/** 
	 * destructor
	 */
	virtual ~MFIR();

	/**
	 * calculates new filter coeffs
	 */
	virtual void calculateCoeffs( float normalizedCutoffFrequency, unsigned int coeffsLength = 9 );

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();

	/** 
	 * returns the number of embedded controls
	 */
	virtual unsigned int getControlCount();

	/**
	 * returns a embedded control by index
	 */
	virtual IControl* getControl( unsigned int index );

	/**
	 * does the filtering
	 */
	virtual void goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	/**
	 * returns the name
	 */
	virtual String getName();

	/** 
	 * returns the short name
	 */
	virtual String getShortName();

	/**
	 * invoked when value changed
	 */
	virtual void valueChanged( MControlListenerEvent *anEvent );
};

#endif