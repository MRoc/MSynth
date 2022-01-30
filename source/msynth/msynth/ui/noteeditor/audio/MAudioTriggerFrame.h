/*

to do:

	- fix combo selection bug, if more combos on one model, selection does not the redrawaction

	- fix partsize in class MAudioChannelGrid

	- introduce edit mode + user actions...

	- implement drawing of waves

	- implement method inheritanced from MDCGrid

	- implement the MAudioChannel

*/

#ifndef __MAudioTriggerFrame
#define __MAudioTriggerFrame

#include <framework/performance/MClock.h>
#include <gui/button/MToggleButton.h>
#include "MAudioChannelGrid.h"
#include "../MAbstractNoteFrame.h"
#include "../../control/inspector/MFloatEventInspector.h"
#include "../../../model/audiochannel/MAudioTriggerList.h"
#include "../../../model/audiochannel/MAudioChannel.h"

class MAudioTriggerFrame :
	public MAbstractNoteFrame
{
protected:

	MComboBox*			ivPtAudioFiles;

	int					ivDividerPosY1,
						ivDividerPosY2,
						ivDividerPosX1;

	MFloatEventInspector *ivPtFEInspector;

public:

	MAudioTriggerFrame();
	virtual ~MAudioTriggerFrame();

	virtual void				setBar( MBar* ptBar );

	virtual bool				create( MRect rect, MTopWnd *parentWnd );

	virtual void				updateFromModel();
	virtual void				doLayout();

	virtual void onCopy();
	virtual void onCut();
	virtual void onPaste();
	virtual void onDelete();

	static MAbstractNoteFrame*	createInstance();

//DECLARE_MESSAGE_MAP()
};

#endif