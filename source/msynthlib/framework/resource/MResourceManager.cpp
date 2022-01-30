#include "MResourceManager.h"
#include "MWin32Directory.h"
#include "../io/MLogger.h"
#include "../io/MIoException.h"

/**
 * the singleton instance
 */
MResourceManager* MResourceManager::g_pInstance = 0;

/** 
 * constructor
 */
MResourceManager::MResourceManager()
{
}

/**
 * destructor
 */
MResourceManager::~MResourceManager()
{
	while( m_instreamMap.size() > 0 )
		close( m_instreamMap.begin()->first );
	while( m_outstreamMap.size() > 0 )
		close( m_outstreamMap.begin()->first );

	for(
		Str2FolderMapIter i=m_folderMap.begin();
		i != m_folderMap.end();
		i++ )
	{
		i->second->release();
	}
	m_folderMap.clear();
}

/**
 * returns a pointer to the singleton instance
 */
IMResourceManager* MResourceManager::getInstance()
{
	if( g_pInstance == 0 )
		g_pInstance = new MResourceManager();
	return g_pInstance;
}

/**
 * releases the singletion instance
 */
void MResourceManager::release()
{
	if( g_pInstance )
	{
		delete g_pInstance;
		g_pInstance = 0;
	}
}

/**
 * adds a folder to the resource manager
 * (inheritanced from IMResourceManager)
 */
void MResourceManager::loadArchive( IFolder* pFolder )
{
	ASSERT( pFolder );
	ASSERT( m_folderMap.find( pFolder->getName() ) == m_folderMap.end() );

	m_folderMap.insert( Str2FolderMap::value_type( pFolder->getName(), pFolder )  );
}

/**
 * registers a directory as archive,
 * be carefull with this, because a big directory tree
 * will take a big amount of reserved memory when beeing
 * registered here!!!
 */
void MResourceManager::loadArchive( String pathOnDisk, String aliasName )
{
	loadArchive( new MWin32Directory( pathOnDisk, aliasName ) );
}

/**
 * opens a file in a registered archive.
 *
 * the parameter pathNName sould be as follows:
 *
 * archiveName/[path/]filename[.suffix]
 *
 * (inheritanced from IMResourceManager)
 */
ISeekableInputStream* MResourceManager::openReadFile( String pathNName )
{
	ASSERT( pathNName != "" );

	// the returned value
	ISeekableInputStream* pInStreamBack = 0;

	// divide path seperated by slash (/) into string list,
	// first = folder in foldermap
	// middle = deeper folders in the first folder
	// last = file in the last folder
	StrList dividedPath = divide( pathNName );

	// first item is folder in foldemap...
	StrListIter strs = dividedPath.begin();
	if( m_folderMap.find(*strs)!=m_folderMap.end() )
	{
		IFolder* pFolder = m_folderMap.find( (*strs) )->second; // TODO: check
		strs++;

		// go deeper into folder structure...
		for(
			;
			strs != dividedPath.end()-1;
			strs++ )
		{
			pFolder = pFolder->getFolder( (*strs) );
		}

		if( pFolder )
		{
			pInStreamBack = pFolder->openReadFile( (*strs) );

			// if succeeded, store creator of stream
			if( pInStreamBack )
				this->m_instreamMap.insert(
					InStream2FolderMap::value_type(
						pInStreamBack,
						pFolder ) );
		}
	}
	else
	{
		MLogger::logError( "MResourceManager::openReadFile: entry for \"%s\" not found!", strs );
	}
	dividedPath.clear();

	return pInStreamBack;
}

/** 
 * closes a stream previously created by openReadFile(...)
 * (inheritanced from IMResourceManager)
 */
void MResourceManager::close( ISeekableInputStream* pInStream )
{
	// assert valid string
	ASSERT( pInStream );

	// assert resource manager is creator of the stream
	ASSERT( m_instreamMap.find( pInStream ) != m_instreamMap.end() );

	InStream2FolderMapIter i = m_instreamMap.find( pInStream );
	if( i != m_instreamMap.end() )
	{
		IFolder* pFolder = i->second;
		if( pFolder )
		{
			pFolder->close( pInStream );
		}
		else
			MLogger::logError(
				"MResourceManager::close: folder of instream not found!" );

		m_instreamMap.erase( i );
	}
}

/**
 * opens a output stream to write into
 */
ISeekableOutputStream* MResourceManager::openWriteFile( String pathNName )
{
	ASSERT( pathNName != "" );

	// the returned value
	ISeekableOutputStream* pOutStreamBack = 0;

	// divide path seperated by slash (/) into string list,
	// first = folder in foldermap
	// middle = deeper folders in the first folder
	// last = file in the last folder
	StrList dividedPath = divide( pathNName );

	// first item is folder in foldemap...
	StrListIter strs = dividedPath.begin();
	if( m_folderMap.find(*strs)!=m_folderMap.end() )
	{
		IFolder* pFolder = m_folderMap.find( (*strs) )->second; // TODO: check
		strs++;

		// go deeper into folder structure...
		for(
			;
			strs != dividedPath.end()-1;
			strs++ )
		{
			pFolder = pFolder->getFolder( (*strs) );
		}

		if( pFolder )
		{
			pOutStreamBack = pFolder->openWriteFile( (*strs) );

			// if succeeded, store creator of stream
			if( pOutStreamBack )
				this->m_outstreamMap.insert(
					OutStream2FolderMap::value_type(
						pOutStreamBack,
						pFolder ) );
		}
	}
	else
	{
		MLogger::logError( "MResourceManager::openWriteFile: entry for \"%s\" not found!", strs );
	}
	dividedPath.clear();

	return pOutStreamBack;
}

