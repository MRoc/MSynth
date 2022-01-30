/*

	MProperties (C)2001 MRoc hifiShock

*/

#ifndef __MProperties
#define __MProperties

#include "MStr2ObjMap.h"

class MProperties :
	public MStr2ObjMap
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

private:

	static MProperties*		gvInstance;

public:

	/**
	 * constructor
	 */
	MProperties();

	/**
	 * destructor
	 */
	virtual ~MProperties();

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	virtual String			toString() const;

	virtual int				_getInt( String key, int defaultValue );
	virtual bool			_getBool( String key, bool defaultValue );

	virtual void			load( String fileName );
	virtual void			save( String fileName );

	virtual void			store( String key, MObject* value );

// STATICS

	static void				loadProperties( String fileName );
	static void				saveProperties( String fileName );


	static MProperties*		getInstance();
	static void				release();

	static void				storeInt( String key, int value );
	static int				getInt( String key, int defaultValue );
	static int				getInt( String key, int defaultValue, int min, int max );

	static void				storeBool( String key, bool value );
	static bool				getBool( String key, bool defaultValue );

	static void				storeString( String key, String value );
	static String			getString( String key, String defaultValue );	

	static MObject*			createInstance();
};

#endif