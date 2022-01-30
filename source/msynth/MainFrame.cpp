/*-----------------------------------------------------------------------------

	MSynth (C)2000-2003 MRoc hifiShock

	required libs:

		dsound.lib
		winmm.lib
		xerces-c_2D.lib
		dxguid.lib
		dmoguids.lib
		amstrmid.lib
		msdmo.lib
		comctl32.lib
		kernel32.lib
		user32.lib
		gdi32.lib
		winspool.lib
		comdlg32.lib
		advapi32.lib
		shell32.lib
		ole32.lib
		oleaut32.lib
		uuid.lib
		shlwapi.lib
		dxerr8.lib


	HISTORY

	v. 0.8			- Saveable Samplebanks
	v. 0.8a			- Editable SampleNames	
	v. 0.8b			- Saveing machinestate with pattern
	v. 0.8c			- Synth with Portamento
	v. 0.81			- bugfixes(samplePlayer), optimized, working
	v. 0.81a		- bugfix
	v. 0.81b		- bugfix maxAmp, title with fileNames
	v. 0.82 beta	- StreamingAudio BufferCaps fixed, MSynth.ini for DxDevice,
					- MExceptions in WaveFileHandling(30%)
	v. 0.83 beta	- Dialog for StreamingMode (Render current/whole pattern,
					- record live-session),
					- Sequencer with renderFunction,
					- MExceptionBugFix in MWavePlayer
	v. 0.83 		- Performance-Tune in MControlButton, added more Accelerators
					- (return -> play/stop)
	v. 0.84			- Synth engine rewritten -> more OOP, vtable,
					- SynthControl,
					- WavePlayerControl
					- CenterDialogs,
					- bugfix slide in triangleOscilator
					- MEGA-Performancetune in MWavePlayer (interpolation)
					- SettingsDialog with choosable BufferLength & StickyFocus
					- (saved in msynth.ini)
					- Bitmap-Controls do not need CRect anymore, only x, y
	v. 0.85			- FX-dialog-try
	v. 0.86			- XML-FileFormat
	v. 0.87			- dynamical GUI
	v. 0.88	beta	- Eventdriven DrumBox, Memoryleaks removes first betas out...
	v. 0.89			- Synthcount

	v. 0.9 beta		- working
	v. 0.9a			- some fixes, introducing SequencerControl
	v. 0.9b			- new MAudioEngine
	v. 0.91			- MBeatBox in a own Frame
	v. 0.92			- MTransformatorCollection for MBeatBox (MasterFX for drums)
	v. 0.92 b		- MPollingStreamingAudio and MAudioEngine
	v. 0.92 c		- Link in MAboutDialog, working link to Help
	v. 0.92 d		- help-link allways works(with error message),
					- transformer collection editor better userinterface
					- (add/remove automatic selection)
	v. 0.92 d2		- some codecleanings(SAFE_DELETE, ...)
					- removed some unused files from project
					- opened fx-editor when exiting app -> crash fixed
	v. 0.93 beta	- opening pattern loads samplebank,
					- new Oscillator, MOscString,
					- new MDescriptedViewFactory,
					- MNoteSetp reacting on tab and cursor,
					- mutebutton(drumboxChannel) triggers note-off
					- void reset() in all transformers
					- frames are brought to front
	v. 0.94 beta	- introducing Mixer Frame
	v. 0.95 beta	- REWRITTEN -> now bufferBased,
					- stereo-out
					- many performance-tunes, better than ever before
	v. 0.96 alphaDV - introducting aplha state of MArrangeView and MSong,
					- MNoteFrame, MNoteGrid, MNoteControl, IAbstractNote,
					- IAbstractNoteList, MArrangeBar...
					- doublebuffered sliders,
					- removed memoryleak when	exited over menu(file->exit),
					- new Oscilator MSineOsc
	v. 0.98 alpha	- Removed everything from MainFrame -> MSong becomes the
					- most important component.
	v. 0.98			- introduces MTimeController, MArrangeView & Mixer most
					- important components.

	v. 1.0 beta1-3	- removes selectObject GDC bug,
					- added new splash
					- fixed bug in mbar serialisation (max startsAt was 16)
					- introducing MClipBoard
					- faster GUI work, FillRect -> FillSolidRect, (less brushes)
	v. 1.0 beta4	- MColorMap, MMultiOsc, MIIR (IIR), popup slider
	v. 1.0 beta5	- bugs in grids and windows removed, send fx
	v. 1.0 beta6	- much more bugfixes
	v. 1.0 beta7	- MGuiUtils, M3D***Buttons
	v. 1.0 beta8	- MProperties, MObject, MDefaultFactory...
	v. 1.0.0		- First demo release
	v. 1.0.1		- MSI Installer
	v. 1.0.2		- Interfaces -> I... , BpmControl,...

	v. 1.1.0 betadmo - DirectMediaObject Audiofilter support,
					- IControl, IControlCollection...
	v. 1.1.0 beta	- Better controls with OnHover/RoolOver,
					- StringTable, english resources,
					- MUpdateThread
	v. 1.1.0.2		- ControlRegistry, AutomationTracker...

	v. 1.2.0.0		- better automation, buttons changed, bugs...
	v. 1.2.2.0		- removed fatal delete bug in arrange grid, bundled with docu
	v. 1.2.3.0		- Offscreen windows, interactive components, own scrollbars, own spinButtons, own combox
	v. 1.2.4.0		- introducing the sequencercontrol, removed MUpdateThread's own position calculation 

	v. 1.3.0.0		- replaced all combos ->new bugs fixed
					- saves the editors' positions
					- many bugfixes:
						crashs when two bars with open editor are glown
						mem leaks when open bar editor and recording events
						loading synth when playing resulted in critical section deadlock
						waves from the windows audiorecorder can be loaded now
						envelope forgotten in moogfilter, but not working worth
						rescaling beatbox notes was possible by increasing resolution of bbbar from 16 up to 32
					- controlguis only rerendering is necessary
					- fx rack frame hided behind mainframe
					- mscrollwnd replaced updatewindow -> redrawwindow (failed in release version)

	v. 1.4.0.0		- audiochannels as new instrument
					- many memory leaks removed
					- xerces xml compiled with same runtime version???
					- complete restructuring code into packages
					- introduced Streams
					- rewrote MWave and introduced MWaveFileHeader
					- removed CWndListenerList from models, introduced normal interface observer
					- introduced MTreeDocument, completely new serialisation
					- introduced new tree document converter (MXmlConverter with matchings and converter)
					- waves saved in song file base64 encoded
					- MControlGui inheritanced from MyInteractiveWnd, interactiveWnd now handles right mouse (MMouseEvent)
					- small changes in grid classes
					- fixes in all sine oscis (oscis&lfo), more stable recursive algo found
					- rewrote MNoise based on a linear kongruence algo
					- more and more objects become a MObject
					- new MTransformer- and MSourceRegistry, transformer and oscs created by MObjectFactory
					- maximal loudness 2x because of silly fuck bug
					- introduced MCriticalSection & MSynchronisation
					- introduced MThread and IRunnable for work posted in gui thread, less wnd msgs
					- introduced IGraphics...
					- introduced MWndContainer...
					- introduced MLayout
					- MAudioEngine got MAudioThread
					- EQs bases on MBiquad
					- Sequencer rewritten, stream oriented...
					- Mixer rewritten
					#
	v. 1.5.0		- resource manager, string resource
	v. 1.5.1		- dynamics, patterns
					- rewrote GUI MFC->win32

-----------------------------------------------------------------------------*/

