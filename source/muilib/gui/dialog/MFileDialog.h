#ifndef __MFileDialog
#define __MFileDialog

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include "../MTopWnd.h"

/**
 * class encapsulating a native file dialog
 */
class MFileDialog
{
protected:

	char ivBuffer[MAX_PATH];
	OPENFILENAME ivDlgStruc;
	String ivFilter;

public:

	MFileDialog();
	virtual ~MFileDialog();

	virtual String getSelectedFile();
	virtual bool showOpenDialog( MTopWnd* parent, String title, char* filter );
	virtual bool showSaveDialog( MTopWnd* parent, String title, char* filter );

protected:

	void setFilter( String filter );
	String getErrorText( DWORD errCode );
};

#endif