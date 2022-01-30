#ifndef __MStr2ObjMap
#define __MStr2ObjMap

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"

typedef std::map<String, MObject*> Str2ObjMap;
typedef Str2ObjMap::iterator Str2ObjMapIter;

class MStr2ObjMap :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

protected:

	/**
	 * the internal used map
	 */
	Str2ObjMap ivMap;

public:

	/**
	 * constructor
	 */
	MStr2ObjMap();

	/**
	 * destructor deleting all stored objects
	 */
	virtual ~MStr2ObjMap();

	/**
	 * stores a MObject under the specified key
	 */
	virtual void store( String key, MObject* value );

	/**
	 * removes the MObject specified by key
	 */
	virtual MObject* remove( String key );

	/**
	 * returns the MObject stored under the key
	 */
	virtual MObject* get( String key );

	/**
	 * returns true if the key is found
	 */
	virtual bool contains( String key );

	/**
	 * deletes all stored objects
	 */
	virtual void deleteAll();

	/**
	 * clears the map
	 */
	virtual void clear();


	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
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
	 * creates an instance of this class
	 */
	static MObject* createInstance();
};

#endif