#ifndef BASE64_HPP
#define BASE64_HPP
#include <string.h>
#include "../Janitor.h"

//
// This class provides encode/decode for RFC 2045 Base64 as
// defined by RFC 2045, N. Freed and N. Borenstein.
// RFC 2045: Multipurpose Internet Mail Extensions (MIME)
// Part One: Format of Internet Message Bodies. Reference
// 1996 Available at: http://www.ietf.org/rfc/rfc2045.txt
// This class is used by XML Schema binary format validation
//  
//
class Base64
{
public :

    static unsigned char* encode(
		const unsigned char* const inputData,
		const unsigned int   inputLength,
		unsigned int*        outputLength);

    static unsigned char* decode(
		const unsigned char* const inputData,
		unsigned int*        outputLength);

    static char* decode(
		const char* const inputData,
		unsigned int*      outputLength);

    static int getDataLength(const char* const inputData );

private :

    static void init();

    static bool isData(const unsigned char& octet);
    static bool isPad(const unsigned char& octet);

    static unsigned char set1stOctet(const unsigned char&, const unsigned char&);
    static unsigned char set2ndOctet(const unsigned char&, const unsigned char&);
    static unsigned char set3rdOctet(const unsigned char&, const unsigned char&);

    static void split1stOctet(const unsigned char&, unsigned char&, unsigned char&);
    static void split2ndOctet(const unsigned char&, unsigned char&, unsigned char&);
    static void split3rdOctet(const unsigned char&, unsigned char&, unsigned char&);

    Base64();
    Base64(const Base64&);

    static const unsigned char  base64Alphabet[];
    static const unsigned char  base64Padding;

    static unsigned char  base64Inverse[];
    static bool  isInitialized;

    static const unsigned int  quadsPerLine;
};

inline bool Base64::isPad(const unsigned char& octet) 
{
    return ( octet == base64Padding );
}

inline unsigned char Base64::set1stOctet(const unsigned char& b1, const unsigned char& b2)
{
    return (( b1 << 2 ) | ( b2 >> 4 ));
}

inline unsigned char Base64::set2ndOctet(const unsigned char& b2, const unsigned char& b3)
{
    return (( b2 << 4 ) | ( b3 >> 2 ));
}

inline unsigned char Base64::set3rdOctet(const unsigned char& b3, const unsigned char& b4)
{
    return (( b3 << 6 ) | b4 );
}

inline void Base64::split1stOctet(const unsigned char& ch, unsigned char& b1, unsigned char& b2) {
    b1 = ch >> 2;
    b2 = ( ch & 0x3 ) << 4;
}

inline void Base64::split2ndOctet(const unsigned char& ch, unsigned char& b2, unsigned char& b3) {
    b2 |= ch >> 4;  // combine with previous value  
    b3 = ( ch & 0xf ) << 2;
}

inline void Base64::split3rdOctet(const unsigned char& ch, unsigned char& b3, unsigned char& b4) {
    b3 |= ch >> 6;  // combine with previous value 
    b4 = ( ch & 0x3f );
}

#endif
