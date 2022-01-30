/**
 * (C) 2001-2003 MRoc
 */

#include "MString.h"
#include "../io/MLogger.h"
#include "../MNativeTypes.h"

/**
 * constructor
 */
MString::MString() :
	ivData( 0 )
{
	setData( "" );
}

/**
 * constructor
 */
MString::MString( const char character ) :
	ivData( 0 )
{
	char buffer[2];
	buffer[0] = character;
	buffer[1] = 0;
	setData( buffer );
}

/**
 * constructor
 */
MString::MString( const char* string ) :
	ivData( 0 )
{
	setData( string );
}

/**
 * constructor
 */
MString::MString( const unsigned short* string ) :
	ivData( 0 )
{
	TCHAR tempStr[4096];
	if( wcstombs( tempStr, string, 4095 ) == -1 )
		MLogger::logError( "MString::MString: wcstombs failed" );
	setData( tempStr );
}

/**
 * constructor
 */
MString::MString( const MString& string ) :
	ivData( 0 )
{
	setData( string );
}

/**
 * destructor
 */
MString::~MString()
{
	if( ivData )
		delete ivData;
}

/**
 * sets the data
 */
void MString::setData( const MString& str )
{
	setData( str.ivData );
}

/** sets a string not terminated by zero */
void MString::setData( const char* str, unsigned int len )
{
	if( ivData )
		delete ivData;
	ivData = new char[ len + 1 ];
	memcpy( ivData, str, len );
	ivData[ len ] = 0;
}

/**
 * sets the data
 */
void MString::setData( const char* str )
{
	if( ivData )
		delete ivData;
	size_t len = strlen( str );
	ivData = new char[ len + 1 ];
	memcpy( ivData, str, len + 1 );
}

/** 
 * returns the data
 */
const char* const MString::getData() const
{
	return ivData;
}

/**
 * returns the indexed character
 */
char MString::getChar( unsigned int index ) const
{
	return ivData[ index ];
}

/** 
 * adds the given string
 */
void MString::addString( const char* strToAdd )
{
	unsigned int srcLen1 = (unsigned int) strlen( ivData );
	unsigned int srcLen2 = (unsigned int) strlen( strToAdd );
	unsigned int newLen = srcLen1 + srcLen2 + 1;
	char* tmp = new char[ newLen ];
	memcpy( tmp, ivData, srcLen1 );
	memcpy( tmp + srcLen1, strToAdd, srcLen2 );
	tmp[ newLen-1 ] = 0;

	if( ivData )
		delete ivData;
	ivData = tmp;
}

/**
 * adds the given character
 */
void MString::addChar( const char charToAdd )
{
	if( charToAdd == 0 )
		ASSERT( FALSE );
	unsigned int srcLen1 = (unsigned int) strlen( ivData );
	unsigned int newLen = srcLen1 + 2;
	char* tmp = new char[ newLen ];
	memcpy( tmp, ivData, srcLen1 );
	tmp[ srcLen1 ] = charToAdd;
	tmp[ newLen-1 ] = 0;
	if( ivData )
		delete ivData;
	ivData = tmp;
}

/**
 * format string
 */
void MString::format( const char *pchFormat,... )
{
#define LEN 4096
	va_list list;
	char pchText[LEN];
	int iRet;
	
	va_start(list,pchFormat);
	iRet = _vsnprintf(pchText,LEN-1,pchFormat,list);
	va_end(list);
      
	if(iRet<0)
		pchText[LEN-1]=0;

	setData(pchText);
}

