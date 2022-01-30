#ifndef __MDynamics
#define __MDynamics

#include <framework/MObject.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/primitive/MArray.h>
#include <framework/primitive/MFloat.h>

#include "MDefaultNoteList.h"

/**
 * a dynamics attack line to be applied on a notelist
 */
class MDynamics :
	public MObject,
	public ISerializeable
{
protected:

	/**
	 * the gobal runtime type info
	 */
	static MRtti gvRtti;

	/**
	 * contains the dynamics values
	 */
	MDoubleArray ivValues;

	/**
	 * the dynamic's name
	 */
	String ivName;

public:

	/**
	 * constructor
	 */
	MDynamics();

	/**
	 * destructor
	 */
	virtual ~MDynamics();

	/**
	 * returns the runtime type information
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 * (inheritanced from IMUnknown)
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * returns the dynamic's name
	 */
	virtual String getName();

	/**
	 * applies the dynamics on the given notelist
	 */
	virtual void apply( MDefaultNoteList* pList );

	/**
	 * returns the indexed value
	 */
	virtual double get( unsigned int index );

	/**
	 * parses all dynamics files found in the given resource path
	 */
	static MObjectList* loadFromResource( String path );
};

#endif