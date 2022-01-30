#ifndef __IMResourceManager
#define __IMResourceManager

#include "IFolder.h"

class IMResourceManager
{
public:

	/**
	 * adds a folder to the resource manager
	 */
    virtual void loadArchive( IFolder* pFolder ) = 0;

	/**
	 * registers a directory as archive,
	 * be carefull with this, because a big directory tree
	 * will take a big amount of reserved memory when beeing
	 * registered here!!!
	 */
	virtual void loadArchive( String pathOnDisk, String aliasName ) = 0;

	/**
	 * opens a file in a registered archive.
	 *
	 * the parameter pathNName sould be as follows:
	 *
	 * archiveName/[path/]filename[.suffix]
	 */
	virtual ISeekableInputStream* openReadFile( String pathNName ) = 0;
	
	/** 
	 * closes a stream previously created by openReadFile(...)
	 */
	virtual void close( ISeekableInputStream* pInStream ) = 0;

	/**
	 * opens a output stream to write into
	 */
	virtual ISeekableOutputStream* openWriteFile( String pathNName ) = 0;

	/** 
	 * closes a stream previously created by openReadFile(...)
	 */
	virtual void close( ISeekableOutputStream* pInStream ) = 0;

	/** 
	 * this function loads specified resource into memory.
	 * it allocates memory for pDataOut, writes the data into
	 * it and outputs the datalength of the buffer by pDataLenOut.
	 *
	 * the caller of this function is responsible for deleting
	 * the allocates memory!!!
	 */
	virtual void loadResource( String pathNNameIn, unsigned char** pDataOut, unsigned int* pDataLenOut ) = 0;

	/**
	 * returns a folder encapsulating the given folder
	 */
	virtual IFolder* getFolder( String name ) = 0;

	/**
	 * returns the full filename into the resource
	 */
	virtual String getFile( String resourceName ) = 0;
};

#endif