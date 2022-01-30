#ifndef __MTestDialog
#define __MTestDialog

#include <gui/dialog/MDialog.h>
#include <gui/button/MButton.h>

class MTestDialog :
	public MDialog,
	public IActionListener
{
protected:

	/** the button which opens a new testdialog */
	MButton* ivPtButton;

public:

	/** constructor */
	MTestDialog();

	/** destructor */
	virtual ~MTestDialog();

	/** invoked when button was pressed */
	virtual void onActionPerformed( void* ptSrc );

	/** query interface */
	virtual void* getInterface( const String& className ) const;
};

#endif