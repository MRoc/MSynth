/*

	MDCGrid (C)2001-2002 MRoc hifiShock

*/

#ifndef __MDCGrid
#define __MDCGrid

#include <gui/MyInteractiveWnd.h>

#include "MDCControlSelectionManager.h"


typedef std::vector<MDCControl*> DCControlList;
typedef DCControlList::const_iterator ControlIter;

class IDCGridListener :
	public IListener
{
public:
	virtual void onGridContentChanged() = 0;
};

class MDCGrid : public MyInteractiveWnd
{
public:

	enum EditMode
	{
		SELECT_MODE,
		DRAW_MODE,
		CUT_MODE,
		GLOW_MODE
	};

protected:

	MListenerList			ivListeners;

	EditMode				ivEditMode;

	bool					ivMouseCaptured,
							ivMoveMode;

	MPoint					ivPointMouseDown,
							ivPointMouseUp,
							ivCurrentEditOffset;

	MSize*					ivPtPartSize;

	DCControlList			ivControls;
	MDCControl*				ivPtEditControl;

	MDCControlSelectionManager* ivPtSelectionManager;

public:

	MDCGrid( MSize* ptPartSize );

	virtual ~MDCGrid();

	void					setEditMode( EditMode mode );
	EditMode				getEditMode();
	void					updateFromPartSize();

	virtual void			paint( IGraphics* ptGraphics, const MRect &rect ) = 0;

	virtual void			updateFromModel() = 0;
	virtual MSize			calculateScrollViewSize();
	virtual void			copy() = 0;
	virtual void			paste( int index ) = 0;
	virtual void			deleteSelected() = 0;

	MDCControlSelectionManager* getSelectionManager();

	void					addListener( IDCGridListener* pListener );
	void					removeListener( IDCGridListener* pListener );

protected:

	void					fireContentChanged();

	virtual MDCControl*		getControlAt( const MPoint& point );

	virtual int				getMaxRows() = 0;
	virtual int				getMaxCols() = 0;

	virtual int				getRow( MPoint point );
	virtual int				getCol( MPoint point );

	virtual int				getRow( MDCControl* ptControl ) = 0;
	virtual int				getCol( MDCControl* ptControl ) = 0;

	virtual int				getColSpan( MDCControl* ptControl ) = 0;

	virtual void			onMouseDown( MMouseEvent* anEvent );
	virtual void			onMouseUp( MMouseEvent* anEvent );
	virtual void			onMouseMove( MMouseEvent* anEvent );
	virtual void			onMouseDoubleClick( MMouseEvent* anEvent );

	void					setPartSize( MSize* ptSize );
	MSize*					getPartSize();

	virtual void			deleteControls();
	virtual int				GetIndexOfControl( MDCControl* ptControl );
	virtual void			startControlMoving( MDCControl *control );
	virtual void			stopControlMoving( MPoint point ) = 0;
	virtual void			updateSelection( MRect rect );
	virtual void			layoutControl( MDCControl *control );
	virtual MDCControl*		createControl( MPoint point ) = 0;
	virtual void			deleteControl( MDCControl* ptControl ) = 0;

	virtual bool			processEvent( IUiEvent* pEvent );
};

#endif