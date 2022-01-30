#ifndef __MWndNamespace
#define __MWndNamespace

#include <framework/MTypes.h>

class MWnd;
class MWndCollection;

typedef std::map<String, MObject*> StrObjMap;
typedef StrObjMap::iterator StrObjMapIter;
typedef std::map<MObject*, String> ObjStrMap;
typedef ObjStrMap::iterator ObjStrMapIter;

/** 
 *
 * a wnd namespace is a "id <-> object" relation.
 *
 * a wnd container can have a object namespace.
 *
 * objects in namespace can be resolved by id.
 * objects can be registered in namespace with a id.
 *
 * the id in a namespace must be unique.
 * a namespace has a owning window collection.
 *
 * it provides methods to register objects in available namespaces.
 * provides methods to resolve a reference to a object by id.
 */
class MWndNamespace
{
protected:

	/** stores id(MString) to window(MWnd*) relations */
	StrObjMap ivId2Wnd;

	/** stores windows(MWnd*) to id(MString) relations */
	ObjStrMap ivWnd2Id;

	/** the owning window collection */
	MWndCollection* ivPtOwner;

public:

	/** constructor */
	MWndNamespace( MWndCollection* pOwner );

	/** destructor */
	virtual ~MWndNamespace();

	/** registers a object at the context */
    virtual void regObj( MObject* pObj, const String& id );

	/** unregisters a window from the context */
	virtual void unregObj( MObject* pObj );

	/** returns a window from the context */
	virtual MObject* getObj( const String& id );

	/** returns the id of a window */
	virtual String getId( MObject* pObj );

	/** returns a namespace for the given control if available */
	static MWndNamespace* getNamespace( MWnd* pControl );

	/** registers a object in the next available namespace, returns false if failed */
	static bool regObjNS( MObject* pObj, MWnd* pControl, const String& id );

	/** unregisters a object in the next available namespace, returns false if failed */
	static bool unregObjNS( MObject* pObj, MWnd* pControl );

	/** returns the object in the given namespace, if available */
	static MObject* getObjNS( MWndCollection* pCollection, const String& id );

	/** returns the object in the given namespace or in upper namespaces, if available */
	static MObject* getObjFullNS( MWndCollection* pCollection, const String& id );
};

#endif