#include "Base64.hpp"
#include <ctype.h>
static const int BASELENGTH = 255;       
static const int FOURBYTE   = 4;

// the base64 alphabet according to definition in RFC 2045
const unsigned char Base64::base64Alphabet[] = {
    'A', 'B', 'C', 'D', 'E',
    'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y',
	'Z', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i',
	'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's',
	't', 'u', 'v', 'w', 'x',
	'y', 'z', '0', '1', '2',
	'3', '4', '5', '6', '7',
	'8', '9', '+', '/', '\0'
};

unsigned char Base64::base64Inverse[BASELENGTH];

const unsigned char Base64::base64Padding = '=';

bool Base64::isInitialized = false;

// number of quadruplets per one line ( must be >1 and <19 )
const unsigned int Base64::quadsPerLine = 15;

unsigned char* Base64::encode(const unsigned char* const inputData,
                        const unsigned int   inputLength,
                        unsigned int*        outputLength)
{
    if (!isInitialized)
        init();

    if (!inputData)
        return 0;

    int quadrupletCount = ( inputLength + 2 ) / 3;
    if (quadrupletCount == 0)
        return 0;

    // number of rows in encoded stream ( including the last one )
    int lineCount = ( quadrupletCount + quadsPerLine-1 ) / quadsPerLine;

    //
    // convert the triplet(s) to quadruplet(s)
    //
    unsigned char  b1, b2, b3, b4;  // base64 binary codes ( 0..63 )

    unsigned int inputIndex = 0;
    unsigned int outputIndex = 0;
    unsigned char *encodedData = new unsigned char[ quadrupletCount*FOURBYTE + lineCount + 1 ];

    //
    // Process all quadruplet(s) except the last
    //
    int quad = 1;
    for (; quad <= quadrupletCount-1; quad++ )
    {
        // read triplet from the input stream
        split1stOctet( inputData[ inputIndex++ ], b1, b2 );
        split2ndOctet( inputData[ inputIndex++ ], b2, b3 );
        split3rdOctet( inputData[ inputIndex++ ], b3, b4 );

        // write quadruplet to the output stream
        encodedData[ outputIndex++ ] = base64Alphabet[ b1 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b4 ];

        if (( quad % quadsPerLine ) == 0 )
            encodedData[ outputIndex++ ] = 0xA;
    }

    //
    // process the last Quadruplet
    //
    // first octet is present always, process it
    split1stOctet( inputData[ inputIndex++ ], b1, b2 );
    encodedData[ outputIndex++ ] = base64Alphabet[ b1 ];

    if( inputIndex < inputLength )
    {
        // second octet is present, process it
        split2ndOctet( inputData[ inputIndex++ ], b2, b3 );
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];

        if( inputIndex < inputLength )
        {
            // third octet present, process it
            // no PAD e.g. 3cQl
            split3rdOctet( inputData[ inputIndex++ ], b3, b4 );
            encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
            encodedData[ outputIndex++ ] = base64Alphabet[ b4 ];
        }
        else
        {
            // third octet not present
            // one PAD e.g. 3cQ=
            encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
            encodedData[ outputIndex++ ] = base64Padding;
        }
    }
    else
    {
        // second octet not present
        // two PADs e.g. 3c==
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];
        encodedData[ outputIndex++ ] = base64Padding;
        encodedData[ outputIndex++ ] = base64Padding;
    }

    // write out end of the last line
    encodedData[ outputIndex++ ] = 0xA;
    // write out end of string
    encodedData[ outputIndex ] = 0;

    if( outputLength != 0 )
      (*outputLength) = outputIndex;

    return encodedData;
}

//
// delete the buffer allocated by decode() if
// decoding is successfully done.
//
// In previous version, we use XMLString::strLen(decodedData)
// to get the length, this will fail for test case containing
// consequtive "A", such "AAFF", or "ab56AA56". Instead of 
// returning 3/6, we have 0 and 3, indicating that "AA", after
// decoded, is interpreted as <null> by the strLen().
//
// Since decode() has track of length of the decoded data, we
// will get this length from decode(), instead of strLen().
//
int Base64::getDataLength( const char* const inputData )
{
    unsigned int    retLen = 0;
    char* decodedData = decode(inputData, &retLen);

    if ( !decodedData )
        return -1;
    else
    {
        delete[] decodedData;
        return retLen;
    }
}

