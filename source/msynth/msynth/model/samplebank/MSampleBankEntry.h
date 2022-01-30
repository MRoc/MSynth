#ifndef __MSampleBankEntry
#define __MSampleBankEntry

#include <framework/mobject.h>
#include "../MWave.h"

/**
 * implementation of a sample bank entry,
 * one sample plus samplename
 */
class MSampleBankEntry :
	public MObject
{
public:
	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the wave data
	 */
	MWave* ivPtWave;

	/**
	 * the sample name
	 */
	String ivName;

public:

	/**
	 * constructor
	 */
	MSampleBankEntry();

	/**
	 * constructor2
	 */
	MSampleBankEntry( MWave* ptWave );

	/**
	 * destructor
	 */
	virtual ~MSampleBankEntry();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns a string representation
	 */
	virtual String toString();

	/**
	 * returns the stored wave
	 */
	virtual MWave* getWave();

	/**
	 * sets the stored wave
	 */
	virtual void setWave( MWave* ptWave );

	/**
	 * returns the sample name
	 */
	virtual String getName();

	/**
	 * sets the sample name
	 */
	virtual void setName( String name );

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();
};

#endif