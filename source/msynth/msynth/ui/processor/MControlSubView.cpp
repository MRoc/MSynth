#include "MControlSubView.h"

MControlSubView::MControlSubView(
	IControlCollection* ptControlCollection,
	MColor bkColor )
	:
	MWndCollection(),
	ivPtControlCollection( ptControlCollection )
{
	setBkColor( bkColor );

	int xx = 5;
	unsigned int count = ivPtControlCollection->getControlCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MDefaultControl* _ptControl = (MDefaultControl*) ivPtControlCollection->getControl( i );
		ASSERT( _ptControl );

		if( _ptControl->getViewID() == DESC_TYPE_SLIDER )
		{
			MFloatControl* ptFloat = (MFloatControl*)_ptControl;
			MFloatGui* ptControl = new MFloatGui( ptFloat, getBkColor() );
			ptControl->setPoint( MPoint( xx, 5 ) );
			ivArControls.push_back( ptControl );
			addChild( ptControl );
			xx += (5+ptControl->getWidth());
		}
		else if( _ptControl->getViewID() == DESC_TYPE_BUTTON )
		{
			MBoolControl* ptBool = (MBoolControl*)_ptControl;
			MBoolGui* ptControl = new MBoolGui( ptBool, getBkColor() );
			ptControl->setPoint( MPoint( xx, 5 ) );
			ivArControls.push_back( ptControl );
			addChild( ptControl );
			xx += (5+ptControl->getWidth());
		}
		else
			ASSERT( true );
	}
	setSize( MSize( xx+5, 55 ) );
	doLayout();
}

MControlSubView::~MControlSubView()
{
	ivPtControlCollection = 0;
}

void MControlSubView::doLayout()
{
	int xx = 5;
	unsigned int count = ivArControls.size();
	for( unsigned int i=0;i<count;i++ )
	{
		ivArControls[ i ]->setPoint(
			MPoint(
				xx,
				5 ) );
		xx += ivArControls[ i ]->getWidth() + 5;
	}
}

void MControlSubView::paint( IGraphics* ptGraphics, const MRect& rect )
{
	ptGraphics->fillRect( rect, ivBkColor );
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}
