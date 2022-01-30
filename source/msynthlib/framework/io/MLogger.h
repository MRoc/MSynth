#ifndef __MLogger
#define __MLogger

#include "../MTypes.h"
#include "MFilePrinter.h"

class MLogger
{
private:

	static MLogger* g_pInstance;
	MFilePrinter* m_pOut;

	MLogger();
	MLogger( String logFile );
	virtual ~MLogger();

public:

	static void init( String fileName );
	static void release();

	static void logError( String error,... );

	static void logWarning( String warning,... );

	static void logMessage( String msg,... );
};

#endif