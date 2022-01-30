/*

	MainFrame (C)2000-2002 MRoc hifiShock

*/

#ifndef __MainFrame
#define __MainFrame



#include <framework/MTypes.h>
#include <framework/map/MProperties.h>
#include <framework/performance/MClock.h>
#include <gui/MAcceleratorTable.h>

#include "resource.h"

#include "msynth/model/Sequencer.h"
#include "msynth/model/MUpdateThread.h"
#include "msynth/model/MSequencerControl.h"
#include "msynth/model/MSong.h"
#include "msynth/model/file/MFileHandling.h"
#include "msynth/model/dmo/MDMOWrapper.h"
#include "msynth/model/automation/ITargetRegistry.h"
#include "msynth/model/streamingengine/MAudioEngine.h"

#include "msynth/ui/dialog/MAboutDialog.h"
#include "msynth/ui/splash/Splash1.h"

#include "msynth/ui/arrange/MSequencerControlWnd.h"
#include "msynth/ui/arrange/MArrangeFrame.h"
#include "msynth/ui/mixer/MMixerFrame.h"
#include "msynth/ui/fxrack/MFxRackFrame.h"
#include "msynth/ui/dialog/Settings.h"
#include "msynth/ui/dialog/MRecordDialog.h"

#include "gui/button/MButton.h"

class MainFrame :
	public MFrameWnd
{
protected:

	String						ivFileName,
								ivFullFileName;

	MSequencerControl*			ivPtSequencerControl;
	MSequencerControlWnd*		ivPtSequencerControlWnd;
	MMixerFrame*				ivPtMixerControl;
	MArrangeFrame*				ivPtArrangeFrame;
	MFxRackFrame*				ivPtFxRackFrame;

public:

	MainFrame( String fileName );
	virtual ~MainFrame();

	virtual bool		create( MRect rect, MTopWnd* pWnd );

	// menu items
	void				onNew();
	void				onOpen();
	void				onSave();
	void				onSaveAs();
	void				onExportAsWave();

	void				onCopy();
	void				onCut();
	void				onPaste();
	void				onDelete();

	void				showMixer();
	void				showFxRack();

	void				onCreateSynth();
	void				onCreateBeatBox();
	void				onCreateAudiochannel();
	void				onReset();
	
	void				onSettingsDialog();

	void				onAbout();
	void				onHelp();

	// accelerators
	void				onPlay();

	void				updateWindowText();

	virtual void		doLayout();

protected:

	virtual void		onCommand( unsigned int id );
	virtual void		onActivate( bool active );
	virtual void		onClose();

	void				open( String fileName );

	void				init();
	void				initStreamingEngine();
};


#endif