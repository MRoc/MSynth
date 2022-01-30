#ifndef __MFLOATEVENTINSPECTOR
#define __MFLOATEVENTINSPECTOR

#include <gui/MWnd.h>
#include "IBarInspector.h"
#include "../../../model/control/MFloatControl.h"
#include "../../../model/automation/IEventListCollection.h"

class MFloatEventInspector :
	public MObject,
	public IBarInspector,
	public IBarListener,
	public IEventListCollectionListener
{
private:

	MSize ivSize;
	MSize* ivPtPartSize;
	int ivOffsetX;

	MBar* ivPtBar;

	MWnd* ivPtContainer;

	IEventTarget* ivPtCurTarget;

public:

	FULL_RTTI( MFloatEventInspector, "MFloatEventInspector" );

	MFloatEventInspector();
	virtual ~MFloatEventInspector();

	virtual String getName();

	virtual void setContainingWnd( MWnd* ptContainer );
	virtual MWnd* getContainingWnd();

	virtual void setCurrentTarget( IEventTarget* ptTarget );
	virtual IEventTarget* getCurrentTarget();

	virtual void setBar( MBar* ptBar );
	virtual MBar* getBar();

	virtual ToolTipInfo getToolTip( MPoint point );

	virtual void setSize( MSize dimension );
	virtual MSize getSize();

	virtual void setPartSize( MSize* ptSize );
	virtual MSize* getPartSize();

	virtual void setOffsetX( int x );
	virtual int getOffsetX();

	virtual void paint( IGraphics* ptGraphics, const MRect& rect );

	virtual bool onMouseDown( MMouseEvent* pEvent );
	virtual bool onMouseUp( MMouseEvent* pEvent );
	virtual bool onMouseMoved( MMouseEvent* pEvent );

	virtual bool getForwardEvents();

	virtual void release();

	virtual void setInspectorInformation( MObject* ptObject );

	// inheritanced from IEvenListCollectionListener
	virtual void eventAdded( IAutomationEvent* ptEvent );
	virtual void eventRemoved( IAutomationEvent* ptEvent );
	virtual void allEventsRemoved();
	virtual void eventListAdded( IAutomationEventList* ptList );
	virtual void eventListRemoved( IAutomationEventList* ptList );
	virtual void eventListCollectionReleasing();

	virtual void barColChanged(){}
	virtual void barRowChanged(){}
	virtual void barLengthChanged(){}
	virtual void barResolutionChanged(){}

	virtual void barReleasing();

private:

	unsigned int getIndex();

	MFloatEvent* getEvent( MPoint point );
	MRect getBar( int index, FP value );

	unsigned int getIndex( MPoint point );
	FP getValue( unsigned int index );

	String getTargetName();
};

#endif