#ifdef WIN32

#include "MWin32Directory.h"
#include "../io/MLogger.h"
#include "../io/MFileInputStream.h"
#include "../io/MFileOutputStream.h"
#include "../io/MIoException.h"
#include "../MNativeTypes.h"


/**
 * runtime type info
 */
MRtti MWin32Directory::g_rtti = MRtti( "MWin32Directory", 0 );

/**
 * constructor
 */
MWin32Directory::MWin32Directory() :
	m_path( "" ),
	m_name( "" )
{
}

/**
 * constructor
 */
MWin32Directory::MWin32Directory( String path, String name ) :
	m_path( path ),
	m_name( name )
{
	this->updateFromPath();
}

/** 
 * destructor
 */
MWin32Directory::~MWin32Directory()
{
	for(
		Str2ObjMapIter i=m_folders.begin();
		i != m_folders.end();
		i++ )
	{
		if( i->second )
			delete i->second;
	}

	m_folders.clear();
	m_files.clear();
}

/**
 * sets the data if constructor 1 was invoked
 */
void MWin32Directory::setData( String path, String name )
{
	m_path = path;
	m_name = name;
	updateFromPath();
}

/** 
 * returns the runtime type info
 */
MRtti* MWin32Directory::getRtti() const
{
	return &g_rtti;
}

/** 
 * querys for a interface
 */
