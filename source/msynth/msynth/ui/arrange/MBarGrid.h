/*

	MBarGrid (C)2001 MRoc hifiShock

	A ScrollPane displaying the MBarControls,
	double buffered, bar can be created, dragged and edited.

*/

#ifndef __MBARGRID
#define __MBARGRID

#include <framework/MTypes.h>

#include "../dcgrid/MDCGrid.h"
#include "MArrangeBar.h"
#include "../noteeditor/MAbstractNoteFrame.h"
#include <gui/MGuiUtils.h>

#include "../../model/MTimeController.h"
#include "../../model/Sequencer.h"
#include "../../model/MBar.h"
#include "../../model/MBarContainer.h"
#include "../../model/MSong.h"

class MBarGrid :
	public MDCGrid
{
protected:

	class MSongListener;

	MSong*			ivPtSong;
	bool			ivBarEditMode;
	int				ivCurrentResolution;
	MTimeController* ivPtTimeControl;
	IBpmControl*	ivPtBpmControl;
	MSongListener*	ivPtListener;

public:
	FULL_RTTI_SUPER( MBarGrid,"MBarGrid",  MDCGrid );

	MBarGrid( MSong *ptSong, MTimeController* ptTimeControl, IBpmControl* ptBpmControl, MSize* ptSize );
	virtual ~MBarGrid();

	virtual void deleteControls();

	virtual void closeChildFrames();
	virtual void storeChildFramePosition();

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	virtual void copy();
	virtual void cut();
	virtual void paste( int index );
	virtual void deleteSelected();

	void updateFromModel(); // OK
	void updateFromPartSize(); // OK

	bool hasSelectedControls();
	void deleteFirstSelection();

	MBarControl* getFirstSelection();

	void barAdded( MBar* ptBar, int row );

	void changeResolution( int resolution );

	virtual void updateSelection( MRect rect );

protected:

	virtual int getMaxRows();
	virtual int getMaxCols();

	virtual int getRow( MDCControl* ptControl );
	virtual int getCol( MDCControl* ptControl );

	virtual int getColSpan( MDCControl* ptControl );

// Info setter/getter...

	void setSong( MSong *ptSong ); // OK
	MSong* getSong(); // OK

// EVENTS...

	virtual void onMouseDown( MMouseEvent* anEvent );
	virtual void onMouseUp( MMouseEvent* anEvent );
	virtual void onMouseDoubleClick( MMouseEvent* anEvent );
	virtual void onMouseMove( MMouseEvent* anEvent );

	virtual	bool onSetCursor();

	MDCControl* createControl( MPoint point );  // OK
	MBarControl* getControlForBar( MBar* ptBar );
	MBarControl* createBarControl( MBar* ptBar, int row );

	void layoutControl( MDCControl *ptControl, int startsAt, int stopsAt ); // OK
	virtual void deleteControl( MDCControl* ptControl );

	bool glowBar( MPoint point, MBarControl* control );
	bool cutBar( MPoint point, MBarControl* control );

	void startControlEditing( MDCControl* control );
	void stopControlEditing();

	void startControlMoving( MBarControl* control );
	virtual void stopControlMoving( MPoint point );

	bool updateControlEditing( MPoint point );

	void moveControl( MDCControl* _ptControl, int rowDiv, int colDiv );

	MBCBWList* getCopy();
	void paste( MBCBWList* ptList, unsigned int index );

	friend class MSongListener;
	class MSongListener :
		public MObject,
		public ISongListener
	{
	protected:

		MSong* ivPtSong;
		MBarGrid* ivPtEditor;

	public:
		FULL_RTTI( MBarGrid::MSongListener, "MBarGrid::MSongListener" );
		MSongListener( MSong* ptSong, MBarGrid* ptEditor );
		virtual ~MSongListener();

		virtual void instrumentAdded( unsigned int index );
		virtual void instrumentRemoved( unsigned int index );
		virtual void instrumentsSwapped( unsigned int index1, unsigned int index2 );
		virtual void songModelChanged();
		virtual void songDestroy();

		virtual String getClassName();
	};
};

#endif