#include "MainFrame.h"
#include <gui/MApp.h>
#include <gui/dialog/MFileDialog.h>

#define SEQUENCER_CONTROL_ID	1000
#define ARRANGE_ID				2000
#define MIXER_ID				3000
#define FXRACK_ID				4000

#define DEF_WIDTH 640
#define DEF_HEIGHT 480

MainFrame::MainFrame( String fileName ) :
	ivPtSequencerControlWnd(0),
	ivPtMixerControl(0),
	ivPtArrangeFrame(0),
	ivPtFxRackFrame(0),
	ivFileName(""),
	ivFullFileName(""),
	ivPtSequencerControl( 0 )
{
	setActionOnClose( MTopWnd::ActionOnClose::QUIT );
}

MainFrame::~MainFrame()
{
	MApp::getInstance()->setMainWnd( 0 );

	MLogger::logMessage( "<mainframe::~mainframe/>\n" );

	if( ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::PLAYING ||
		ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::SESSION_PLAYING ||
		ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::SESSION_NOT_PLAYING )
	{
		ivPtSequencerControl->stop();
	}

	if( ivPtArrangeFrame )
	{
		removeChild( ivPtArrangeFrame );
		delete ivPtArrangeFrame;
		ivPtArrangeFrame = 0;
	}
	if( ivPtSequencerControlWnd )
	{
		removeChild( ivPtSequencerControlWnd );
		delete ivPtSequencerControlWnd;
		ivPtSequencerControlWnd = 0;
	}
	if( ivPtSequencerControl )
	{
		delete ivPtSequencerControl;
		ivPtSequencerControl = 0;
	}

	MTimeController::release();
}