//
// return 0(null) if invalid data found.
// return the buffer containning decoded data otherwise
// the caller is responsible for the de-allocation of the
// buffer returned. 
//
// temporary data, rawInputData, is ALWAYS released by this function.
//
unsigned char* Base64::decode(
	const unsigned char* const inputData,
	unsigned int*        outputLength)
{
    if (!isInitialized)
        init();

    if ((!inputData) || (!*inputData))
        return 0;

    //
    // remove all whitespaces from the base64Data
    //

    unsigned int inputLength = (unsigned int) strlen( (const char* const)inputData );
    unsigned char* rawInputData = new unsigned char[ inputLength + 1 ];
    ArrayJanitor<unsigned char> jan(rawInputData);

    unsigned int inputIndex = 0;
    unsigned int rawInputLength = 0;
    while ( inputIndex < inputLength )
    {
        if ( ! isspace( inputData[inputIndex] ) )
            rawInputData[ rawInputLength++ ] = inputData[ inputIndex ];

        inputIndex++;
    }
    rawInputData[ rawInputLength ] = 0;

    // the length of raw data should be divisible by four
    if (( rawInputLength % FOURBYTE ) != 0 ) 
        return 0;  

    int quadrupletCount = rawInputLength / FOURBYTE;
    if ( quadrupletCount == 0 )
        return 0;

    //
    // convert the quadruplet(s) to triplet(s)
    //
    unsigned char  d1, d2, d3, d4;  // base64 characters
    unsigned char  b1, b2, b3, b4;  // base64 binary codes ( 0..64 )

    int rawInputIndex  = 0;
    int outputIndex    = 0;
    unsigned char *decodedData = new unsigned char[ quadrupletCount*3 + 1 ];

    //
    // Process all quadruplet(s) except the last
    //
    int quad = 1;
    for (; quad <= quadrupletCount-1; quad++ )
    {
        // read quadruplet from the input stream
        if (!isData( (d1 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d2 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d3 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d4 = rawInputData[ rawInputIndex++ ]) ))
        {
            // if found "no data" just return NULL
            delete[] decodedData;
            return 0;
        }

        b1 = base64Inverse[ d1 ];
        b2 = base64Inverse[ d2 ];
        b3 = base64Inverse[ d3 ];
        b4 = base64Inverse[ d4 ];

        // write triplet to the output stream
        decodedData[ outputIndex++ ] = set1stOctet(b1, b2);
        decodedData[ outputIndex++ ] = set2ndOctet(b2, b3);
        decodedData[ outputIndex++ ] = set3rdOctet(b3, b4);
    }

    //
    // process the last Quadruplet
    //
    // first two octets are present always, process them
    if (!isData( (d1 = rawInputData[ rawInputIndex++ ]) ) ||
        !isData( (d2 = rawInputData[ rawInputIndex++ ]) ))
    {
        // if found "no data" just return NULL
        delete[] decodedData;
        return 0;
    }

    b1 = base64Inverse[ d1 ]; 
    b2 = base64Inverse[ d2 ];

    // try to process last two octets
    d3 = rawInputData[ rawInputIndex++ ];
    d4 = rawInputData[ rawInputIndex++ ];

    if (!isData( d3 ) || !isData( d4 )) 
    {
        // check if last two are PAD characters
        if (isPad( d3 ) && isPad( d4 )) 
        {
            // two PAD e.g. 3c==
            if ((b2 & 0xf) != 0) // last 4 bits should be zero
            {
                delete[] decodedData;
                return 0;
            }

            decodedData[ outputIndex++ ] = set1stOctet(b1, b2);
        }
        else if (!isPad( d3 ) && isPad( d4 )) 
        {               
            // one PAD e.g. 3cQ=
            b3 = base64Inverse[ d3 ];
            if (( b3 & 0x3 ) != 0 ) // last 2 bits should be zero
            {
                delete[] decodedData;
                return 0;
            }

            decodedData[ outputIndex++ ] = set1stOctet( b1, b2 );
            decodedData[ outputIndex++ ] = set2ndOctet( b2, b3 );
        } 
        else 
        {
            // an error like "3c[Pad]r", "3cdX", "3cXd", "3cXX" where X is non data 
            delete[] decodedData;
            return 0;
        }       
    } 
    else 
    { 
        // no PAD e.g 3cQl
        b3 = base64Inverse[ d3 ];
        b4 = base64Inverse[ d4 ];
        decodedData[ outputIndex++ ] = set1stOctet( b1, b2 );
        decodedData[ outputIndex++ ] = set2ndOctet( b2, b3 );
        decodedData[ outputIndex++ ] = set3rdOctet( b3, b4 );
    }

    // write out the end of string
    decodedData[ outputIndex ] = 0; 
    *outputLength = outputIndex;

    return decodedData;
}

char* Base64::decode(const char* const inputData,
                      unsigned int*      outputLength)
{
	if (!inputData)
		return 0;

	unsigned int srcLen = (unsigned int) strlen(inputData);
    unsigned char *toFill = new unsigned char[srcLen+1];
    ArrayJanitor<unsigned char> janFill(toFill);

    for (unsigned int i = 0; i < srcLen; i++)
		toFill[i] = (unsigned char)inputData[i];

	toFill[srcLen] = 0;

	unsigned int      decodedLen = 0;
	unsigned char *DecodedBuf = decode(toFill, &decodedLen);

	if (!DecodedBuf)
		return 0;

    char *toRet = new char[decodedLen+1];

    for (unsigned int j = 0; j < decodedLen; j++)
		toRet[j] = (char)DecodedBuf[j];

	toRet[decodedLen] = 0;

	*outputLength = decodedLen;
	delete[] DecodedBuf;

	return toRet;

}

// -----------------------------------------------------------------------
//  Helper methods
// -----------------------------------------------------------------------
void Base64::init()
{
    if (isInitialized)
        return;

    isInitialized = true;

    // create inverse table for base64 decoding
    // if base64Alphabet[ 17 ] = 'R', then base64Inverse[ 'R' ] = 17
    // for characters not in base64Alphabet the base64Inverse[] = -1

    int i;
    // set all fields to -1
    for ( i = 0; i < BASELENGTH; i++ ) 
        base64Inverse[i] = (unsigned char)-1; 

    // compute inverse table
    for ( i = 0; i < 64; i++ ) 
        base64Inverse[ base64Alphabet[i] ] = (unsigned char)i;
}

bool Base64::isData(const unsigned char& octet) 
{
    return (base64Inverse[octet]!=(unsigned char)-1);
}