void* MWin32Directory::getInterface( const String &className ) const
{
	if( className == "MWin32Directory" )
		return (void*) ((MWin32Directory*)this);
	else if( className == "IFolder" )
		return (void*) ((IFolder*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the directory's name
 */
String MWin32Directory::getName()
{
	return this->m_name;
}

/** 
 * returns the number of child files in this folder
 */
unsigned int MWin32Directory::getFolderCount()
{
	return (unsigned int) this->m_folders.size();
}

/**
 * returns the folder at the specified index
 */
String MWin32Directory::getFolder( unsigned int index )
{
	String back = "";

	unsigned int counter = 0;
	for(
		Str2ObjMapIter i=m_folders.begin();
		i != m_folders.end();
		i++, counter++ )
	{
		if( counter == index )
		{
			back = i->first;
			break;
		}
	}

	return back;
}

/**
 * returns a folder encapsulating the given folder
 */
IFolder* MWin32Directory::getFolder( String name )
{
	IFolder* pBack = 0;

	// exists a folder was found matching the given name?
	Str2ObjMapIter i = m_folders.find( name );
	if( i != m_folders.end() )
	{
		// no folder object existing?
		if( i->second == 0 )
		{
			// create new one...
			pBack = new MWin32Directory( m_path + "/" + name, name );
			setFolder( name, pBack );
			pBack = (IFolder*) m_folders.find( name )->second->getInterface( "IFolder" );
		}
		else
		{
			// fetch existing one returned by previous query...
			pBack = (IFolder*) i->second->getInterface( "IFolder" );
		}
	}

	return pBack;
}

/**
 * returns the number of files contained in this folder
 */
unsigned int MWin32Directory::getFileCount()
{
	return (unsigned int) m_files.size();
}

/** 
 * returns the file at the specified index contained in this folder
 */
String MWin32Directory::getFile( unsigned int index )
{
	return this->m_files[ index ];
}

/**
 * opens a existing file for reading
 */
ISeekableInputStream* MWin32Directory::openReadFile( String fileName )
{
	ISeekableInputStream* pStream = 0;

	MFileInputStream* pFile = new MFileInputStream();
	try
	{
		pFile->open( this->m_path + "/" + fileName );
		pStream = pFile;
	}
	catch( ... )
	{
		delete pFile;
	}

	return pStream;
}

/** 
 * closes the input stream
 */
void MWin32Directory::close( ISeekableInputStream* pIn )
{
	if( pIn )
	{
		MFileInputStream* pFile = (MFileInputStream*) pIn;
		try
		{
			pFile->close();
		}
		catch( ... )
		{
			throw MIoException( "MWin32Directory::close failed" );
		}
        delete pFile;
	}
}

/**
 * opens a output stream
 */
ISeekableOutputStream* MWin32Directory::openWriteFile( String fileName )
{
	ISeekableOutputStream* pStream = 0;

	MFileOutputStream* pFile = new MFileOutputStream();
	try
	{
		pFile->open( this->m_path + "/" + fileName );
		pStream = pFile;
	}
	catch( ... )
	{
		delete pFile;
	}

	return pStream;
}

/** 
 * closes a output stream
 */
void MWin32Directory::close( ISeekableOutputStream* pIn )
{
	if( pIn )
	{
		MFileOutputStream* pFile = (MFileOutputStream*) pIn;
		try
		{
			pFile->close();
		}
		catch( ... )
		{
			throw MIoException( "MWin32Directory::close failed" );
		}
        delete pFile;
	}
}

/** 
 * updates the folder and child lists
 */
void MWin32Directory::updateFromPath()
{
	ASSERT( m_path != "" );

	m_folders.clear();
	m_files.clear();

	WIN32_FIND_DATA attr;
	HANDLE searchHandle = FindFirstFile( (m_path+"/*"), &attr );
	if( searchHandle != INVALID_HANDLE_VALUE )
	{
		insert( &attr );
		while( FindNextFile( searchHandle, &attr ) )
			insert( &attr );

		FindClose( searchHandle );
	}
	else
	{
		throw MIoException( "error finding " + m_path );
	}
}

/** 
 * checks ONE result of a win32 directory
 * search and inserts the found item
 * into the folder or file list.
 */
void MWin32Directory::insert( void* pvAttr )
{
	WIN32_FIND_DATA *pAttr=(WIN32_FIND_DATA *)pvAttr;

	ASSERT( pAttr );

	if( pAttr->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		if( strcmp( pAttr->cFileName, "." ) != 0 &&
			strcmp( pAttr->cFileName, ".." ) != 0 )
		{
			this->m_folders.insert(
				Str2ObjMap::value_type(
					String( pAttr->cFileName ),
					0 ) );
		}
	}
	else
	{
		this->m_files.push_back( String( pAttr->cFileName ) );
	}
}

/**
 * sets a folder for caching
 */
void MWin32Directory::setFolder( String folderName, IFolder* pFolder )
{
	ASSERT( folderName != "" );
	ASSERT( pFolder );
	ASSERT( pFolder->getInterface( "MObject" ) );

	// remove existing if nescessary...
	Str2ObjMapIter i = m_folders.find( folderName );
	if( i != m_folders.end() )
	{
		if( i->second )
			delete i->second;
		m_folders.erase( i );
	}

	// insert new folder...
	m_folders.insert(
		Str2ObjMap::value_type(
			folderName,
			(MObject*) pFolder->getInterface( "MObject" ) ) );
}

/** 
 * reads the content of the specified
 * resource into the pDataOut buffer.
 *
 * the calles is responsible for deleting
 * the allocated memory!!!
 */
void MWin32Directory::loadResource( String fileName, unsigned char** pDataOut, unsigned int* pDataLenOut )
{
	MFileInputStream file;
	try
	{
		file.open( m_path + "/" + fileName );

		unsigned int fileLen = (unsigned int) file.getFileLength();
		(*pDataOut) = new unsigned char[ fileLen ];
		file.read( (*pDataOut), fileLen );
		(*pDataLenOut) = fileLen;
		file.close();
	}
	catch( MIoException ae )
	{
		 (*pDataOut) = 0;
		 (*pDataLenOut) = 0;

		 throw ae;
	}
}

/** 
 * free instance
 */
void MWin32Directory::release()
{
	delete this;
}

/**
 * returns the path on disk
 */
String MWin32Directory::getPath()
{
	return this->m_path;
}

#endif