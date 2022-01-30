#ifndef __MXmlUtil
#define __MXmlUtil

#include "../MTypes.h"

#define TEST_VALUE( value, minValue, maxValue ) \
	{\
		if( (value) < (minValue) )\
		{\
			(value) = (minValue);\
		}\
		else if( (value) > (maxValue) )\
		{\
			(value) = (maxValue);\
		}\
	}

class MXmlUtil
{
public:

	static String convertToValidXML( String text );
	static unsigned char* getBase64Binary( String textData, unsigned int* len );
	static String getBase64String( unsigned char* data, unsigned int len );
};

#endif