/////////////////////////////////////////////////////////////////////////////////////////
void MString::operator = (const char character)
{
	if( ivData )
		delete ivData;
	ivData = new char[ 2 ];
	ivData[ 0 ] = character;
	ivData[ 1 ] = 0;
}
void MString::operator = (const char* string)
{
	setData( string );
}
void MString::operator = (const MString& string)
{
	setData( string.ivData );
}
/////////////////////////////////////////////////////////////////////////////////////////
MString MString::operator + (const char character)
{
	MString tmp( *this );
	tmp.addChar( character );
	return tmp;
}
MString MString::operator + (const char* string)
{
	MString tmp( *this );
	tmp.addString( string );
	return tmp;
}
MString MString::operator + (const MString& string)
{
	MString tmp( *this );
	tmp.addString( string.ivData );
	return tmp;
}
MString operator  +  ( const MString& str1, const MString& str2 )
{
	MString tmp( str1.ivData );
	tmp.addString( str2.ivData );
	return tmp;
}
/////////////////////////////////////////////////////////////////////////////////////////
void MString::operator += (const char character)
{
	addChar( character );
}
void MString::operator += (const char* string)
{
	addString( string );
}
void MString::operator += (const MString& string)
{
	addString( string.ivData );
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator == (const char *pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) == 0;
}
bool operator == (const MString& cMString,const char *pchString)
{
	return strcmp( pchString, cMString.ivData ) == 0;
}
bool operator == (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) == 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator != (const MString& cMString,const char*pchString)
{
	return strcmp( pchString, cMString.ivData ) != 0;
}
bool operator != (const char *pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) != 0;
}
bool operator != (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) != 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator <= (const MString& cMString,const char*pchString)
{
	return strcmp( cMString.ivData, pchString ) < 0;
}
bool operator <= (const char* pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) < 0;
}
bool operator <= (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) < 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator >= (const MString& cMString,const char*pchString)
{
	return strcmp( cMString.ivData, pchString ) >= 0;
}
bool operator >= (const char* pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) >= 0;
}
bool operator >= (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) >= 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator < (const MString& cMString,const char*pchString)
{
	return strcmp( cMString.ivData, pchString ) < 0;
}
bool operator < (const char* pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) < 0;
}
bool operator < (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) < 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool operator > (const MString& cMString,const char*pchString)
{
	return strcmp( cMString.ivData, pchString ) > 0;
}
bool operator > (const char* pchString,const MString& cMString)
{
	return strcmp( pchString, cMString.ivData ) > 0;
}
bool operator > (const MString& cMString1,const MString& cMString2)
{
	return strcmp( cMString1.ivData, cMString2.ivData ) > 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
MString::operator const char *() const
{
	return ivData;
}
/////////////////////////////////////////////////////////////////////////////////////////
// CString overloading
void MString::Format( const char *pchFormat,... )
{
#define LEN 4096
	va_list list;
	char pchText[LEN];
	int iRet;
	
	va_start(list,pchFormat);
	iRet = _vsnprintf(pchText,LEN-1,pchFormat,list);
	va_end(list);
      
	if(iRet<0)
		pchText[LEN-1]=0;

	setData(pchText);
}
char MString::GetAt( int index ) const
{
	return ivData[ index ];
}
int MString::GetLength() const
{
	return (int) strlen( ivData );
}
int MString::Replace( char oldChar, char newChar )
{
	int back = 0;
	char* pt = ivData;
	while( *pt )
	{
		if( *pt == oldChar )
		{
			*pt = newChar;
			back++;
		}
		pt++;
	}
	return back;
}
int MString::Replace( MString oldStr, MString newStr )
{
	int back = 0;

	MString tmp;

	const char* pToSearch = oldStr.getData();
	int searchLen = oldStr.GetLength();
	char* pOrig = ivData;
	int origLen = GetLength();
	for( int i=0;i<origLen;i++ )
	{
		bool found = false;

		if( i + searchLen < origLen )
		{
			bool fTmp = true;
			for( int j=0;j<searchLen&&fTmp;j++ )
				if( pOrig[ i + j ] != pToSearch[ j ] )
					fTmp = false;
			found = fTmp;
		}

		if( found )
		{
			tmp += newStr;
			i += searchLen;
			back++;
		}
		else
			tmp += pOrig[ i ];
	}

	(*this) = tmp;

	return back;
}
int MString::Find( const MString &str ) const
{
	char* pt = strstr( ivData, str.ivData );
	if( pt == 0 )
		return -1;
	else
		return (int)(pt - ivData);
}
MString MString::Left( int count ) const
{
	ASSERT( count <= GetLength() );
	MString back;
	for( int i=0;i<count;i++ )
		back += getChar( i );
	return back;
}
MString MString::Mid( int index1, int count ) const
{
	ASSERT( index1 >= 0 );
	ASSERT( count >= 0 );
	ASSERT( index1 + count <= GetLength() );
	MString back;
	int stop = index1+count;
	if( stop > GetLength() )
		stop = GetLength();
	for( int i=index1;i<stop;i++ )
		back += getChar( i );
	return back;
}
MString MString::Right( int count ) const
{
	ASSERT( GetLength() - count <= GetLength() );
	MString back;
	int begin = GetLength() - count;
	int end = begin + count;
	for( int i=begin;i<end;i++ )
		back += getChar( i );
	return back;
}
int MString::ReverseFind( const char toSearch ) const
{
	int back = -1;
	for( int i=GetLength()-1;i>0&&back==-1;i--)
		if( ivData[ i ] == toSearch )
			back = i;
	return back;
}