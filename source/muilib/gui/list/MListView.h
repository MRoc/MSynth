#ifndef __IListViewListener
#define __IListViewListener

#include "../MWndCollection.h"
#include <framework/list/MSelectableObjectList.h>
#include <framework/primitive/MInsets.h>

/**
 * defines a listener to beeing informed about
 * changes on the listview
 */
class IListViewListener :
	public IListener
{
public:

	/**
	 * invoked when selection chagned
	 */
	virtual void onSelection() = 0;

	/**
	 * invoked when pre selection
	 * (by mouse over) changed
	 */
	virtual void onPreSelection() = 0;
};

/**
 * implementation of the listview
 * TODO: inheritance from MyInteractiveWnd
 */
class MListView : public MWndCollection
{
public:

	static const int CY_SIZE;

protected:

	/**
	 * private listener class for beeing
	 * informed about changed on the
	 * list view model
	 */
	class MListModelListener;

	/**
	 * instance of the model listener
	 */
	MListModelListener* ivPtListener;

	/**
	 * reference to the model
	 */
	MSelectableObjectList* ivPtModel;

	/**
	 * list of registered listeners
	 */
	MListenerList ivListeners;

	/**
	 * the index of the preselection
	 * (the mouse over pre selection)
	 */
	int ivPreSelection;

	/**
	 * flag enabling the mouse over pre selection
	 */
	bool ivPreSelectionEnabled;

public:

	FULL_RTTI_SUPER( MListView, "MListView", MWndCollection );

	MListView();
	MListView( MSelectableObjectList* ptList );

	void init();
	virtual ~MListView();
	
	void onModelChanged();
	void onselectionChanged();
	
	MSelectableObjectList* getModel();
	void setModel( MSelectableObjectList* ptList );

	void setPreSelection( int preSelection );
	int getPreSelection();

	void setPreSelectionEnabled( bool enable );
	bool getPreSelectionEnabled();

	MSize getPreferredSize();

	void addListViewListener( IListViewListener* ptWnd );
	void removeListViewListener( IListViewListener* ptWnd );

	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	static MObject* createInstance(){ return new MListView(); }

protected:

	virtual bool processEvent( IUiEvent* pEvent );

	void fireActionPerformed();
	void firePreselect();

	class MListModelListener :
		public MObject,
		public IObjectListListener,
		public ISelectionListener
	{
	protected:
		MListView* ivPtView;
	public:
		FULL_RTTI( MListView::MListModelListener, "MListView::MListModelListener" );
		MListModelListener( MListView* ptView );
		~MListModelListener();
		virtual void objectAdded( MObject* obj );
		virtual void objectRemoved( MObject* obj );
		virtual void allObjectsRemoved();
		virtual void selectionChanged( MObject* newSelection, MObject* ptSource );
	};
};		

#endif