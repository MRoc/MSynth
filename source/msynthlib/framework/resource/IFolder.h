#ifndef __IFolder
#define __IFolder

#include "../MTypes.h"
#include "../IMUnknown.h"
#include "../io/ISeekableInputStream.h"
#include "../io/ISeekableOutputStream.h"

/**
 * definnition of a folder interface for browsing a file tree
 */
class IFolder :
	public IMUnknown
{
public:

	/**
	 * returns the directory's name
	 */
	virtual String getName() = 0;

	/** 
	 * returns the number of child files in this folder
	 */
	virtual unsigned int getFolderCount() = 0;

	/** 
	 * returns the folder at the specified index
	 */
	virtual String getFolder( unsigned int index ) = 0;

	/**
	 * returns the folder object with the specified name,
	 * if none exists, a new one is creates. if the
	 * given name does not exist in file structure,
	 * NULL is returned.
	 */
	virtual IFolder* getFolder( String name ) = 0;

	/**
	 * returns the number of files contained in this folder
	 */
	virtual unsigned int getFileCount() = 0;

	/** 
	 * returns the file at the specified index contained in this folder
	 */
	virtual String getFile( unsigned int index ) = 0;

	/**
	 * opens a existing file for reading
	 */
	virtual ISeekableInputStream* openReadFile( String fileName ) = 0;

	/** 
	 * closes the input stream
	 */
	virtual void close( ISeekableInputStream* pIn ) = 0;

	/**
	 * opens a output stream
	 */
	virtual ISeekableOutputStream* openWriteFile( String fileName ) = 0;

	/** 
	 * closes a output stream
	 */
	virtual void close( ISeekableOutputStream* pIn ) = 0;

	/** 
	 * reads the content of the specified
	 * resource into the pDataOut buffer.
	 *
	 * the calles is responsible for deleting
	 * the allocated memory!!!
	 */
	virtual void loadResource( String fileName, unsigned char** pDataOut, unsigned int* pDataLenOut ) = 0;

	/**
	 * frees the instance
	 */
	virtual void release() = 0;

	/**
	 * returns the path on disk
	 */
	virtual String getPath() = 0;
};

#endif