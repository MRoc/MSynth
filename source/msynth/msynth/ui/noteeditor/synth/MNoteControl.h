/*

	MNoteControl (C)2001 MRoc hifiShock

	Represents a Control to paint on the DC, but this class is not a window,
	the paint-method must be called explicit with a given device-context

*/

#ifndef __MNoteControl
#define __MNoteControl

#include "../../dcgrid/MDCControl.h"
#include "../../../model/note/MDefaultSynthNote.h"

class MNoteControl :
	public MDCControl
{
protected:

	MDefaultNote		*ivPtNote;

public:

	MNoteControl( MDefaultNote *ptNote );
	virtual ~MNoteControl();

	virtual void paintControl( IGraphics* pDC );
	void setNote( MDefaultNote *ptNote );
	MDefaultNote* getNote();
};

class MMCComparator : public MDefaultDCComparator
{
public:

	MMCComparator( SortType sortMode )
		: MDefaultDCComparator( sortMode )
	{
	};

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 )
	{
		MDefaultSynthNote* ptNote1 = (MDefaultSynthNote*) (((MNoteControl*)ptControl1)->getNote());
		MDefaultSynthNote* ptNote2 = (MDefaultSynthNote*) (((MNoteControl*)ptControl2)->getNote());
		unsigned int ns1 = ptNote1->getIndex();
		unsigned int ns2 = ptNote2->getIndex();
		unsigned int row1 = ptNote1->getValue();
		unsigned int row2 = ptNote2->getValue();

		int back = 0;
		switch( ivSortMode )
		{
			case STD_LO_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = row2 - row1;
				break;
			case STD_HI_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = row1 - row2;
				break;
			case REV_LO_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = row2 - row1;
				break;
			case REV_HI_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = row1 - row2;
				break;
		};
		return back;
	};
};

#endif

