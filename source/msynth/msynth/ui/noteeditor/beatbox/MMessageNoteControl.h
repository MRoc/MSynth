/*

	MMessageNoteControl (C)2001 MRoc hifiShock

	Represents a Control to paint on the DC, but this class is not a window,
	the paint-method must be called explicit with a given device-context

*/

#ifndef __MMessageNoteControl
#define __MMessageNoteControl

#include <framework/MTypes.h>

#include "../../dcgrid/MDCControl.h"
#include "../../../model/note/MDefaultMessageNote.h"

class MMessageNoteControl :
	public MDCControl
{
protected:

	MDefaultMessageNote*	ivPtNote;
	MMessage*				ivPtMessage;

public:

	MMessageNoteControl( MDefaultMessageNote *ptNote, MMessage* ptMessage );
	virtual ~MMessageNoteControl();

	virtual void			paintControl( IGraphics* pDC );

	void					SetNote( MDefaultMessageNote *ptMessageNote );
	MDefaultNote*			GetNote();

	void					setMsg( MMessage* ptMessage );
	MMessage*				getMsg();
};

class MMNCComparator : public MDefaultDCComparator
{
public:

	MMNCComparator( SortType sortMode )
		: MDefaultDCComparator( sortMode )
	{
	}

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 )
	{
		MDefaultNote* ptNote1 = ((MMessageNoteControl*)ptControl1)->GetNote();
		MDefaultNote* ptNote2 = ((MMessageNoteControl*)ptControl2)->GetNote();
		unsigned int ns1 = ptNote1->getIndex();
		unsigned int ns2 = ptNote2->getIndex();
		MMessage* ptMsg1 = ((MMessageNoteControl*)ptControl1)->getMsg();
		MMessage* ptMsg2 = ((MMessageNoteControl*)ptControl2)->getMsg();
		int back = 0;
		switch( ivSortMode )
		{
			case STD_LO_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = ptMsg1->getId() - ptMsg2->getId();
				break;
			case STD_HI_PRIOR:
				back = ns1 - ns2;
				if( ! back )
					back = ptMsg2->getId() - ptMsg1->getId();
				break;
			case REV_LO_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = ptMsg1->getId() - ptMsg2->getId();
				break;
			case REV_HI_PRIOR:
				back = ns2 - ns1;
				if( ! back )
					back = ptMsg2->getId() - ptMsg1->getId();
				break;
		};
		return back;
	}
};

#endif