#ifndef __MFilePrinter
#define __MFilePrinter

#include <fstream>
#include "IPrintStream.h"

class MFilePrinter :
	public IPrintStream
{
protected:

	FILE* ivPtFile;

public:

	MFilePrinter();
	virtual ~MFilePrinter();

	virtual bool open( String filename );
	virtual bool close();

	virtual void print( String str );
	virtual void println( String str );

	virtual void print( int i );
	virtual void println( int i );

	virtual void print( FP i );
	virtual void println( FP i );
};

#endif