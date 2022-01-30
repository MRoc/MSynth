#ifndef __MLayoutRest
#define __MLayoutRest

#include "MLayoutLeaf.h"
#include <framework/treedocument/ISerializeable.h>

class MLayoutRest :
	public MLayoutLeaf,
	public ISerializeable
{
public:

	static MRtti gvRtti;

	enum Type
	{
		VERTICAL,
		HORIZONTAL,
		BOTH
	};

protected:

	Type ivType;

public:

	MLayoutRest();
	virtual ~MLayoutRest();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual MSize getSize( MyWnd* ptWnd, MSize maxSize );
	virtual void doLayout( MyWnd* ptWnd, MRect rect );

	virtual String type2str( Type type );
	virtual Type str2Type( String type );

	static MObject* createInstance();
};

#endif