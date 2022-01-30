/*
	(C) 2001 MRoc hifiShock
*/

#ifndef __MNoteFrame
#define __MNoteFrame

#include <framework/performance/MClock.h>

#include "../MAbstractNoteFrame.h"
#include "MNotePiano.h"
#include "MNoteGrid.h"
#include "../../../../MAppData.h"

#include "../../../model/interfaces/MInstrument.h"
#include "../../../model/MBar.h"
#include "../../control/inspector/MVolumeInspector.h"
#include "../../control/inspector/MFloatEventInspector.h"
#include "../../clipboard/MClipboard.h"

class MNoteFrame :
	public MAbstractNoteFrame
{
protected:
	
	int					ivDividerPosY1,
						ivDividerPosY2,
						ivDividerPosX1;

	MNotePiano			*ivPtPiano;

	MVolumeInspector	*ivPtVolInspector;
	MFloatEventInspector *ivPtFEInspector;

public:

	MNoteFrame();
	virtual ~MNoteFrame();

	virtual bool	create( MRect rect, MTopWnd *parentWnd );

	virtual void	setBar( MBar* ptBar );

	virtual void	updateFromModel();
	virtual void	doLayout();

	virtual void	onCopy();
	virtual void	onCut();
	virtual void	onPaste();
	virtual void	onDelete();

	static MAbstractNoteFrame* createInstance();
};

#endif
