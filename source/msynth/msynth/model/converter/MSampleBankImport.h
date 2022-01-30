#ifndef __MSampleBankImport
#define __MSampleBankImport

#include <framework/MObject.h>
#include <framework/xml/MXmlUtil.h>

#include "INodeConverter.h"

#include "../samplebank/MSampleBank.h"

/**
 * this clas imports a samplebank into a mxl file.
 * if a beatbox node was found, this converter loads
 * the samplebank and stores all samples as base64
 * encoded samples as child nodes of the beatboxnode.
 */
class MSampleBankImport :
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
	MSampleBankImport();

	/**
	 * destructor
	 */
	virtual ~MSampleBankImport();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * returns a string representation of this class
	 */
	virtual String toString();

	/**
	 * queries for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * storing
	 */
	virtual MTreeNode* save();

	/**
	 * does the conversion
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * returns a instance of this class
	 */
	static MObject* createInstance();
};

#endif