bool MainFrame::create( MRect rect, MTopWnd* pWnd )
{
	int x = (GetSystemMetrics( SM_CXSCREEN )-DEF_WIDTH) / 2;
	int y = (GetSystemMetrics( SM_CYSCREEN )-DEF_HEIGHT) / 2;
	bool back = MFrameWnd::create( MRect( x, y, DEF_WIDTH, DEF_HEIGHT ), pWnd );
	if( back == false )
		MLogger::logError( "MainFrame::MainFrame: create failed" );
	else
	{
		MMenu* pMenu = new MMenu();
		pMenu->load( IDR_MENU1 );
		setMenu( pMenu );

		setIcon( new MIcon( IDI_ICON1 ) );

		// init models...
		init();	

		// init controls...
		ivPtSequencerControlWnd = new MSequencerControlWnd();
		ivPtSequencerControlWnd->SetSequencerControl( ivPtSequencerControl );
		ivPtSequencerControlWnd->setRect( MRect( 0, 0, 630, 55 ) );
		addChild( ivPtSequencerControlWnd );
		ivPtSequencerControlWnd->updateFromModel();

		ivPtArrangeFrame =
			new MArrangeFrame(
				ARRANGE_ID,
				ivPtSequencerControl->getSong(), 
				ivPtSequencerControl->getTimeControl(),
				ivPtSequencerControl->getUpdateThread(),
				ivPtSequencerControl->getSequencer() );
		ivPtArrangeFrame->setTimeController( ivPtSequencerControl->getTimeControl() );
		addChild( ivPtArrangeFrame );
		ivPtArrangeFrame->updateFromModel();
		ivPtArrangeFrame->updateFromPartSize();

		ivPtArrangeFrame->setVisible( true );
		updateWindowText();
		doLayout();

		/*if( ! fileName.IsEmpty() )
		{
			open( fileName );
			ivPtSequencerControlWnd->updateFromModel();
			ivPtSequencerControl->start();
		}*/
	}
	return back;
}

void MainFrame::onClose()
{
#ifndef _DEBUG
	if( MMessageBox::showQuestion( "QUIT" ) == IDYES )
#endif
	{
		if( ivPtArrangeFrame )
			ivPtArrangeFrame->closeChildFrames();

		SAFE_DELETE( ivPtFxRackFrame );
		SAFE_DELETE( ivPtMixerControl );
		MFrameWnd::onClose();
	}
	MApp::getInstance()->setQuit();
}

void MainFrame::init()
{
	MLogger::logMessage( "<mainframe::init/>\n" );

	// song...
	Sequencer* ptSequencer = new Sequencer();
	MSong* ptSong = new MSong( ptSequencer );
	ptSong->newSong();

	// sequencer...
	ptSequencer->setSong( ptSong );
	ptSequencer->setTimeController( MTimeController::getInstance() );

	// update thread...
	MUpdateThread* ptUpdateThread = new MUpdateThread(
		MTimeController::getInstance(),
		ptSequencer );

	ivPtSequencerControl = new MSequencerControl();
	ivPtSequencerControl->setSequencer( ptSequencer );
	ivPtSequencerControl->setUpdateThread( ptUpdateThread );
	ivPtSequencerControl->setSong( ptSong );
	ivPtSequencerControl->setTimeControl( MTimeController::getInstance() );

	initStreamingEngine();
}

void MainFrame::initStreamingEngine()
{
	MLogger::logMessage( "<mainframe::initstreamingengine/>\n" );

	if( ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::PLAYING ||
		ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::SESSION_PLAYING ||
		ivPtSequencerControl && ivPtSequencerControl->getPlayState() == IPlayState::SESSION_NOT_PLAYING )
	{
		ivPtSequencerControl->stop();
	}

	try
	{
		ivPtSequencerControl->getAudioEngine()->create(	ivPtSequencerControl->getSequencer() );
	}
	catch( MException ae )
	{
		MMessageBox::showError( ae.getExceptionDescripton() );
		exit( -1 );
	}
}

void MainFrame::updateWindowText()
{
	String title = MApp::getInstance()->getAppName();
	if( ivFileName != "" )
		title += " [" + ivFileName + "]";
	setText(title);
}

void MainFrame::doLayout()
{
	MSize size = getClientSize();
	if( ivPtSequencerControlWnd )
		ivPtSequencerControlWnd->setRect( MRect( 0, 0, size.getWidth(), 55 ) );
	if( ivPtArrangeFrame )
		ivPtArrangeFrame->setRect( MRect( 0, 55, size.getWidth()-1, size.getHeight() - 55 ) );
}

