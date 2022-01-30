#include "MLogger.h"
#include "../MNativeTypes.h"

MLogger* MLogger::g_pInstance = 0;

MLogger::MLogger() :
	m_pOut( 0 )
{
}

MLogger::MLogger( String logFile ) :
	m_pOut( 0 )
{
	m_pOut = new MFilePrinter();
	m_pOut->open( logFile );
}


MLogger::~MLogger()
{
	if( m_pOut )
	{
		m_pOut->close();
		delete m_pOut;
	}
}

void MLogger::init( String fileName )
{
	if( g_pInstance == 0 )
		g_pInstance = new MLogger( fileName );
}

void MLogger::release()
{
	if( g_pInstance )
	{
		delete g_pInstance;
		g_pInstance = 0;
	}
}

void MLogger::logError( String error,... )
{
	va_list l;
	va_start( l, error );

	char buffer[ 4096 ];
	vsprintf(
		buffer,
		error,
		l ); 

	if( g_pInstance && g_pInstance->m_pOut )
		 g_pInstance->m_pOut->println( buffer );

	OutputDebugString( buffer );
	MessageBox(
		0,
		buffer,
		"ERROR",
		MB_OK | MB_ICONERROR );

	va_end( l );

	ASSERT( FALSE );
}

void MLogger::logWarning( String warning,... )
{
	va_list l;
	va_start( l, warning );

	char buffer[ 4096 ];
	vsprintf(
		buffer,
		warning,
		l ); 

	if( g_pInstance && g_pInstance->m_pOut )
		g_pInstance->m_pOut->println( buffer );

	OutputDebugString( buffer );
	MessageBox(
		0,
		buffer,
		"WARNING",
		MB_OK | MB_ICONERROR );

	va_end( l );
}

void MLogger::logMessage( String msg,... )
{
	va_list l;
	va_start( l, msg );

	char buffer[ 4096 ];
	vsprintf(
		buffer,
		msg,
		l ); 

	if( g_pInstance && g_pInstance->m_pOut )
		g_pInstance->m_pOut->println( buffer );

	OutputDebugString( buffer );

	printf( buffer );

	va_end( l );
}