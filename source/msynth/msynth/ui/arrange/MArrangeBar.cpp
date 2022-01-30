/*
	MBarControl (C)2001-2003 MRoc hifiShock

*/

#include "MArrangeBar.h"
#include <gui/MGuiUtils.h>

int MBarControl::STD_WIDTH = 30;
int MBarControl::STD_HEIGHT = 20;

/** constructor */
MBarControl::MBarControl( MBar *ptBar ) :
	MDCControl(),
	ivPtEditor( 0 ),
	ivPtBar( 0 ),
	ivChannel( 0 )
{
	setBar( ptBar );
	setEditor( 0 );
}

/** destructor */
MBarControl::~MBarControl()
{
	if( ivPtEditor )
	{
		delete ivPtEditor;
		ivPtEditor = 0;
	}
}

/** set bar */
void MBarControl::setBar( MBar *ptBar )
{
	ivPtBar = ptBar;
}

/** returns bar */
MBar* MBarControl::getBar()
{
	return ivPtBar;
}

/** returns channel index */
void MBarControl::setChannel( unsigned int channel )
{
	ivChannel = channel;
}

/** set channel index */
unsigned int MBarControl::getChannel()
{
	return ivChannel;
}

/** set editor */
void MBarControl::setEditor( MAbstractNoteFrame* ptEditor )
{
	if( ivPtEditor )
		ivPtEditor->removeListener( this );
	ivPtEditor = ptEditor;
	if( ivPtEditor )
		ivPtEditor->addListener( this );
}

/** returns editor */
MAbstractNoteFrame* MBarControl::getEditor()
{
	return ivPtEditor;
}

/** paints the control */
void MBarControl::paintControl( IGraphics* pDC)
{
	ASSERT( ivPtBar->getNoteList() != 0 );

	MColor bkColor, fgColor;

	if( getSelected() )
	{
		bkColor = getBkColorSelected();
		fgColor = getBkColor();
	}
	else
	{
		bkColor = ivPtBar->getNoteList()->getPreferedColor();
		fgColor = getBkColorSelected();
	}

	// Draw control
	pDC->fillRect( ivRect, bkColor );
	if( getHover() && ! getSelected() )
	{
		MRect frameRect(
			ivRect.getX(),
			ivRect.getY(),
			ivRect.getWidth() - 1,
			ivRect.getHeight() - 1 );
		MGuiUtils::paintRaisedBorder(
			pDC,
			frameRect, 
			bkColor  );
	}
	else if( getSelected() )
	{
		MRect frameRect(
			ivRect.getX(),
			ivRect.getY(),
			ivRect.getWidth() - 1,
			ivRect.getHeight() - 1 );
		MGuiUtils::paintSunkenBorder(
			pDC,
			frameRect, 
			bkColor  );
	}
	else
		pDC->drawRect( ivRect, fgColor );
}

void MBarControl::closeEditor()
{
	if( ivPtEditor )
	{
		delete ivPtEditor;
	}
}

void MBarControl::onClose( MTopWnd* pWnd )
{
	setEditor( 0 );
}

void MBarControl::onDestroy( MTopWnd* pWnd )
{
	setEditor( 0 );
}

void* MBarControl::getInterface( const String &className ) const
{
	if( className == "MBarControl" )
		return (void*) ((MBarControl*)this);
	else
		return MObject::getInterface( className );
}