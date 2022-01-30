/*
	(C)2001 MRoc hifiShock
*/

#ifndef __MBarControl
#define __MBarControl

#include "../dcgrid/MDCControl.h"
#include "../noteeditor/MAbstractNoteFrame.h"
#include "../../model/MBar.h"

class MBarControl :
	public MDCControl,
	public IFrameWndListener
{
public:

	static int				STD_WIDTH;
	static int				STD_HEIGHT;

protected:

	MBar*					ivPtBar;
	unsigned int			ivChannel;

	MAbstractNoteFrame*		ivPtEditor;

public:

	MBarControl( MBar* ptBar );
	virtual ~MBarControl();

	virtual void paintControl( IGraphics* pDC );

	void setBar( MBar *bar );
	MBar* getBar();

	void setChannel( unsigned int channel );
	unsigned int getChannel();

	void setEditor( MAbstractNoteFrame* ptEditor );
	MAbstractNoteFrame* getEditor();

	virtual void closeEditor();

	virtual void* getInterface( const String &className ) const;

	virtual void onClose( MTopWnd* pWnd );
	virtual void onDestroy( MTopWnd* pWnd );
};

class MBCComparator : public MDefaultDCComparator
{
public:

	MBCComparator( SortType sortMode )
		: MDefaultDCComparator( sortMode )
	{
	};

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 )
	{
		MBarControl* ptBC1 = (MBarControl*) ptControl1;
		MBarControl* ptBC2 = (MBarControl*) ptControl2;
		MBar* ptBar1 = ptBC1->getBar();
		MBar* ptBar2 = ptBC2->getBar();
		unsigned int ns1 = ptBar1->getIndex();
		unsigned int ns2 = ptBar2->getIndex();
		unsigned int row1 = ptBC1->getChannel();
		unsigned int row2 = ptBC2->getChannel();

		int back = 0;
		switch( ivSortMode )
		{
			case STD_LO_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = row1 - row2;
				break;
			case STD_HI_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = row2 - row1;
				break;
			case REV_LO_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = row1 - row2;
				break;
			case REV_HI_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = row2 - row1;
				break;
		};
		return back;
	};
};

#endif