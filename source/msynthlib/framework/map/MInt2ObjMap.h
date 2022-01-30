#ifndef __MInt2ObjMap
#define __MInt2ObjMap

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"

typedef std::map<int, MObject*> Int2ObjMap;
typedef Int2ObjMap::iterator Int2ObjMapIter;

/**
 * implementation of a map/dictionary/hastable
 * with int as key and MObject as value
 */
class MInt2ObjMap :
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
	 * the internale map used
	 */
	Int2ObjMap ivMap;

public:

	/**
	 * constructor
	 */
	MInt2ObjMap();

	/**
	 * destructor, deleting every stored item
	 */
	virtual ~MInt2ObjMap();

	/**
	 * stores a value under the specified key
	 */
	virtual void store( int key, MObject* value );

	/**
	 * returns the value at the specified key
	 */
	virtual MObject* get( int key );

	/**
	 * returns true if the key is found
	 */
	virtual bool contains( int key );

	/**
	 * deletes all stored objects
	 */
	virtual void deleteAll();

	/**
	 * clears all stored objects without deleting them
	 */
	virtual void clear();

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
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