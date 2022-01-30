#include "MLayoutRow.h"

MRtti MLayoutRow::gvRtti = MRtti( "MLayoutRow", MLayoutRow::createInstance );

MLayoutRow::MLayoutRow()
{
}

MLayoutRow::~MLayoutRow()
{
}

IRtti* MLayoutRow::getRtti() const
{
	return &gvRtti;
}

void* MLayoutRow::getInterface( const String &className ) const
{
	if( className == "MLayoutRow" )
		return (void*) ((MLayoutRow*)this);
	else
		return MLayoutNode::getInterface( className );
}

MSize MLayoutRow::getSize( MWnd* ptWnd, MSize maxSize )
{
	int w = 0;
	int h = 0;
	unsigned int childCount = getChildCount();
	for( unsigned int i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );

		MSize cSize = ptChild->getSize( ptWnd, maxSize );

		if( ptChild->getSizing() == MLayoutLeaf::FIXED )
		{
			if( cSize.getHeight() > h )
				h = cSize.getHeight();
			w += cSize.getWidth() + 2;
		}
		else if( ptChild->getSizing() == MLayoutLeaf::HORIZONTAL )
		{
			if( cSize.getHeight() > h )
				h = cSize.getHeight();
		}
		else if( ptChild->getSizing() == MLayoutLeaf::BOTH )
		{
			/*if( cSize.getHeight() > h )
				h = cSize.getHeight();*/
		}
	}
	return
		MSize(
			w + ivInsets.left() + ivInsets.right(),
			h + ivInsets.top() + ivInsets.bottom() );
}

void MLayoutRow::doLayout( MWnd* ptWnd, MRect rect )
{
	//TRACE( "layoutrow %s\n", rect.toString() );
	unsigned int childCount = getChildCount();
	int x = rect.getX() + ivInsets.left();
	int y = rect.getY() + ivInsets.top();

	// calculate fixed width
	int rest = ivInsets.left() + ivInsets.right();
	unsigned int i;
	for( i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );
		if( ptChild->getSizing() == MLayoutRow::FIXED ||
			ptChild->getSizing() == MLayoutRow::VERTICAL )
			rest += ptChild->getSize( ptWnd, rect.getSize() ).getWidth() + 2;
	}

	// layout childs
	for( i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );

		MSize childSize = ptChild->getSize( ptWnd, rect.getSize() );
		int width = (int) childSize.getWidth();
		int height = (int) childSize.getHeight();
		int rectHeight = (int) rect.getHeight();
		int rectWidth = (int) rect.getWidth();

		int yy = y;
		int h = rectHeight - (ivInsets.top() + ivInsets.bottom());
		if( ivAlign == CENTER )
			yy = y + (h-height)/2;
		else if( ivAlign == BOTTOM )
			yy = y + (h-height);

		if( ptChild->getSizing() == MLayoutRow::FIXED )
		{
			ptChild->doLayout(
				ptWnd,
				MRect( x, yy, width, height ) );

			x += width + 2;
		}
		else if( ptChild->getSizing() == MLayoutRow::HORIZONTAL )
		{
			int w = rectWidth - rest;
			if( w < 0 )
				w = 0;
			ptChild->doLayout(
				ptWnd,
				MRect(
					x,
					yy,
					w,
					height ) );
			x += w + 2;
		}
		else if( ptChild->getSizing() == MLayoutRow::BOTH )
		{
			int h = rectHeight - ( ivInsets.top() + ivInsets.bottom() );
			int w = rectWidth - ( ivInsets.left() + ivInsets.right() ) - rest;
			if( w < 0 )
				w = 0;
			ptChild->doLayout(
				ptWnd,
				MRect(
					x,
					yy,
					w,
					h ) );
			x += w + 2;
		}
	}
}

/**
 * loads from the given tree node
 */
void MLayoutRow::load( MTreeNode* ptNode )
{
	MLayoutNode::load( ptNode );
	ivAlign = str2align( ptNode->getAttribute( "align" ) );
	unsigned int count = ptNode->getChildCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MTreeLeaf* ptChildLeaf = ptNode->getChild( i );
		if( ptChildLeaf->getType() == MTreeLeaf::NODE )
		{
			MTreeNode* ptChild1 = (MTreeNode*) ptChildLeaf;
			if( ptChild1->getName() == "children" )
			{
				unsigned int count1 = ptChild1->getChildCount();
				for( unsigned int j=0;j<count1;j++ )
				{
					MTreeLeaf* ptChildLeaf2 = ptChild1->getChild( j );
					if( ptChildLeaf2->getType() == MTreeLeaf::NODE )
					{
						MTreeNode* ptChild = (MTreeNode*) ptChildLeaf2;
 						MObject* ptObj = MDefaultFactory::getInstance()->createObject( ptChild );
						if( ptObj )
						{
							MLayoutLeaf* ptLeaf = (MLayoutLeaf*) ptObj->getInterface( "MLayoutLeaf" );
							if( ptLeaf )
								addChild( ptLeaf );
							else
								delete ptObj;
						}
					}
				}
			}
			else if( ptChild1->getName() == "insets" )
			{
				MTreeNode* pfirst = ptChild1->getFirstNode( ELEM_OBJECT );
				if( pfirst )
					ivInsets.load( pfirst );
			}
		}
	}
}

/**
 * stores as tree node
 */
MTreeNode* MLayoutRow::save()
{
	MTreeNode* ptBack = MLayoutNode::save();
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "align", align2str( ivAlign ) );
	unsigned int childCount = getChildCount();
	MTreeNode* ptChilds = new MTreeNode( "children" );
	for( unsigned int i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );
		ISerializeable* ptSer = (ISerializeable*) ptChild->getInterface( "ISerializeable" );
		if( ptSer )
			ptChilds->addChild( ptSer->save() );
	}
	ptBack->addChild( ptChilds );
	return ptBack;
}

MObject* MLayoutRow::createInstance()
{
	return new MLayoutRow();
}

String MLayoutRow::align2str( MLayoutRow::Align align )
{
	switch( align )
	{
		case MLayoutRow::TOP:
			return "TOP";
		case MLayoutRow::CENTER:
			return "CENTER";
		case MLayoutRow::BOTTOM:
			return "BOTTOM";
		default:
			return "unknown";
	}
}

MLayoutRow::Align MLayoutRow::str2align( String str )
{
	if( str == "TOP" )
		return MLayoutRow::TOP;
	else if( str == "CENTER" )
		return MLayoutRow::CENTER;
	else if( str == "BOTTOM" )
		return MLayoutRow::BOTTOM;
	else
		return MLayoutRow::CENTER;
}