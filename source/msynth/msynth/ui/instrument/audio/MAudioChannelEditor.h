#ifndef __MAudioChannelEditor
#define __MAudioChannelEditor

#include <framework/MTypes.h>
#include <gui/list/MScrolledListView.h>
#include "../MInstrumentEditor.h"
#include "../../../model/audiochannel/MAudioChannel.h"
#include "../../../../resource.h"

class MAudioChannelEditor :
	public MInstrumentEditor
{
protected:

	MMenu*					ivPtMenu;
	MScrolledListView*		ivPtListView;

public:

	MAudioChannelEditor(void);
	virtual ~MAudioChannelEditor();

	virtual void			doLayout( bool autoSize );

	virtual bool			create( MRect rect, MTopWnd *parentWnd );

	virtual void			setInstrument( MInstrument* ptInstrument );

	static MInstrumentEditor* createInstance();

protected:

	virtual void			onCommand( unsigned int id );
};

#endif