#ifndef __MComboBoxPopup
#define __MComboBoxPopup

#include "../scrollbar/MScrollBar.h"
#include "../popup/MPopupWnd.h"
#include "../list/MListView.h"

#define COMBOBOX_CLOSED ( WM_USER + 4394 )

/**
 * implementation of a combo box popup.
 * this is a popup containing a scrollbar
 * and a listview.
 */
class MComboBoxPopup :
	public MPopupWnd
{
protected:

	class MComboPopupController;

	/** the listener handling events from the listview. */
	MComboPopupController* ivPtListener;

	/** the list view displaying the content */
	MListView* ivPtView;

	/** the scrollbar for scrolling the listview */
	MScrollBar* ivPtScrollBar;

public:

	/** constructor */
	MComboBoxPopup();

	/** constuctor */
	MComboBoxPopup( MSelectableObjectList* ptList );

	/** destructor */
	virtual ~MComboBoxPopup();

	/** creates the combo box */
	virtual bool create( MRect rect, MTopWnd* parent );

	/** layouts the content of this popup (inheritanced from MPopup) */
	virtual void doLayout();

	/** paints the content, in this case, it paints a border arround the listview. */
	virtual void paint( IGraphics* pGraphics, const MRect &rect );

	/** returns the model of this combobox popup */
	virtual MSelectableObjectList* getModel();

	/** sets the model of this combobox popup */
	virtual void setModel( MSelectableObjectList* ivPtModel );

	/** returns the preferred size of this popup */
	virtual MSize getPeferredSize();

protected:

	friend class MComboPopupController;

	/** handles the list view events */
	class MComboPopupController :
		public MObject,
		public IListViewListener,
		public IScrollBarListener
	{
	protected:
		MComboBoxPopup* ivPtPopup;
	public:
		FULL_RTTI( MComboBoxPopup::MComboPopupController, "MComboBoxPopup::MComboPopupController" );
		MComboPopupController( MComboBoxPopup* ptPopup );
		virtual ~MComboPopupController();
		virtual void onSelection(); // IListViewListener
		virtual void onPreSelection(); // IListViewListener
		virtual void valueChanged( void* source ); // IScrollBarListener
	};
};

#endif