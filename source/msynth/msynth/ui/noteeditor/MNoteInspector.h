/*

	MNoteEditor (C)2001 MRoc hifiShock

	A scrollpane displaying the selected value of the note above in the MNoteGrid,
	to edit the selected value.

*/

#ifndef __MNoteInspector
#define __MNoteInspector

#include <gui/tooltip/MToolTipCtrl.h>
#include <gui/MyInteractiveWnd.h>
#include "../control/inspector/IBarInspector.h"
#include "../../model/MBar.h"

typedef std::vector<IBarInspector*> InspectorList;
typedef InspectorList::iterator InspectorListIter;

/**
 * interface for receiving note inspector change notifies
 */
class INoteInspectorListener :
	public IListener
{
public:
	/**
	 * invoked when note inspector made any changes on notes
	 */
	virtual void onInspectorChanged() = 0;
};

class MNoteInspector :
	public MyInteractiveWnd
{
protected:

	MPoint				ivPointMouseDown,
						ivPointMouseUp;

	MSize*				ivPtSize;

	MBar*				ivPtBar;
	InspectorList		ivPtInspectors;
	IBarInspector*		ivPtSelectedInspector;
	MListenerList		ivListeners;

public:

	MNoteInspector( MBar *ptBar, MSize* ptSize );
	virtual				~MNoteInspector();

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );

	void				setBar( MBar* ptNoteList );
	MBar*				getBar();

	void				setPartSize( MSize* ptSize );
	MSize*				getPartSize();

	void				addInspector( IBarInspector* ptInspector );
	void				removeInspector( IBarInspector* ptInspector );
	IBarInspector*		getInspectorByName( String name );
	void				selectInspectorByName( String name );
	void				setSelectedInspector( IBarInspector* ivPtInspector );

	virtual void		addListener( INoteInspectorListener* pListener );
	virtual void		removeListener( INoteInspectorListener* pListener );

protected:

	virtual void		fireChange();

	virtual void		onMouseDown( MMouseEvent* anEvent );
	virtual void		onMouseUp( MMouseEvent* anEvent );
	virtual void		onMouseMove( MMouseEvent* anEvent );
	virtual void		onMouseHover( MHoverEvent* anEvent );
	virtual void		onMouseLeave( MHoverEvent* anEvent );
/*
	afx_msg	void		OnLButtonDown( unsigned int flags, CPoint point );
	afx_msg	void		OnLButtonUp( unsigned int flags, CPoint point );
	afx_msg void		OnMouseMove( unsigned int flags, CPoint point );

	afx_msg void		OnDestroy();

	virtual LRESULT		onMouseHover( WPARAM wparam, LPARAM lparam );
	virtual LRESULT		onMouseLeave( WPARAM wparam, LPARAM lparam );

DECLARE_MESSAGE_MAP();
*/
};

#endif