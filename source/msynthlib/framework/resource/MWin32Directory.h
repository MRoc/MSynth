#ifdef WIN32

#ifndef __MWin32Directory
#define __MWin32Directory

#include "IFolder.h"
#include "../MObject.h"
#include "../runtime/MRtti.h"

/**
 * a win32 deepnding implementation of the win32 folder
 */
class MWin32Directory :
	public MObject,
	public IFolder
{
protected:

	/**
	 * runtime type info
	 */
	static MRtti g_rtti;

	/** 
	 * a link to a path on the disk
	 */
	String m_path;

	/** 
	 * the name of the folder
	 */
	String m_name;

	/**
	 * a map storing strings of child folders
	 * as key and null or a allready created
	 * IFolder as value
	 */
	Str2ObjMap m_folders;

	/**
	 * list of all child file names
	 */
	StrList m_files;

public:

	/**
	 * constructor 1
	 */
	MWin32Directory();

	/**
	 * constructor 2
	 */
	MWin32Directory( String path, String name );

	/** 
	 * destructor
	 */
	virtual ~MWin32Directory();

	/**
	 * sets the data if constructor 1 was invoked
	 */
	virtual void setData( String path, String name );

	/** 
	 * returns the runtime type info
	 */
	virtual MRtti* getRtti() const;

	/** 
	 * querys for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the directory's name
	 */
	virtual String getName();

	/** 
	 * returns the number of child files in this folder
	 */
	virtual unsigned int getFolderCount();

	/** 
	 * returns the folder at the specified index
	 */
	virtual String getFolder( unsigned int index );

	/**
	 * returns a folder encapsulating the given folder
	 */
	virtual IFolder* getFolder( String name );

	/**
	 * returns the number of files contained in this folder
	 */
	virtual unsigned int getFileCount();

	/** 
	 * returns the file at the specified index contained in this folder
	 */
	virtual String getFile( unsigned int index );

	/**
	 * opens a existing file for reading
	 */
	virtual ISeekableInputStream* openReadFile( String fileName );

	/** 
	 * closes the input stream
	 */
	virtual void close( ISeekableInputStream* pIn );

	/**
	 * opens a output stream
	 */
	virtual ISeekableOutputStream* openWriteFile( String fileName );

	/** 
	 * closes a output stream
	 */
	virtual void close( ISeekableOutputStream* pIn );

	/** 
	 * reads the content of the specified
	 * resource into the pDataOut buffer.
	 *
	 * the calles is responsible for deleting
	 * the allocated memory!!!
	 */
	virtual void loadResource( String fileName, unsigned char** pDataOut, unsigned int* pDataLenOut );

	/** 
	 * free instance
	 */
	virtual void release();

	/**
	 * returns the path on disk
	 */
	virtual String getPath();

protected:

	/** 
	 * updates the folder and child lists
	 */
	void updateFromPath();

	/** 
	 * checks ONE result of a win32 directory
	 * search and inserts the found item
	 * into the folder or file list.
	 */
	void insert( void* pvAttr );

	/**
	 * sets a folder for caching
	 */
	void setFolder( String folderName, IFolder* pFolder );
};

#endif

#endif