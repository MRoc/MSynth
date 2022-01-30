#include "MTestDialog.h"
#include <framework/io/MLogger.h>

/** constructor */
MTestDialog::MTestDialog() :
	ivPtButton( 0 )
{
	ivPtButton =
		new MButton(
			"open new modal",
			MColorMap::create( 32, 32, 64 ),
			MColorMap::create( 64, 64, 64 ),
			MColorMap::create( 180, 180, 210 ) );
	ivPtButton->setRect( MRect( 10, 10, 170, 20 ) );
	ivPtButton->addActionListener( this );
	addChild( ivPtButton );

	if( create( MRect( 100, 100, 200, 200 ), 0 ) == false )
		MLogger::logError( "MTestDialog::create failed" );

	setText( "MTestDialog" );

	doModal();
}

/** destructor */
MTestDialog::~MTestDialog()
{
	ivPtButton->removeActionListener( this );
}

/** invoked when button was pressed */
void MTestDialog::onActionPerformed( void* ptSrc )
{
	MTestDialog dlg;
}

/** query interface */
void* MTestDialog::getInterface( const String& className ) const
{
	if( className == "MTestDialog" )
		return (void*) ((MTestDialog*)this);
	else if( className == "IActionListener" )
		return (void*) ((IActionListener*)this);
	else
		return MDialog::getInterface( className );
}