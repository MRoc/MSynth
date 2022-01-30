#ifndef __MTreeDocumentUtils
#define __MTreeDocumentUtils

#include "MTreeNode.h"

/** some utils to be used with a document tree */
class MTreeDocumentUtils
{
public:

	/** creates a mobject from the given node */
	static MObject* createObject( MTreeNode* pNode );

	/** parses and creates the objects in the given xml file */
	static MObject* parse( String fileName );

	/** recursive print the node */
	static void store( MTreeNode* pNode, String fileName );
};

#endif