void MainFrame::onNew()
{
	MLogger::logMessage( "<mainframe::onnew/>\n" );

	if( ivPtArrangeFrame )
		ivPtArrangeFrame->closeChildFrames();

	ivPtSequencerControl->getSong()->newSong();
	ivPtSequencerControl->getSequencer()->newSong();

	ivFileName = "";
	ivFullFileName = "";
	updateWindowText();
}

void MainFrame::onOpen()
{
	MLogger::logMessage( "<mainframe::open/>\n" );
	MFileDialog dlg;
	if( dlg.showOpenDialog(
		this,
		"Open song",
		"MSynth Song\0*.MXL\0All files\0*.*\0\0" ) )
	{
		open( dlg.getSelectedFile() );
		ivFileName = dlg.getSelectedFile();
		ivFullFileName = dlg.getSelectedFile().Left( dlg.getSelectedFile().ReverseFind( '\\' ) );
		if( ivPtSequencerControlWnd )
			ivPtSequencerControlWnd->updateFromModel();
		updateWindowText();
	}
}

void MainFrame::onSave()
{
	MLogger::logMessage( "<mainframe::onsave/>\n" );

	if( ivFullFileName != "" )
	{
		ivPtArrangeFrame->storeChildPositions();
		MTreeNode* ptNode = ivPtSequencerControl->getSequencer()->save();
		MFileHandling::getInstance()->saveMXLFile(
			ivFullFileName,
			ptNode );
		delete ptNode;
	}
	else
		onSaveAs();
}

void MainFrame::onSaveAs()
{
	MLogger::logMessage( "<mainframe::onsaveas/>\n" );

	MFileDialog dlg;
	if( dlg.showSaveDialog(
		this,
		"Save song",
		"MSynth Song\0*.MXL\0All files\0*.*\0\0" ) )
	{
		ivFileName = dlg.getSelectedFile();
		ivFullFileName = dlg.getSelectedFile().Left( dlg.getSelectedFile().ReverseFind( '\\' ) );

		/*CFileFind finder;
		if( finder.FindFile( ivFullFileName ) )
			if( MMessageBox::showQuestion( "OVERWRITE", ivFullFileName ) != IDYES )
				return;*/

		this->ivPtArrangeFrame->storeChildPositions();
		String songName = ivFileName.Left( ivFileName.ReverseFind( '.' ) );
		String dir = ivFullFileName.Left( ivFullFileName.ReverseFind( '\\' ) );
		MTreeNode* ptNode = ivPtSequencerControl->getSequencer()->save();
		MFileHandling::getInstance()->saveMXLFile(
			dlg.getSelectedFile(),
			ptNode );
		delete ptNode;

		updateWindowText();
	}
}

void MainFrame::onExportAsWave()
{
	if( ivPtSequencerControlWnd )
		ivPtSequencerControlWnd->Export();
}

void MainFrame::onCopy()
{
	IClipBoardContext* pContext = MClipBoard::getInstance()->getContext();
	if( pContext )
		pContext->onCopy();
	else
		MMessageBox::showError( "NO_WINDOW_SUPPORTING_COPY" );
}

void MainFrame::onCut()
{
	IClipBoardContext* pContext = MClipBoard::getInstance()->getContext();
	if( pContext )
		pContext->onCut();
	else
		MMessageBox::showError( "NO_WINDOW_SUPPORTING_CUT" );
}

void MainFrame::onPaste()
{
	IClipBoardContext* pContext = MClipBoard::getInstance()->getContext();
	if( pContext )
		pContext->onPaste();
	else
		MMessageBox::showError( "NO_WINDOW_SUPPORTING_PASTE" );
}

void MainFrame::onDelete()
{
	IClipBoardContext* pContext = MClipBoard::getInstance()->getContext();
	if( pContext )
		pContext->onDelete();
	else
		MMessageBox::showError( "NO_WINDOW_SUPPORTING_DELETE" );
}

void MainFrame::showMixer()
{
	if(! ivPtMixerControl)
	{
		ivPtMixerControl = new MMixerFrame();
		ivPtMixerControl->create( MRect( 0, 0, 100, 100 ), this ); 
		ivPtMixerControl->setMixer( ivPtSequencerControl->getSequencer()->getMixer() );	
		ivPtMixerControl->updateGui();
	}
	ivPtMixerControl->updateFromModel();
	ivPtMixerControl->setVisible( true );
}

