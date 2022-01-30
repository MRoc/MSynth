/*

	MSynthControl (C)2000/2001 MRoc hifiShock

*/

#ifndef __SYNTHCONTROL
#define __SYNTHCONTROL

#include <gui/scrollbar\MScrollPane.h>
#include "MSynthStructureFrame.h"
#include "MSynthScrollView.h"
#include "../../processor/MDescriptorView.h"
#include "../MInstrumentEditor.h"

#include "../../../model/Synth.h"

class MSynthEditor :
	public MInstrumentEditor,
	public IFrameWndListener
{
protected:

	Synth*					ivPtSynth;

	MSynthScrollView*		ivSynthView;
	MScrollPane*			ivPtScrollPane;
	MSynthStructureFrame*	ivPtEditor;

public:

	FULL_RTTI_SUPER( MSynthEditor, "MSynthEditor", MInstrumentEditor );

	MSynthEditor();
	virtual ~MSynthEditor();

	virtual bool			create( MRect rect, MTopWnd *parentWnd );

	virtual void			doLayout();
	virtual void			doLayout( bool autoSize );
	virtual void			updateFromModel();
	void					UpdateTitle();

	virtual void			setInstrument( MInstrument* ptInstrument );
	virtual Synth*			getSynth();

	static MInstrumentEditor* createInstance();

	virtual void			onCommand( unsigned int id );

	//IFrameWndListener
	virtual void onClose( MTopWnd* pWnd );
	//IFrameWndListener
	virtual void onDestroy( MTopWnd* pWnd );

protected:

	void					OnOpen();
	void					OnSaveAs();

	void					OnCloseWindow();

	void					OnEdit();
	void					OnReset();

	void OnImport();
};


#endif
