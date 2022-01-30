#ifndef __MScrollBarButton
#define __MScrollBarButton

#include "MScrollBarModel.h"
#include "../button/MButton.h"
#include "../timer/MTimer.h"

/**
 * implementation of a scrollbar button.
 * such a button has an arrow and an
 * increment/decrement type.
 * when pressed, a thread is started
 * inc/decrementing the scrollbar model.
 */
class MScrollBarButton :
	public MButton,
	public IActionListener
{
public:

	/**
	 * the scrollbar button type
	 */
	enum ScrollBarButtonType
	{
		DECREMENT_BUTTON,
		INCREMENT_BUTTON
	};

protected:

	/**
	 * the trackingstate of the button
	 */
	enum MScrollBarButtonState
	{
		NEUTRAL,
		FIRST,
		SECOND,
		THIRD
	};

	/**
	 * the sleep time used in the inc/dec thread
	 */
	static const int SLEEP_TIME;

	/**
	 * the sleep time used after the button
	 * was pressed the first time
	 ´*/
	static const int FIRST_SLEEP_TIME;

	/**
	 * the button type (inc/dec)
	 */
	ScrollBarButtonType ivButtonType;

	/**
	 * the scrollbar model
	 */
	MScrollBarModel* ivPtModel;

	/**
	 * the timer used for tracking
	 */
	MTimer ivTimer;

	/**
	 * the button's trackingstate
	 */
	MScrollBarButtonState ivState;

public:

	/**
	 * constructor
	 */
	MScrollBarButton(
		MArrowRenderer::Direction direction,
		ScrollBarButtonType type,
		MScrollBarModel* ptModel );

	/**
	 * destructor
	 */
	virtual ~MScrollBarButton();

	/**
	 * invoked when the mouse was pressed
	 */
	virtual void onMouseDown( MMouseEvent* ptEvent );

	/**
	 * invoked when the mouse was released
	 */
	virtual void onMouseUp( MMouseEvent* ptEvent );

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * the timer callback
	 */
	virtual void onActionPerformed( void* ptSrc );

protected:

	/**
	 * does a inc- or dec on the model depending on button's direction
	 */
	void track();
};

#endif