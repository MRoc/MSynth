#ifndef __MMessageBox
#define __MMessageBox

#include <framework/MTypes.h>

class MMessageBox
{
public:

	static void showError( String id );

	static void showError( String id, String subString );

	static void showError( String id, String subString1, String subString2 );

	static int showQuestion( String id, String subString );

	static int showQuestion( String id );

private:
};

#endif
