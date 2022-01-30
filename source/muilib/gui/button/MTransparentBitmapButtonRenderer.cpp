#include "MTransparentBitmapButtonRenderer.h"
#include "MButton.h"
#include "../MGuiUtils.h"

/** runtime type info */
MRtti MTransparentBitmapButtonRenderer::gvRtti = MRtti( "MTransparentBitmapButtonRenderer", MTransparentBitmapButtonRenderer::createInstance );

/** default constructor */
MTransparentBitmapButtonRenderer::MTransparentBitmapButtonRenderer() :
	ivAllocated( false )
{
	ivPtBitmaps[0] = 0;
	ivPtBitmaps[1] = 0;
	ivTransparenceColors[0] = 0;
	ivTransparenceColors[1] = 0;
}

/** constructor 1 */
MTransparentBitmapButtonRenderer::MTransparentBitmapButtonRenderer(
	MImage* ptBitmap1,
	MImage* ptBitmap2,
	MColor transparenceColor1,
	MColor transparenceColor2 )
	:
	ivAllocated( false )
{
	ivPtBitmaps[0] = ptBitmap1;
	ivPtBitmaps[1] = ptBitmap2;
	ivTransparenceColors[0] = transparenceColor1;
	ivTransparenceColors[1] = transparenceColor2;

	ASSERT( ivPtBitmaps[ 0 ] );
	ASSERT( ivPtBitmaps[ 1 ] );
}

/** constructor 2 */
MTransparentBitmapButtonRenderer::MTransparentBitmapButtonRenderer(
	unsigned int id1,
	unsigned int id2,
	MColor transparenceColor1,
	MColor transparenceColor2 )
	:
	ivAllocated( true )
{
	ivPtBitmaps[0] = 0;
	ivPtBitmaps[1] = 0;
	ivTransparenceColors[0] = transparenceColor1;
	ivTransparenceColors[1] = transparenceColor2;

	for( int i=0;i<2;i++ )
		ivPtBitmaps[ i ] = new MImage();

	ivPtBitmaps[ 0 ]->loadImage( id1 );
	ivPtBitmaps[ 1 ]->loadImage( id2 );

	ASSERT( ivPtBitmaps[ 0 ] );
	ASSERT( ivPtBitmaps[ 1 ] );
}

/** destructor */
MTransparentBitmapButtonRenderer::~MTransparentBitmapButtonRenderer()
{
	if( ivAllocated )
		for( int i=0;i<2;i++ )
		{
			if( ivPtBitmaps[ i ] )
			{
				delete ivPtBitmaps[ i ];
				ivPtBitmaps[ i ] = 0;
			}
		}
}

/** does the paining for the given compoent */
void MTransparentBitmapButtonRenderer::paint( MWnd* pWnd, IGraphics* pG, const MRect& rect )
{
	MButton* pButton = (MButton*) pWnd->getInterface( "MButton" );
	if( pButton )
	{
		bool pressed = pButton->isSunken();
		MColor bkColor = pressed ?
			(pButton->getIsHovering() ?
				MGuiUtils::multiplyColor( pButton->getBkColor(), 0.95f ) :
				MGuiUtils::multiplyColor( pButton->getBkColor(), 0.9f )) :
					(pButton->getIsHovering() ?
						MGuiUtils::multiplyColor( pButton->getBkColor(), 1.1f ) :
						pButton->getBkColor() );

		if( pressed )
			MGuiUtils::paintSunkenBorder( pG, rect, bkColor );
		else
			MGuiUtils::paintRaisedBorder( pG, rect, bkColor );

		int index = pressed ? 0 : 1;
		int x = rect.getX() + (rect.getWidth() - (int)ivPtBitmaps[index]->getWidth()) / 2;
		int y = rect.getY() + (rect.getHeight() - (int)ivPtBitmaps[index]->getHeight()) / 2;

		if( pressed )
		{
			x++;
			y++;
		}
		pG->drawImageTransparent(
			ivPtBitmaps[ index ],
			x,
			y,
			ivTransparenceColors[ index ] );
	}
}

/** query for superclass */
void* MTransparentBitmapButtonRenderer::getInterface( const String &className ) const
{
	if( className == "MTransparentBitmapButtonRenderer" )
		return (void*) ((MTransparentBitmapButtonRenderer*)this );
	else if( className == "IWndRenderer" )
		return (void*) ((IWndRenderer*)this );
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this );
	else
		return MObject::getInterface( className );
}

/** returns the runtime type info */
IRtti* MTransparentBitmapButtonRenderer::getRtti() const{ return &gvRtti; }

/** loads from the given tree node */
void MTransparentBitmapButtonRenderer::load( MTreeNode* ptNode )
{
	ivPtBitmaps[0] = new MImage( ptNode->getAttribute( "imgup" ) );
	ivPtBitmaps[1] = new MImage( ptNode->getAttribute( "imgdown" ) );
	ivTransparenceColors[0] = MUtil::parseHexString( ptNode->getAttribute( "tcolup", "0" ) );
	ivTransparenceColors[1] = MUtil::parseHexString( ptNode->getAttribute( "tcoldown", "0" ) );
}

/** stores as tree node */
MTreeNode* MTransparentBitmapButtonRenderer::save()
{
	MTreeNode* pBack = new MTreeNode( ELEM_OBJECT );
	pBack->setAttribute( ATTR_CLASS, getRtti()->getClassName() );
	
	ASSERT( false );

	return pBack;
}

/** creates an instance of this class */
MObject* MTransparentBitmapButtonRenderer::createInstance()
{
	return new MTransparentBitmapButtonRenderer();
}