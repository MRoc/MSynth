#ifndef __MTreeConverterDocument
#define __MTreeConverterDocument

#include <framework/MObject.h>

#include "MElementMatch.h"

typedef std::vector<MElementMatch*> NcList;
typedef NcList::iterator NcListIter;

typedef std::map<String, MElementMatch*> Str2NcMap;
typedef Str2NcMap::iterator Str2NcMapIter;

/**
 * a document able to convert a MTreeDocument
 */
class MTreeConverterDocument :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * a map storing the MTreeConverterEntrys under
	 * their element name. More than one value can
	 * point to the same entry, so delete this map.
	 */
	Str2NcMap ivElementMappings;

	/**
	 * list storing all used MTreeConverterEntrys once.
	 */
	NcList ivElements;

public:

	/**
	 * constructor
	 */
	MTreeConverterDocument();

	/**
	 * destructor
	 */
	virtual ~MTreeConverterDocument();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * create a instance of this class
	 */
	static MObject* createInstance();

	/**
	 * this method adds a MTreeConverterEntry to this converter
	 */
	void addEntry( MElementMatch* ptEntry );

	/**
	 * this method performs the conversion of a complete tree
	 */
	void convert( MTreeNode* ptNode );

	/**
	 * loading
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * saving
	 */
	virtual MTreeNode* save();

protected:

	/**
	 * used for recursive converting the tree
	 */
	void convertRecursive( MTreeNode* ptNode );
};

#endif