/* (C)2000-2002 MRoc hifiShock */

#include "Settings.h"
#include <framework/primitive/MInt.h>

#define DEFCOUNT 5
const int Settings::gvDefBufLen[] = {512, 1024, 2048, 4096, 8192};

Settings::Settings()
{
	MSaxTreeDocument doc;
	doc.parseResource( "resource/xml/settingsdialog.xml" );
	load( doc.getRoot() );
}

Settings::~Settings()
{
}

void Settings::onInit()
{
	/*CDialog::OnInitDialog();
	ivPtDxCombo	= (CComboBox*)GetDlgItem(IDC_DXDEVICE);
	ivPtBlCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BUFFERLENGTH);
	ivPtSFCheck = (CButton*)GetDlgItem(IDC_CHECK_STICKYFOCUS);*/

	MWndNamespace::getObjNS( this, "dxCombo" );

	MComboBox* dxCombo = (MComboBox*) MWndNamespace::getObjNS( this, "dxCombo" );
	if( dxCombo )
	{
        unsigned int count = MAudioEngine::getDxDeviceCount();
		for( unsigned int i=0;i<count;i++ )
			dxCombo->getModel()->add( new MObjString( MAudioEngine::getDxDeviceDesc( i ) ) );
		dxCombo->getModel()->setSelection(
			MProperties::getInt(
				MAudioEngine::gvDxDeviceTag,
				0,
				0,
				MAudioEngine::getDxDeviceCount() ) );
	}
	else
		MLogger::logError(
			"Settings::onInit: MWndNamespace::getObjNS( dxCombo ) failed" );

	MComboBox* blCombo = (MComboBox*) MWndNamespace::getObjNS( this, "blCombo" );
	if( blCombo )
	{
		int bufferLength = MProperties::getInt( MAudioEngine::gvBufferLengthTag, 8192, 512, 8192 );
		for( int i=0;i<DEFCOUNT;i++)
		{
			blCombo->getModel()->add( new MObjString( MInt::toString( gvDefBufLen[i] ) + " bytes" ) );
			if( bufferLength == gvDefBufLen[i] )
				blCombo->getModel()->setSelection( i );
		}
	}
	else
		MLogger::logError(
			"Settings::onInit: MWndNamespace::getWndNS( blCombo ) failed" );

	MToggleButton* sfCheck = (MToggleButton*) MWndNamespace::getObjNS( this, "sfButton" );
	if( sfCheck )
	{
		sfCheck->setValue(
			MProperties::getInt(
				MAudioEngine::gvStickyFocusTag,
				1,
				0,
				1 ) == 1  );
	}
	else
		MLogger::logError(
			"Settings::onInit: MWndNamespace::getObjNS( sfButton ) failed" );
}

MDialog::DlgResult Settings::onDlgCommand( unsigned int id )
{
	switch( id )
	{
	case IDOK:
		{
			MComboBox* dxCombo = (MComboBox*) MWndNamespace::getObjNS( this, "dxCombo" );
			if( dxCombo )
				MProperties::storeInt(
					MAudioEngine::gvDxDeviceTag,
					dxCombo->getModel()->getSelection() );
			else
				MLogger::logError(
					"Settings::onDlgCommand: MWndNamespace::getObjNS( dxCombo ) failed" );

			MComboBox* blCombo = (MComboBox*) MWndNamespace::getObjNS( this, "blCombo" );
			if( blCombo )
				MProperties::storeInt(
					MAudioEngine::gvBufferLengthTag,
					gvDefBufLen[ blCombo->getSelection() ] );
			else
				MLogger::logError(
					"Settings::onDlgCommand: MWndNamespace::getObjNS( blCombo ) failed" );

			MToggleButton* sfCheck = (MToggleButton*) MWndNamespace::getObjNS( this, "sfButton" );
			if( sfCheck )
				MProperties::storeInt( MAudioEngine::gvStickyFocusTag, sfCheck->getValue() ? 1 : 0 );
			else
				MLogger::logError(
					"Settings::onDlgCommand: MWndNamespace::getObjNS( sfButton ) failed" );
		}
		return MDialog::DlgResult::OK;
		break;
	default:
		return MDialog::DlgResult::CANCEL;//MDialog::onDlgCommand( id );
		break;
	}
}

void Settings::onDestroy()
{
}