#ifndef __MElementRenamer
#define __MElementRenamer

#include <framework/MObject.h>

#include "INodeConverter.h"

/**
 * renames a element
 */
class MElementRenamer :
	public MObject,
	public INodeConverter
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the element's new name
	 */
	String ivNewName;

public:

	/**
	 * constructor
	 */
	MElementRenamer();

	/**
	 * destructor
	 */
	virtual ~MElementRenamer();

	/**
	 * converts the given node
	 */
	virtual void convert( MTreeNode* ptNode );

	/**
	 * queries for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * storing
	 */
	virtual MTreeNode* save();

	/**
	 * create a instanceof this class
	 */
	static MObject* createInstance();
};

#endif