/** 
 * closes a stream previously created by openReadFile(...)
 */
void MResourceManager::close( ISeekableOutputStream* pOutStream )
{
	// assert valid string
	ASSERT( pOutStream );

	// assert resource manager is creator of the stream
	ASSERT( m_outstreamMap.find( pOutStream ) != m_outstreamMap.end() );

	OutStream2FolderMapIter i = m_outstreamMap.find( pOutStream );
	if( i != m_outstreamMap.end() )
	{
		IFolder* pFolder = i->second;
		if( pFolder )
		{
			pFolder->close( pOutStream );
		}
		else
			MLogger::logError(
				"MResourceManager::close: folder of outstream not found!" );

		m_outstreamMap.erase( i );
	}
}

/** 
 * this function loads specified resource into memory.
 * it allocates memory for pDataOut, writes the data into
 * it and outputs the datalength of the buffer by pDataLenOut.
 *
 * the caller of this function is responsible for deleting
 * the allocates memory!!!
 */
void MResourceManager::loadResource( String pathNNameIn, unsigned char** pDataOut, unsigned int* pDataLenOut )
{
	ASSERT( pathNNameIn != "" );
	ASSERT( pDataOut );
	ASSERT( pDataLenOut );

	// divide path seperated by slash (/) into string list,
	// first = folder in foldermap
	// middle = deeper folders in the first folder
	// last = file in the last folder
	StrList dividedPath = divide( pathNNameIn );

	// first item is folder in foldemap...
	StrListIter strs = dividedPath.begin();
	if( m_folderMap.find(*strs)!=m_folderMap.end() )
	{
		IFolder* pFolder = m_folderMap.find( (*strs) )->second; // TODO: check
		strs++;

		// go deeper into folder structure...
		for(
			;
			strs != dividedPath.end()-1;
			strs++ )
		{
			pFolder = pFolder->getFolder( (*strs) );
		}

		if( pFolder )
		{
			try
			{
				pFolder->loadResource( (*strs), pDataOut, pDataLenOut );
			}
			catch( MIoException ae )
			{
				dividedPath.clear();
				throw ae;
			}
		}
	}
	else
		MLogger::logError(
			"MResourceManager::openReadFile: entry for \"%s\" not found!",
			*strs );

	dividedPath.clear();
}

/**
 * divides the given path by / and \
 */
StrList MResourceManager::divide( String path )
{
	StrList back;

	int index = -1;
	
	String p = path;
	while( (index=p.Find( "/" )) != -1 )
	{
		String left = p.Left( index );
		back.push_back( left );
		p = p.Right( p.GetLength() - (index+1) );
	}

	back.push_back( p );

	return back;
}

/**
 * returns a folder encapsulating the given folder
 */
IFolder* MResourceManager::getFolder( String name )
{
	ASSERT( name != "" );

	// the returned value
	IFolder* pBack = 0;

	// divide path seperated by slash (/) into string list,
	// first = folder in foldermap
	// middle = deeper folders in the first folder
	// last = file in the last folder
	StrList dividedPath = divide( name );

	// first item is folder in foldemap...
	StrListIter strs = dividedPath.begin();
	if( m_folderMap.find(*strs)!=m_folderMap.end() )
	{
		IFolder* pFolder = m_folderMap.find( (*strs) )->second; // TODO: check
		strs++;

		// go deeper into folder structure...
		for(
			;
			strs != dividedPath.end();
			strs++ )
		{
			pFolder = pFolder->getFolder( (*strs) );
		}

		pBack = pFolder;
	}
	else
	{
		MLogger::logError( "MResourceManager::openWriteFile: entry for \"%s\" not found!", strs );
	}
	dividedPath.clear();

	return pBack;
}

/**
 * returns the full filename into the resource
 */
String MResourceManager::getFile( String resourceName )
{
	String back;
	StrList dividedPath = divide( resourceName );
	// first item is folder in foldemap...
	StrListIter strs = dividedPath.begin();
	if( m_folderMap.find(*strs)!=m_folderMap.end() )
	{
		IFolder* pFolder = m_folderMap.find( (*strs) )->second; // TODO: check
		strs++;

		// go deeper into folder structure...
		for(
			;
			strs != dividedPath.end()-1;
			strs++ )
		{
			pFolder = pFolder->getFolder( (*strs) );
		}

		back = pFolder->getPath() + "/" + *strs;
	}

	return back;
}