/*

	MAudioTriggerControl (C)2001 MRoc hifiShock

	Represents a Control to paint on the DC, but this class is not a window,
	the paint-method must be called explicit with a given device-context

*/

#ifndef __MAudioTriggerControl
#define __MAudioTriggerControl

#include "../../../model/audiochannel/MAudioTrigger.h"
#include "../../../model/interfaces/IBpmListener.h"
#include "../../dcgrid/mdccontrol.h"

class MAudioTriggerControl :
	public MDCControl
{
protected:

	MAudioTrigger* ivPtTrigger;
	unsigned int ivStepWidthInPixel;
	FP ivPeaksPerPixel;

public:

	MAudioTriggerControl( MAudioTrigger* ptTrigger );
	virtual ~MAudioTriggerControl();

	void			setPeaksPerPixel( FP peaksPerPixel );
	FP				getPeaksPerPixel();
	void			setStepWidthInPixel( unsigned int width );
	unsigned int			getStepWidthOnPixel();

	virtual void	paintControl( IGraphics* pDC );
	virtual void	paintPeak( IGraphics* pDC, unsigned char* ptPeaks, unsigned int length, MRect &rect );

	void			setTrigger( MAudioTrigger *ptTrigger );
	MDefaultNote*	getTrigger();
};

class MATCComparator : public MDefaultDCComparator
{
public:

	MATCComparator( SortType mode )
		: MDefaultDCComparator( mode )
	{
	};

	virtual int compare( MDCControl* ptControl1, MDCControl* ptControl2 )
	{
		MAudioTrigger* ptNote1 = (MAudioTrigger*) (((MAudioTriggerControl*)ptControl1)->getTrigger());
		MAudioTrigger* ptNote2 = (MAudioTrigger*) (((MAudioTriggerControl*)ptControl2)->getTrigger());
		unsigned int ns1 = ptNote1->getIndex();
		unsigned int ns2 = ptNote2->getIndex();

		int back = 0;
		switch( ivSortMode )
		{
			case STD_LO_PRIOR:
				back = ns1 - ns2;
				break;
			case STD_HI_PRIOR:
				back = ns1 - ns2;
				break;
			case REV_LO_PRIOR:
				back = ns2 - ns1;
				break;
			case REV_HI_PRIOR:
				back = ns2 - ns1;
				break;
		};
		return back;
	};
};

#endif