#ifndef __MObjectFactory
#define __MObjectFactory

#include "treedocument/ISerializeable.h"

typedef std::map<String, MRtti*> StrRttiMap;
typedef StrRttiMap::iterator StrRttiIter;

class MObjectFactory :
	protected StrRttiMap
{
public:

	MObjectFactory();
	virtual ~MObjectFactory();

	virtual MObject* createObject( MTreeNode* ptNode );
	virtual MObject* createObject( String className );
	virtual MRtti* get( String key );

	virtual void store( MRtti* ptRtti );
};

#endif