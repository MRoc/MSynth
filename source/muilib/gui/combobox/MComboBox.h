#ifndef __MComboBox
#define __MComboBox

#include "MComboBoxPopup.h"
#include "../MyInteractiveWnd.h"
#include "../label/MObjectLabel.h"
#include "../tooltip/MToolTipCtrl.h"

class MComboBox : public MyInteractiveWnd
{
public:

	static MRtti gvRtti;

protected:

	class MSelectableObjectListListener;
	MSelectableObjectListListener* ivPtListener;

	MComboBoxPopup*		ivPtPopup;

public:

	MComboBox();
	MComboBox( MSelectableObjectList* ivPtModel );
	virtual ~MComboBox();

	virtual void		setSelection( int index );
	virtual int			getSelection();

	virtual MObject*	getSelectedObject();

	virtual MSelectableObjectList*	getModel();
	virtual void					setModel( MSelectableObjectList* ivPtModel );

	virtual IRtti* getRtti() const;
	static MObject* createInstance();

protected:

	virtual void		paint( IGraphics* ptGraphics, const MRect &rect );
	virtual void		onMouseDown( MMouseEvent* ptEvent ); 

	class MSelectableObjectListListener :
		public MObject,
		public ISelectionListener
	{
	protected:
		MComboBox* ivPtCombo;
	public:
		FULL_RTTI( MComboBox::MSelectableObjectListListener, "MComboBox::MSelectableObjectListListener" );
		MSelectableObjectListListener( MComboBox* ptCombo );
		virtual ~MSelectableObjectListListener();
		virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
		virtual String getClassName();
	};
};

#endif