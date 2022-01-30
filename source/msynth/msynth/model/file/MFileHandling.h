/*

  MFileHandling (C)2001-2002 MRoc hifiShock

*/

#ifndef __MFILEHANDLING
#define __MFILEHANDLING

#include <framework/treedocument/MSaxTreeDocument.h>
#include "../converter/MXMLConverter.h"
#include "../../../resource.h"

class MFileHandling
{
	protected:

		static char* FILE_VERSION;
		static MFileHandling *ivInstance;

		MFileHandling();

	public:

		~MFileHandling();

		String getVersion();

		bool saveMXLFile( String fileName, MTreeNode* ptRoot );
		bool openFile( String fileName, ISerializeable* callbackInit );

		static MFileHandling *getInstance();
		static void release();

	protected:

		MTreeNode* getRootNode( MTreeNode* node );
};

#endif

