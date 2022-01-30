/**
 * (C)2000-2003 MRoc
 */

#include "MainApp.h"
#include <framework/resource/text/MTextResource.h>
#include <framework/io/MIoException.h>
#include <gui/MTopWnd.h>

//MainApp mApp;

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
}

bool MainApp::onInit()
{
	if( ! MAudioEngine::initDxDescs() )
		return false;

#ifndef _DEBUG
	MAppData::APPDIR = getAppDir();
#else
	MAppData::APPDIR = "D:/code/msynth/msynth/";
#endif
	MAppData::APPDIR.Replace( "\\", "/" );

	MLogger::logMessage( "<mainapp::oninit appdir=\"%s\"/>\n", MAppData::APPDIR.getData() );

#ifndef _DEBUG
	//try
	{
		CSplashWnd::getInstance()->create();
		CSplashWnd::getInstance()->setText( MAppData::VERSION );
		CSplashWnd::getInstance()->start();
	}
	/*catch( ... )
	{
		MLogger::logError( "unknown error creating splash string" );
		return false;
	}*/
#endif

	//MCommandLineParser cmdInfo;
	//ParseCommandLine( cmdInfo );

	try
	{
		MResourceManager::getInstance()->loadArchive( MAppData::APPDIR, "app" );
		MResourceManager::getInstance()->loadArchive( MAppData::APPDIR + "resource", "resource" );
	}
	catch( MIoException ae )
	{
		MLogger::logError( ae.getExceptionDescripton() );
		return false;
	}
	catch( ... )
	{
		MLogger::logError( "unknown error loading language" );
		return false;
	}

	try
	{
		MProperties::loadProperties( "resource/settings/settings.xml" );
	}
	catch( MException ae )
	{
		MLogger::logError( ae.getExceptionDescripton() );
	}
	catch( ... )
	{
		MLogger::logError( "unknown error loading properties" );
	}

	try
	{
		MTextResource::getInstance()->load( "resource/text/english.xml" );
	}
	catch( MException ae )
	{
		MLogger::logError( ae.getExceptionDescripton() );
		return false;
	}
	catch( ... )
	{
		MLogger::logError( "unknown error loading language" );
		return false;
	}

	try
	{
		initFactories();
	}
	catch( ... )
	{
		MLogger::logError( "unknown error initializing factories" );
		return false;
	}

	try
	{
		MXMLConverter::getInstance();
	}
	catch( ... )
	{
		MMessageBox::showError(
			"unknown error while instantiating xml converter" );
		return false;
	}

	try
	{
		initAudioDMOList();
	}
	catch( MException ae )
	{
		MLogger::logError( ae.getExceptionDescripton() );
		return false;
	}
	catch( ... )
	{
		MLogger::logError( "unknown error initializing dmo list" );
		return false;
	}
	
	//try
	{
		setMainWnd( new MainFrame( "" ) );
		getMainWnd()->create( MRect(), (MTopWnd*)0 );
		getMainWnd()->setVisible( true );
	}
	/*catch( ... )
	{
		MLogger::logError( "unknown error instantiating mainwindow" );
		return false;
	}*/


	//this->ivAccTable.load( IDR_ACC_MAINFRAME );


	return true;
}

bool MainApp::onExit() 
{
	MApp::onExit();
    
	try
	{
		MProperties::saveProperties( "resource/settings/settings.xml" );
	}
	catch( MException ae )
	{
		MMessageBox::showError(
			ae.getExceptionDescripton() );
	}

	try
	{
		MIdMap::release();
		MAudioEngine::releaseDxDescs();
		MSourceRegistry::release();
		MTransformerRegistry::release();
		MDescriptorViewFactory::getInstance()->release();
		MNoteFactory::getInstance()->release();
		MInstrumentEditorFactory::getInstance()->release();
		MFont::release();
		MToolTipCtrl::release();
		MClipBoard::release();
		MFileHandling::release();
		MXMLConverter::release();
		MTargetRegistry::release();
		MProperties::release();
		MDefaultFactory::release();
		MResourceManager::release();
		MTextResource::release();
	}
	catch( ... )
	{
		MMessageBox::showError(
			"unknown error releasing singleton components" );
	}

	return true;
}

