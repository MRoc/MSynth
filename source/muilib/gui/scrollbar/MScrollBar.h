#ifndef __MScrollBar
#define __MScrollBar

#include "MScrollBarButton.h"
#include "MScrollBarBed.h"

#include "../MWndCollection.h"

/** platform independent scrollbar */
class MScrollBar :
	public MWndCollection
{
public:

	/** the runtime type info */
	static MRtti gvRtti;

	/** the preferred button size */
	static const MSize PREFERRED_BUTTON_SIZE;

	/** type of scrollbar */
	enum ScrollbarType
	{
		VERTICAL,
		HORIZONTAL
	};

protected:

	/** type of scrollbar */
	ScrollbarType ivMode;

	/** the two inc/dec buttons */
	MScrollBarButton* ivPtButtons[2];
	
	/** the scrollbar bed */
	MScrollBarBed* ivPtBed;

	/** the scrollbar's model */
	MScrollBarModel *ivPtModel;

public:

	/** constructor */
	MScrollBar();

	/** constructor */
	MScrollBar( ScrollbarType mode );

	/** destructor */
	virtual ~MScrollBar(void);

	/** layouts the scrollbar's childs */
	virtual void doLayout();

	/** returns the scrollbar's model */
	virtual MScrollBarModel* getModel();

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** store ui state into treenode */
	virtual MTreeNode* save();

	/** load ui state from treenode */
	virtual void load( MTreeNode* pNode );

	static MObject* createInstance(){ return new MScrollBar(); }

protected:

	/** clips the given value */
	int clip( int value, int maxValue );
};

#endif