void MainFrame::showFxRack()
{
	if( ! ivPtFxRackFrame )
	{
		ivPtFxRackFrame = new MFxRackFrame();
		ivPtFxRackFrame->setFxRack( ivPtSequencerControl->getSequencer()->getMixer()->getFxRack() );
		ivPtFxRackFrame->create( MRect( 0, 0, 100, 100 ), this ); 
		ivPtFxRackFrame->setIcon( new MIcon( *getIcon() ) );
	}
	ivPtFxRackFrame->setVisible( true );
}

void MainFrame::onCreateSynth()
{
	ivPtSequencerControl->getSong()->createDefaultSynth();
}

void MainFrame::onCreateBeatBox()
{
	ivPtSequencerControl->getSong()->createDefaultBeatBox();
}

void MainFrame::onCreateAudiochannel()
{
	ivPtSequencerControl->getSong()->createDefaultAudioChannel();
}

void MainFrame::onReset()
{
	ivPtSequencerControl->getSong()->resetAllInstruments();
}

void MainFrame::onSettingsDialog()
{
	ASSERT(
		ivPtSequencerControl &&
		ivPtSequencerControl->getAudioEngine() );

 	Settings settingsDialog;
	settingsDialog.create( MRect( 0, 0, 400, 600 ), (MTopWnd*)getTopParent() );
	if( settingsDialog.doModal() == MDialog::OK )
	{
		if( ivPtArrangeFrame )
			ivPtArrangeFrame->closeChildFrames();
		initStreamingEngine();
	}
}

void MainFrame::onAbout()
{
	MAboutDialog dialog;
	dialog.setCopyright( MAppData::COPYRIGHT );
	dialog.setVersion( MAppData::VERSION );
	dialog.doModal();
}

void MainFrame::onHelp()
{
	if( ((LPARAM)ShellExecute( 0, "open", MAppData::APPDIR + "doc\\index.html", 0, 0, SW_SHOWNORMAL )) <= 32 )
		MMessageBox::showError( "ERROR_STARTING_HELP" );
}

void MainFrame::onPlay()
{
	ivPtSequencerControlWnd->Play();
}

void MainFrame::onActivate( bool active )
{
	if( active )
		MClipBoard::getInstance()->setContext( ivPtArrangeFrame );	
}

void MainFrame::open( String fileName )
{
	MLogger::logMessage( "<mainframe::open filename=\"%s\"/>\n", fileName.getData() );
	if( ivPtSequencerControl &&
		(ivPtSequencerControl->getPlayState() == IPlayState::PLAYING ||
		ivPtSequencerControl->getPlayState() == IPlayState::SESSION_PLAYING ||
		ivPtSequencerControl->getPlayState() == IPlayState::SESSION_NOT_PLAYING ) )
	{
		ivPtSequencerControl->stop();
	}

	try
	{
		if( ivPtArrangeFrame )
			ivPtArrangeFrame->closeChildFrames();
 		if( ! MFileHandling::getInstance()->openFile( fileName, ivPtSequencerControl->getSequencer() ) )
		{
			MMessageBox::showError( "ERROR_LOADING_SONG", fileName );
		}
	}
	catch(MException anException)
	{
		MMessageBox::showError( "ERROR_LOADING_SONG", fileName + "(" + anException.getExceptionDescripton() + ")" );
	}
}

void MainFrame::onCommand( unsigned int id )
{
	switch( id )
	{
	case ID_NEW_SONG: onNew(); break;
	case ID_OPEN_SONG: onOpen(); break;
	case ID_SAVE_SONG: onSave(); break;
	case ID_SAVE_SONG_AS: onSaveAs(); break;
	case ID_EXPORT_SONG: onExportAsWave(); break;
	case ID_EXIT: onClose(); break;

	case ID_COPY: onCopy(); break;
	case ID_PASTE: onPaste(); break;
	case ID_CUT: onCut(); break;
	case ID_DELETE: onDelete(); break;

	case ID_SHOW_MIXER: showMixer(); break;
	case ID_SHOW_FXRACK: showFxRack(); break;

	case ID_SETTINGS_SETTINGS: onSettingsDialog(); break;

	case ID_ABOUT_ABOUT: onAbout(); break;
	case ID_SHOW_HELP: onHelp(); break;

	case ID_CREATESYNTH: onCreateSynth(); break;
	case ID_CREATEBEATBOX: onCreateBeatBox(); break;
	case ID_INSTRUMENT_CREATEAUDIOCHANNEL: onCreateAudiochannel(); break;
	case ID_RESET: onReset(); break;

	case ID_QUIT: onClose(); break;
	
	default: MFrameWnd::onCommand( id ); break;
	}
}