void MainApp::initFactories()
{
	MDefaultFactory* ptObjFactory = MDefaultFactory::getInstance();
	ptObjFactory->store( &( MTreeConverterDocument::gvRtti ) );
	ptObjFactory->store( &( MElementMatch::gvRtti ) );
	ptObjFactory->store( &( MAttributeMatch::gvRtti ) );
	ptObjFactory->store( &( MAttributeSubStr2Attribute::gvRtti ) );
	ptObjFactory->store( &( MElementRenamer::gvRtti ) );
	ptObjFactory->store( &( MAttributeAdder::gvRtti ) );
	ptObjFactory->store( &( MChildElementEnumeration::gvRtti ) );
	ptObjFactory->store( &( MAttributeRenamer::gvRtti ) );
	ptObjFactory->store( &( MAttribute2Element::gvRtti ) );
	ptObjFactory->store( &( MInt2FloatConverter::gvRtti ) );
	ptObjFactory->store( &( MAttributeContentReplacer::gvRtti ) );
	ptObjFactory->store( &( MSampleBankImport::gvRtti ) );
	ptObjFactory->store( &( MSampleImport::gvRtti ) );
	ptObjFactory->store( &( MLayoutWnd::gvRtti ) );
	ptObjFactory->store( &( MLayoutRow::gvRtti ) );
	ptObjFactory->store( &( MLayoutCol::gvRtti ) );
	ptObjFactory->store( &( MLayoutLeaf::gvRtti ) );
	ptObjFactory->store( &( MLayoutRoot::gvRtti ) );
	ptObjFactory->store( &( MLayout::gvRtti ) );
    
	// sources, oscis
	ptObjFactory->store( &( MMultiOsc::gvRtti ) );
	ptObjFactory->store( &( MOscSaw::gvRtti ) );
	ptObjFactory->store( &( MOscSine::gvRtti ) );
	ptObjFactory->store( &( MOscSquare::gvRtti ) );
	ptObjFactory->store( &( MAnalogSquare::gvRtti ) );
	ptObjFactory->store( &( MOscTriangle::gvRtti ) );
	ptObjFactory->store( &( MWaveOsc::gvRtti ) );
	ptObjFactory->store( &( MOscString::gvRtti ) );
	ptObjFactory->store( &( MOrgan::gvRtti ) );
	ptObjFactory->store( &( MNoise::gvRtti ) );
	ptObjFactory->store( &( MAAOsc::gvRtti ) );

	MSourceRegistry *ptSrcRegistry = MSourceRegistry::getInstance();
	ptSrcRegistry->store( new MObjectDescription( "MultiOsc", &MMultiOsc::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Saw", &MOscSaw::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Sine", &MOscSine::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Square", &MOscSquare::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "AnalogSquare", &MAnalogSquare::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Triangle", &MOscTriangle::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Wave", &MWaveOsc::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "String", &MOscString::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Organ", &MOrgan::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "Noise", &MNoise::gvRtti ) );
	ptSrcRegistry->store( new MObjectDescription( "MAAOsc", &MAAOsc::gvRtti ) );

	// transformer
	ptObjFactory->store( &( MIIR::gvRtti ) );
	ptObjFactory->store( &( MEnvelope::gvRtti ) );
	ptObjFactory->store( &( MClipper::gvRtti ) );
	ptObjFactory->store( &( MFuzz::gvRtti ) );
	ptObjFactory->store( &( MLofi::gvRtti ) );
	ptObjFactory->store( &( MDelay::gvRtti ) );
	ptObjFactory->store( &( MReverb::gvRtti ) );
	ptObjFactory->store( &( MPeakEq::gvRtti ) );
	ptObjFactory->store( &( MDMOWrapper::gvRtti ) );
	ptObjFactory->store( &( MOneZeroFilter::gvRtti ) );
	ptObjFactory->store( &( MOnePoleFilter::gvRtti ) );
	ptObjFactory->store( &( MFIR::gvRtti ) );
	ptObjFactory->store( &( MComb::gvRtti ) );

	MTransformerRegistry* ptTransReg = MTransformerRegistry::getInstance();
	ptTransReg->store( new MTransformerRtti( "Filter", &( MIIR::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Envelope", &( MEnvelope::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Clipper", &( MClipper::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Fuzz", &( MFuzz::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Lofi", &( MLofi::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Delay", &( MDelay::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Equalizer", &( MPeakEq::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Reverb", &( MReverb::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "OneZeroFilter", &( MOneZeroFilter::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "OnePoleFilter", &( MOnePoleFilter::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "FIR", &( MFIR::gvRtti ), 0 ) );
	ptTransReg->store( new MTransformerRtti( "Comb filter", &( MComb::gvRtti ), 0 ) );

	// VIEW FACTORY
	MDescriptorViewFactory *viewFactory = MDescriptorViewFactory::getInstance();

	viewFactory->registerDescriptorView( DESC_VIEW_NORMAL, MDescriptorView::createInstance );
	viewFactory->registerDescriptorView( DESC_VIEW_WAVEOSC, MWaveOscView::createInstance );
	viewFactory->registerDescriptorView( DESC_VIEW_MIIR, MIIRControl::createInstance );
	viewFactory->registerDescriptorView( DESC_VIEW_MULTIOSC, MMultiOscControl::createInstance );

	// NOTE LIST FACTORY
	MNoteFactory *noteFactory = MNoteFactory::getInstance();
	noteFactory->registerNoteList( MDefaultSynthNoteList::TYPE_ID, MDefaultSynthNoteList::createInstance, MNoteFrame::createInstance );
	noteFactory->registerNoteList( MBeatBoxNoteList::TYPE_ID, MBeatBoxNoteList::createInstance, MBeatBoxNoteEditor::createInstance );
	noteFactory->registerNoteList( MAudioTriggerList::TYPE_ID, MAudioTriggerList::createInstance, MAudioTriggerFrame::createInstance );

	MInstrumentEditorFactory *editorFactory = MInstrumentEditorFactory::getInstance();
	editorFactory->registerEditor( Synth::TYPE_ID, MSynthEditor::createInstance );
	editorFactory->registerEditor( MBeatBox::TYPE_ID, MBeatBoxEditor::createInstance );
	editorFactory->registerEditor( MAudioChannel::TYPE_ID, MAudioChannelEditor::createInstance );
}

void MainApp::initAudioDMOList()
{
    IEnumDMO* pDMOEnum = 0; 
    HRESULT hrNext;
    DWORD ulNumInfoReturned;
    CLSID clsidCurrentDMO;
    WCHAR* unicodeDmoName;

    // Enumerate all the registered DMOs registered as DMOCATEGORY_AUDIO_EFFECT.
    HRESULT hr =
		DMOEnum(
			DMOCATEGORY_AUDIO_EFFECT,
			DMO_ENUMF_INCLUDE_KEYED,
			0, // Number of input partial media types.
			NULL,
			0, // Number of output partial media types.
			NULL,
			&pDMOEnum );

    if( FAILED( hr ) )
		throw MException( "MainApp::initAudioDMOList: DMOEnum failed" );

	do
    {
        hrNext = pDMOEnum->Next( 1, &clsidCurrentDMO, &unicodeDmoName, &ulNumInfoReturned );
        if( FAILED( hrNext ) )
		{
            SAFE_RELEASE( pDMOEnum );
        }
		else if( ulNumInfoReturned == 1 )
		{  
            TCHAR tempStr[MAX_PATH];
            if( wcstombs( tempStr, unicodeDmoName, MAX_PATH ) == -1 )
				throw MException( 
					"MainApp::initAudioDMOList: wcstombs function failed." );

			String name = String( "DMO-" ) + String( tempStr );
			MTransformerRegistry::getInstance()->store(
				new MTransformerRtti(
					name,
					&( MDMOWrapper::gvRtti ),
					new MDmoWrapperInitParam( clsidCurrentDMO, name ) ) );
            
            CoTaskMemFree( unicodeDmoName );
        }
    } while( (S_OK == hrNext) );

    SAFE_RELEASE( pDMOEnum );
}

String MainApp::getAppDir()
{
	char buffer[MAX_PATH+1];
	DWORD back = GetModuleFileName(
		0,       // handle to module
		buffer,  // path buffer
		MAX_PATH // size of buffer
	);
	ASSERT( back );
	String str( buffer );
	int index = str.ReverseFind( '\\' );
	return str.Left( index + 1 );
}

MApp* createApp()
{
	return new MainApp();
}
