#ifndef __MABOUTDIALOG
#define __MABOUTDIALOG

#include <gui/dialog/MDialog.h>
#include <gui/net/MLink.h>

class MAboutDialog :
	public MDialog
{
protected:

	String	ivVersion;
	String	ivCopyright;

public:

	MAboutDialog();
	virtual ~MAboutDialog();

	void setVersion( String version ){ ivVersion = version; }
	void setCopyright( String copyright ){	ivCopyright = copyright; }

protected:

	virtual void onInit();
	virtual MDialog::DlgResult onDlgCommand( unsigned int id );
	virtual void onDestroy();
};

#endif 
