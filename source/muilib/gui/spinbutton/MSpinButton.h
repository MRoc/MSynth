#ifndef __MSpinButton
#define __MSpinButton

#include "../MWndCollection.h"
#include "MSpinButtonModel.h"
#include "../timer/MTimer.h"
#include "../button/MButton.h"


class MSpinButton :
	public MWndCollection,
	public IButtonListener,
	public IActionListener
{
public:

	enum Orientation
	{
		HORIZONTAL,
		VERTICAL
	};

	enum TimerState
	{
		TS_NEUTRAL,
		TS_PRE,
		TS_POST
	};

	enum TrackingDirection
	{
		TD_INC,
		TD_NEUTRAL,
		TD_DEC
	};

protected:

	MSpinButtonModel* ivPtModel;
	MButton* ivPtButtons[2];
	MTimer ivTimer;

	Orientation ivOrientation;
	TimerState ivState;
	TrackingDirection ivDirection;

public:

	FULL_RTTI_SUPER( MSpinButton, "MSpinButton", MWndCollection );

	MSpinButton( Orientation orientation );
	virtual ~MSpinButton(void);

	virtual void doLayout();

	virtual MSpinButtonModel* getModel();

	virtual void onButtonPressed( MButton* ptButton );
	virtual void onButtonReleased( MButton* ptButton );
	virtual void onActionPerformed( void* ptSrc );

protected:

	virtual void startTracking( TrackingDirection direction );
	virtual void stopTracking();
};

#endif