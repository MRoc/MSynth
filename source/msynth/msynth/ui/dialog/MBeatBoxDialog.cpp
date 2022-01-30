#include "MBeatBoxDialog.h"
#include <gui/dialog/MMessageBox.h>
#include <gui/textfield/MTextField.h>
#include <framework/io/MLogger.h>
#include <framework/treedocument/MSaxTreeDocument.h>

MBeatBoxDialog::MBeatBoxDialog()
	: MDialog(),
	ivChannelCount( 8 )
{
	MSaxTreeDocument doc;
	doc.parseResource( "resource/xml/beatboxdialog.xml" );
	load( doc.getRoot() );
}

void MBeatBoxDialog::onInit()
{
	MTextField* pTF = (MTextField*) MWndNamespace::getObjNS( this, "textfield" );
	if( pTF )
		pTF->getModel()->setText( MInt::toString( ivChannelCount ) );
	else
		MLogger::logError(
			"MBeatBoxDialog::onInit: getObjNS(textfield) failed" );
}

void MBeatBoxDialog::onDestroy()
{
}

void MBeatBoxDialog::setCurrentChannelCount( unsigned int channelCount )
{
	ivChannelCount = channelCount;
}

unsigned int MBeatBoxDialog::getCurrentChannelCount()
{
	return ivChannelCount;
}

MBeatBoxDialog::DlgResult MBeatBoxDialog::onDlgCommand( unsigned int id )
{
	/*switch( id )
	{
	case IDOK:*/
		{
			MTextField* pTF = (MTextField*) MWndNamespace::getObjNS( this, "textfield" );
			if( pTF )
			{
				try
				{
					ivChannelCount = MInt::parse( pTF->getText() );
					if( ivChannelCount < 1 || ivChannelCount > 32 )
					{
						MMessageBox::showError( "NUMBER_RANGE_EXC", String("1"), String("32") );
						return MDialog::DlgResult::CANCEL;
					}
					else
					{
						return MDialog::DlgResult::OK;
					}
				}
				catch( ... )
				{
				}
			}
			else
				MLogger::logError(
					"MBeatBoxDialog::onCommand: GetDlgItem IDC_EDIT_CHANNELCOUNT failed" );
		}
		/*break;
	}*/
	return MDialog::DlgResult::CANCEL;
}