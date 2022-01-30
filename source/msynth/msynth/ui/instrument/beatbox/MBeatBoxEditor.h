#ifndef __MBeatBoxEditor
#define __MBeatBoxEditor

#include "../../clipboard/MClipBoard.h"
#include "MChannelControl.h"
#include "../MInstrumentEditor.h"
#include "../../dialog/MBeatBoxDialog.h"
#include "../../../model/MBeatBox.h"

/**
 * (C)2000-2003 MRoc hifiShock
 *
 * a beatbox instrument editor
 * containing a beatboxchannelcontrol inna scrollpane.
 */
class MBeatBoxEditor : public MInstrumentEditor
{
protected:

	MBeatBox*			ivPtBeatBox;
	MChannelControl*	ivChannelControl;
	MScrollPane*		ivPtScrollPane;

public:

	MBeatBoxEditor();
	virtual ~MBeatBoxEditor();

	virtual bool	create( MRect rect, MTopWnd *parentWnd );

	virtual void	doLayout( bool autoSize );
	virtual void	createControls();
	virtual void	updateFromModel();
	virtual void	updateTitle();

	virtual void	setInstrument( MInstrument* ptInstrument );
	MBeatBox*		getBeatBox();

	virtual void doLayout();

protected:

	virtual void	onCommand( unsigned int id );

public:

	static MInstrumentEditor* createInstance(){	return new MBeatBoxEditor(); };
};

#endif