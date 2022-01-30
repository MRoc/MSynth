#ifndef __MString
#define __MString

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * object implementing a string object
 */
class MString
{
protected:

	/**
	 * storing the string data
	 */
	char* ivData;

public:

	/**
	 * constructor
	 */
	MString();

	/**
	 * constructor
	 */
	MString( const char character );

	/**
	 * constructor
	 */
	MString( const char* string );

	/**
	 * constructor
	 */
	MString( const unsigned short* string );

	/**
	 * constructor
	 */
	MString( const MString& string );

	/**
	 * destructor
	 */
	virtual ~MString();

	/**
	 * sets the data
	 */
	virtual void setData( const char* str );

	/**
	 * sets the data
	 */
	virtual void setData( const MString& str );

	/** sets a string not terminated by zero */
	virtual void setData( const char* str, unsigned int len );

	/**
	 * format string
	 */
	virtual void format( const char *pchFormat,... );

	/** 
	 * returns the data
	 */
	const char* const getData() const;

	/**
	 * returns the indexed character
	 */
	virtual char getChar( unsigned int index ) const;

	/** 
	 * adds the given string
	 */
	virtual void addString( const char* strToAdd );

	/**
	 * adds the given character
	 */
	virtual void addChar( const char charToAdd );

 	void operator = (const char character);
	void operator = (const char* string);
	void operator = (const MString& string);

	MString operator + (const char character);
	MString operator + (const char* string);
	MString operator + (const MString& string);

	friend MString operator  +  (const MString& str1, const MString& str2);

	void operator += (const char character);
	void operator += (const char* string);
	void operator += (const MString& string);

	operator const char*() const;

	friend bool operator == (const char *pchString,const MString& cMString);
	friend bool operator == (const MString& cMString,const char *pchString);
	friend bool operator == (const MString& cMString1,const MString& cMString2);

	friend bool operator != (const MString& cMString,const char*pchString);
	friend bool operator != (const char *pchString,const MString& cMString);
	friend bool operator != (const MString& cMString1,const MString& cMString2);

	friend bool operator <= (const MString& cMString,const char*pchString);
	friend bool operator <= (const char*,const MString& cMString);
	friend bool operator <= (const MString& cMString1,const MString& cMString2);

	friend bool operator >= (const MString& cMString,const char*pchString);
	friend bool operator >= (const char*,const MString& cMString);
	friend bool operator >= (const MString& cMString1,const MString& cMString2);

	friend bool operator < (const MString& cMString,const char*pchString);
	friend bool operator < (const char*,const MString& cMString);
	friend bool operator < (const MString& cMString1,const MString& cMString2);

	friend bool operator > (const MString& cMString,const char*pchString);
	friend bool operator > (const char*,const MString& cMString);
	friend bool operator > (const MString& cMString1,const MString& cMString2);

// CString overloading
	void Format( const char *pchFormat,... );
	char GetAt( int index ) const;
	int GetLength() const;
	int Replace( char oldChar, char newChar );
	int Replace( MString oldStr, MString newStr );
	int Find( const MString &str ) const;
	MString Left( int count ) const;
	MString Mid( int index1, int count ) const;
	MString Right( int count ) const;
	int ReverseFind( const char toSearch ) const;
};
#endif