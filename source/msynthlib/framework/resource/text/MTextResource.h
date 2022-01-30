#ifndef __MTextResource
#define __MTextResource

#include "../../MObject.h"
#include "../../runtime/MRtti.h"
#include "../../treedocument/MSaxTreeDocument.h"
#include "ITextResource.h"

/**
 * implementation of the text resource
 */
class MTextResource :
	public MObject,
	public ITextResource
{
protected:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

	/**
	 * the singleton instance
	 */
	static MTextResource* gvInstance;

	/**
	 * the string table
	 */
	StrStrMap ivStringTable;

public:

	/**
	 * constructor
	 */
	MTextResource();

	/**
	 * destructor
	 */
	virtual ~MTextResource();

	/**
	 * return the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * translates the given string and inserts the given values
	 */
	virtual String getString( String id, ... );

	/**
	 * loads the text resource from xml
	 */
	virtual void load( String resourceName );

	/**
	 * loads the string table from tree
	 */
	virtual void load( MTreeNode* pNode );

	/**
	 * returns the singleton instance
	 */
	static MTextResource* getInstance();

	/**
	 * frees the singleton instance
	 */
	static void release();
};

#endif