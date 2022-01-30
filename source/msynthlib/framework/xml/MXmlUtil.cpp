#include "MXmlUtil.h"

#include "Base64.hpp"
#include "../MUtil.h"
#include "../io/MFilePrinter.h"

String MXmlUtil::convertToValidXML( String text )
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

unsigned char* MXmlUtil::getBase64Binary( String textData, unsigned int* len )
{
	return (unsigned char*) Base64::decode( textData.getData(), len );
}

String MXmlUtil::getBase64String( unsigned char* data, unsigned int len )
{
	char* ptTmp = (char*) Base64::encode( data, len, 0 );
	String back( ptTmp );
	delete[]( ptTmp );
	return back;
}