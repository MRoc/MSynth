#ifndef __MUtil
#define __MUtil

#include "MTypes.h"

#define ROUND( p ) { if( ((FP)((int)p)) + .5f > (p) ){ p = ((FP)((int)p)); } else { p = ((FP)((int)p)) + 1.0f; } }
 
class MUtil
{
public:

	static String toString( int number )
	{
		String back("");
		back.Format( "%i", number );
		return back;
	}

	static String toString( unsigned int number )
	{
		String back("");
		back.Format( "%u", number );
		return back;
	}

	static String toHexString( unsigned int number )
	{
		String back("");
		back.Format( "%x", number );
		return back;
	}

	static unsigned int parseHexString( String value )
	{
		unsigned int back;
		sscanf( value.getData(), "%x", &back );
		return back;
	}

	static String toString( float number )
	{
		String back("");
		back.Format( "%f", number );
		return back;
	}

	static String getTabs( int length )
	{
		String back("");
		for( int i=0;i<length;i++ )
			back += "\t";
		return back;
	}

	static int round( FP value )
	{
		int back = 0;
		if( value < FP(int(value)) + 0.5 )
			back = int( value );
		else
			back = int( value ) + 1;
		return back;
	}

	static String convertToValidXML( String text )
	{
		/*
		< &lt;
		> &gt;
		& &amp;
		" &quot;
		*/

		String back = "";

		for( int i=0;i<text.GetLength();i++ )
		{
			if( text.GetAt( i ) == '<' )
				back += "&lt;";
			else if( text.GetAt( i ) == '>' )
				back += "&gt;";
			else if( text.GetAt( i ) == '&' )
				back += "&amp;";
			else if( text.GetAt( i ) == '\"' )
				back += "&quot;";
			else
				back += text.GetAt( i );
		}

		return back;
	}
};

#endif