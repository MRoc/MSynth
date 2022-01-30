#ifndef __MSampleImport
#define __MSampleImport

#include <framework/MObject.h>

#include "INodeConverter.h"
#include "../MWave.h"

/** 
 * implementation if a INodeConverter importing
 * a MSynth samplebank into the MSynth xml file.
 */
class MSampleImport :
	public MObject,
	public INodeConverter
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

public:

	/**
	 * constructor
	 */
	MSampleImport();


	/**
	 * destructor
	 */
	virtual ~MSampleImport();

	/**
	 * query for interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;


	/**
	 * storing
	 */
	virtual MTreeNode* save();

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * does the xml conversion
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * returns a instance of this class
	 */
	static MObject* createInstance();

};

#endif