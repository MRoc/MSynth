#include "MScrollBar.h"
#include <framework/io/MLogger.h>
#include "../MGUIUtils.h"
#include "../renderer/MArrowRenderer.h"

/** the runtime type info */
MRtti MScrollBar::gvRtti = MRtti( "MScrollBar", MScrollBar::createInstance );

/** the preferred button size */
const MSize MScrollBar::PREFERRED_BUTTON_SIZE = MSize( 16, 16 );

/** constructor */
MScrollBar::MScrollBar() :
	MWndCollection(),
	ivMode( HORIZONTAL ),
	ivPtModel( new MScrollBarModel() ),
	ivPtBed( 0 )
{
	ivPtButtons[0] = new MScrollBarButton( MArrowRenderer::LEFT, MScrollBarButton::DECREMENT_BUTTON, ivPtModel );
	ivPtButtons[1] = new MScrollBarButton( MArrowRenderer::RIGHT, MScrollBarButton::INCREMENT_BUTTON, ivPtModel );
	ivPtBed = new MScrollBarBed( ivPtModel, MScrollBarBed::HORIZONTAL );

	addChild( ivPtButtons[0] );
	addChild( ivPtButtons[1] );
	addChild( ivPtBed );
}

/** constructor */
MScrollBar::MScrollBar( ScrollbarType mode ) :
	MWndCollection(),
	ivMode( mode ),
	ivPtModel( new MScrollBarModel() ),
	ivPtBed( 0 )
{
	if( mode == MScrollBar::HORIZONTAL )
	{
		ivPtButtons[0] = new MScrollBarButton( MArrowRenderer::LEFT, MScrollBarButton::DECREMENT_BUTTON, ivPtModel );
		ivPtButtons[1] = new MScrollBarButton( MArrowRenderer::RIGHT, MScrollBarButton::INCREMENT_BUTTON, ivPtModel );
	}
	else
	{
		ivPtButtons[0] = new MScrollBarButton( MArrowRenderer::UP, MScrollBarButton::DECREMENT_BUTTON, ivPtModel );
		ivPtButtons[1] = new MScrollBarButton( MArrowRenderer::DOWN, MScrollBarButton::INCREMENT_BUTTON, ivPtModel );
	}
	ivPtBed = new MScrollBarBed( ivPtModel, ivMode==HORIZONTAL ? MScrollBarBed::HORIZONTAL : MScrollBarBed::VERTICAL );

	addChild( ivPtButtons[0] );
	addChild( ivPtButtons[1] );
	addChild( ivPtBed );
}

/** destructor */
MScrollBar::~MScrollBar()
{
	removeChild( ivPtButtons[0] );
	removeChild( ivPtButtons[1] );
	removeChild( ivPtBed );
	SAFE_DELETE( ivPtButtons[0] );
	SAFE_DELETE( ivPtButtons[1] );
	SAFE_DELETE( ivPtBed );
	SAFE_DELETE( ivPtModel );
}

/** layouts the scrollbar's childs */
void MScrollBar::doLayout()
{
	MRect rect = getRect();
	MSize buttonSize;
	switch( ivMode )
	{
		case VERTICAL:
			buttonSize = MSize(
				rect.getWidth(),
				clip( PREFERRED_BUTTON_SIZE.getHeight(), rect.getHeight() / 2 ) );
			break;
		case HORIZONTAL:
			buttonSize = MSize(
				clip( PREFERRED_BUTTON_SIZE.getWidth(), rect.getWidth() / 2 ),
				rect.getHeight() );
			break;
	}

	// move button 1
	MRect b1rect =
		MRect(
			0,
			0,
			buttonSize.getWidth(),
			buttonSize.getHeight() );
	if( ivPtButtons[0] )
		ivPtButtons[0]->setRect( b1rect );

	// set location of button two
	MRect b2rect;
	switch( ivMode )
	{
		case VERTICAL:
			b2rect.setY( rect.getHeight() - buttonSize.getHeight() );
			b2rect.setHeight( buttonSize.getHeight() );
			b2rect.setX( 0 );
			b2rect.setWidth( rect.getWidth() );
			break;
		case HORIZONTAL:
			b2rect.setY( 0 );
			b2rect.setHeight( rect.getHeight() );
			b2rect.setX( rect.getWidth() - buttonSize.getWidth() );
			b2rect.setWidth( buttonSize.getWidth() );
			break;
	}
	if( ivPtButtons[1] )
		ivPtButtons[1]->setRect( b2rect );
		 
	// move bed
	MRect bedRect;
	if( ivMode == VERTICAL )
	{
		bedRect.setX( 0 );
		bedRect.setWidth( rect.getWidth() );
		bedRect.setY( b1rect.getHeight() );
		bedRect.setHeight( rect.getHeight() - (b1rect.getHeight() + b2rect.getHeight()) );
		if( rect.getHeight() > (2*buttonSize.getHeight()) )
 			bedRect.setHeight( rect.getHeight() - (2*buttonSize.getHeight()) );
	}
	else if( ivMode == HORIZONTAL )
	{
		bedRect.setX( b1rect.getRight() );
		bedRect.setWidth( rect.getWidth() - (b1rect.getWidth()+b2rect.getWidth()) );
		bedRect.setY( 0 );
		bedRect.setHeight( rect.getHeight() );
		if( rect.getWidth() > (2*buttonSize.getWidth()) )
			bedRect.setWidth( rect.getWidth() - (2*buttonSize.getWidth()) );
	}
	if( ivPtBed )
	{
		ivPtBed->setRect( bedRect );
		ivPtBed->doLayout();
	}
}

/** clips the given value */
int MScrollBar::clip( int value, int maxValue )
{
	if( value > maxValue )
		value = maxValue;
	return value;
}

/** returns the scrollbar's model */
MScrollBarModel* MScrollBar::getModel()
{
	return ivPtModel;
}

/** returns the runtime type info */
IRtti* MScrollBar::getRtti() const
{
	return &gvRtti;
}

/** store ui state into treenode */
MTreeNode* MScrollBar::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "type", ivMode == VERTICAL ? "VERTICAL" : "HORIZONTAL" );
	return pBack;
}

/** load ui state from treenode */
void MScrollBar::load( MTreeNode* pNode )
{
	MWnd::load( pNode );
	ivMode = pNode->getAttribute( "type", "VERTICAL" ) == "VERTICAL" ? VERTICAL : HORIZONTAL;
	doLayout();
}