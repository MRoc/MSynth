#include "MLayoutCol.h"

MRtti MLayoutCol::gvRtti = MRtti( "MLayoutCol", MLayoutCol::createInstance );

MLayoutCol::MLayoutCol()
{
}

MLayoutCol::~MLayoutCol()
{
}

IRtti* MLayoutCol::getRtti() const
{
	return &gvRtti;
}

void* MLayoutCol::getInterface( const String &className ) const
{
	if( className == "MLayoutCol" )
		return (void*) ((MLayoutCol*)this);
	else
		return MLayoutNode::getInterface( className );
}

MSize MLayoutCol::getSize( MWnd* ptWnd, MSize maxSize )
{
	int w = 0;
	int h = 0;
	unsigned int childCount = this->getChildCount();
	for( unsigned int i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = this->getChild( i );
		MSize cSize = ptChild->getSize( ptWnd, maxSize );
		if( ptChild->getSizing() == MLayoutLeaf::FIXED )
		{
			if( cSize.getWidth() > w )
				w = cSize.getWidth();
			h += cSize.getHeight();
		}
		else if( ptChild->getSizing() == MLayoutLeaf::VERTICAL )
		{
			if( cSize.getWidth() > w )
				w = cSize.getWidth();
		}
		else if( ptChild->getSizing() == MLayoutLeaf::BOTH )
		{
			if( cSize.getWidth() > w )
				w = cSize.getWidth();
		}
	}
	return MSize( w + ivInsets.left() + ivInsets.right(), h + ivInsets.top() + ivInsets.bottom() );
}

void MLayoutCol::doLayout( MWnd* ptWnd, MRect rect )
{
	unsigned int childCount = this->getChildCount();
	int x = rect.getX() + ivInsets.left();
	int y = rect.getY() + ivInsets.top();

	int rest = 0;
	unsigned int i;
	for( i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );
		if( ptChild->getSizing() == FIXED || 
			ptChild->getSizing() == HORIZONTAL )
			rest += (int) ptChild->getSize( ptWnd, rect.getSize() ).getHeight() + 2;
	}

	for( i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = getChild( i );
		ASSERT( ptChild != this );

		MSize childSize = ptChild->getSize( ptWnd, rect.getSize() );
		int width = (int) childSize.getWidth();
		int height = (int) childSize.getHeight();
		int rectWidth = (int) rect.getWidth();
		int rectHeight = (int) rect.getHeight();

		int xx = x;
		int w = rectWidth - (ivInsets.left() + ivInsets.right());
		if( ivAlign == RIGHT )
			xx = x + (w-width);
		else if( ivAlign == CENTER )
			xx = x + (w-width)/2;

		if( ptChild->getSizing() == MLayoutLeaf::FIXED )
		{
			ptChild->doLayout(
				ptWnd,
				MRect(
					xx,
					y,
					width,
					height ) );

			y += height + 2;
		}
		else if( ptChild->getSizing() == MLayoutLeaf::HORIZONTAL )
		{
			int w = rectWidth - (ivInsets.left()+ivInsets.right());

			if( w < 0 )
				w = 0;

			ptChild->doLayout(
				ptWnd,
				MRect(
					x, 
					y,
					w,
					height ) );

			y += height + 2;
		}
		else if( ptChild->getSizing() == MLayoutLeaf::BOTH )
		{
			int w = rectWidth - (ivInsets.left()+ivInsets.right());
			int h = rectHeight - (ivInsets.top()+ivInsets.bottom()) - rest;

			if( w < 0 )
				w = 0;
			if( h < 0 )
				h = 0;

			if( h < (int)childSize.getHeight() )
				h = childSize.getHeight();

			ptChild->doLayout(
				ptWnd,
				MRect(
					x, 
					y,
					w,
					h ) );

			y += h + 2;
		}
	}
}

/**
 * loads from the given tree node
 */
void MLayoutCol::load( MTreeNode* ptNode )
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
MTreeNode* MLayoutCol::save()
{
	MTreeNode* ptBack = MLayoutNode::save();
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	ptBack->setAttribute( "align", align2str( ivAlign ) );
	unsigned int childCount = this->getChildCount();
	MTreeNode* ptChilds = new MTreeNode( "children" );
	for( unsigned int i=0;i<childCount;i++ )
	{
		MLayoutLeaf* ptChild = this->getChild( i );
		ISerializeable* ptSer = (ISerializeable*) ptChild->getInterface( "ISerializeable" );
		if( ptSer )
			ptChilds->addChild( ptSer->save() );
	}
	ptBack->addChild( ptChilds );
	return ptBack;
}

MObject* MLayoutCol::createInstance()
{
	return new MLayoutCol();
}

String MLayoutCol::align2str( MLayoutCol::Align align )
{
	switch( align )
	{
		case MLayoutCol::LEFT:
			return "LEFT";
		case MLayoutCol::CENTER:
			return "CENTER";
		case MLayoutCol::RIGHT:
			return "RIGHT";
		default:
			return "unknown";
	}
}

MLayoutCol::Align MLayoutCol::str2align( String str )
{
	if( str == "LEFT" )
		return MLayoutCol::LEFT;
	else if( str == "CENTER" )
		return MLayoutCol::CENTER;
	else if( str == "RIGHT" )
		return MLayoutCol::RIGHT;
	else
		return MLayoutCol::CENTER;
}