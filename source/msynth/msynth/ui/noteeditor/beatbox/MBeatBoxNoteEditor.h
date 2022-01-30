/*

  MBeatBoxNoteEditor (C)2001 MRoc hifiShock

*/

#ifndef __MBeatBoxNoteEditor
#define __MBeatBoxNoteEditor

#include <gui/button/MToggleButton.h>
#include "MBeatBoxChannelControlContainer.h"
#include "MBeatBoxMessageInspector.h"
#include "MBeatBoxGrid.h"
#include "../MAbstractNoteFrame.h"
#include "../../../../MAppData.h"
#include "../../clipboard/MClipBoard.h"

class MBeatBoxNoteEditor :
	public MAbstractNoteFrame,
	public IInstrumentListener
{
protected:

	int									ivDividerPosY1;
	int									ivDividerPosY2;
	int									ivDividerPosX1;

	MBeatBoxMessageInspector*			ivPtVolInspector;
	MFloatEventInspector*				ivPtFEInspector;
	MBeatBoxChannelControlContainer*	ivPtChannelControl;

public:

	FULL_RTTI_SUPER( MBeatBoxNoteEditor, "MBeatBoxNoteEditor", MAbstractNoteFrame );

	MBeatBoxNoteEditor();
	virtual ~MBeatBoxNoteEditor();

	virtual bool		create( MRect rect, MTopWnd *parentWnd );

	virtual void		updateFromModel();
	virtual void		doLayout();

	virtual void		setBar( MBar* ptBar );
	virtual void		setInstrument( MInstrument* ptInstrument );

	virtual void		onCopy();
	virtual void		onCut();
	virtual void		onPaste();
	virtual void		onDelete();

	void				onChannelSelected( unsigned int index );
	void				onUpdateFromModel();

	virtual void stateChanged( MInstrumentEvent anEvent );
	virtual String getInstrumentListenerName();

	static MAbstractNoteFrame* createInstance()
	{
		return new MBeatBoxNoteEditor();
	}
};



#endif

