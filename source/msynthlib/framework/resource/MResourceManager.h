#ifndef __MResourceManager
#define __MResourceManager

#include "IMResourceManager.h"

typedef std::map<String, IFolder*> Str2FolderMap;
typedef Str2FolderMap::iterator Str2FolderMapIter;

typedef std::map<ISeekableInputStream*, IFolder*> InStream2FolderMap;
typedef InStream2FolderMap::iterator InStream2FolderMapIter;

typedef std::map<ISeekableOutputStream*, IFolder*> OutStream2FolderMap;
typedef OutStream2FolderMap::iterator OutStream2FolderMapIter;

/** 
 * the resource manager is a singleton instance
 * for opening input streams into a previously
 * registered folder
 */
class MResourceManager :
	public IMResourceManager
{
protected:

	/**
	 * the singleton instance
	 */
	static MResourceManager* g_pInstance;

	/** 
	 * folder map storing the folders under their name
	 */
	Str2FolderMap m_folderMap;

	/**
	 * stores all outgoin ISeekableInputStreams (key) and
	 * their IFolder creators (value) to call
	 * the right close function for the stream
	 * in the right IFolder
	 */
	InStream2FolderMap m_instreamMap;

	/**
	 * stores all outgoin ISeekableInputStreams (key) and
	 * their IFolder creators (value) to call
	 * the right close function for the stream
	 * in the right IFolder
	 */
	OutStream2FolderMap m_outstreamMap;

	/** 
	 * constructor
	 */
	MResourceManager();

	/** 
	 * destructor
	 */
	virtual ~MResourceManager();
	
public:

	/**
	 * returns a pointer to the singleton instance
	 */
	static IMResourceManager* getInstance();

	/**
	 * releases the singletion instance
	 */
	static void release();

	/**
	 * adds a folder to the resource manager
	 * (inheritanced from IResourceManager)
	 */
    virtual void loadArchive( IFolder* pFolder );

	/**
	 * registers a directory as archive,
	 * be carefull with this, because a big directory tree
	 * will take a big amount of reserved memory when beeing
	 * registered here!!!
	 */
	virtual void loadArchive( String pathOnDisk, String aliasName );

	/** 
	 * this function loads specified resource into memory.
	 * it allocates memory for pDataOut, writes the data into
	 * it and outputs the datalength of the buffer by pDataLenOut.
	 *
	 * the caller of this function is responsible for deleting
	 * the allocates memory!!!
	 */
	virtual void loadResource( String pathNNameIn, unsigned char** pDataOut, unsigned int* pDataLenOut );

	/**
	 * opens a file in a registered archive.
	 *
	 * the parameter pathNName sould be as follows:
	 *
	 * archiveName/[path/]filename[.suffix]
	 *
	 * (inheritanced from IResourceManager)
	 */
	virtual ISeekableInputStream* openReadFile( String pathNName );

	/** 
	 * closes a stream previously created by openReadFile(...)
	 * (inheritanced from IResourceManager)
	 */
	virtual void close( ISeekableInputStream* pInStream );

	/**
	 * opens a output stream to write into
	 */
	virtual ISeekableOutputStream* openWriteFile( String pathNName );

	/** 
	 * closes a stream previously created by openReadFile(...)
	 */
	virtual void close( ISeekableOutputStream* pInStream );

	/**
	 * returns a folder encapsulating the given folder
	 */
	virtual IFolder* getFolder( String name );

	/**
	 * returns the full filename into the resource
	 */
	virtual String getFile( String resourceName );

protected:

	/**
	 * divides the given path by / and \
	 */
	StrList divide( String path );
};

#endif