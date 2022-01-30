/*

	(C)2000/2001 MRoc hifiShock

*/

#ifndef __MXMLCONVERTER
#define __MXMLCONVERTER

#pragma warning(disable:4786)

#include <framework/MTypes.h>
#include <framework/treedocument/MSaxTreeDocument.h>

#include "MTreeConverterDocument.h"

#include "../../../resource.h"
#include "../../../MAppData.h"

/**
 * the singleton tree converter
 */
class MXMLConverter
{
protected:

	/**
	 * a selection of tree converter documents
	 */
	MObjectList ivConverterDocuments;

	/**
	 * the singleton instance
	 */
	static MXMLConverter* gvInstance;

public:

	/**
	 * constructor
	 */
	MXMLConverter();

	/**
	 * destructor
	 */
	virtual ~MXMLConverter();
	
	/**
	 * formats the given xml
	 */
	virtual void formatXML( MTreeNode* ptNode , unsigned int versionTag );

	/**
	 * returns the singleton instance
	 */
	static MXMLConverter* getInstance();

	/**
	 * releases the singleton instance if nescessary
	 */
	static void release();

protected:

	/**
	 * loads all import maps
	 */
	virtual void loadImportDocuments();

	/**
	 * loads a specific import map
	 */
	void loadImportDocument( String filename );
};

#endif

