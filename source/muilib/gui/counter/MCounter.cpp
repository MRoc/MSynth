/**
 * (C)2000-2003 MRoc
 */

#include "MCounter.h"

/** runtime type info */
INIT_RTTI_CREATE( MCounter, "MCounter" );

String MCounter::gvDefaultImages[] = {
							"resource/img/numbers/num0.bmp",
							"resource/img/numbers/num1.bmp",
							"resource/img/numbers/num2.bmp",
							"resource/img/numbers/num3.bmp",
							"resource/img/numbers/num4.bmp",
							"resource/img/numbers/num5.bmp",
							"resource/img/numbers/num6.bmp",
							"resource/img/numbers/num7.bmp",
							"resource/img/numbers/num8.bmp",
							"resource/img/numbers/num9.bmp" };

MCounter::MCounter() :
	MWnd(),
	ivRange( 3 ),
	ivValue( 0 )
{
	for(unsigned int i=0;i<10;i++)
		ivBitmaps[i].loadImage( gvDefaultImages[i] );
	ivRect.setSize( getPreferredSize() );
}

MCounter::MCounter( unsigned int range ) :
	MWnd(),
	ivRange( range ),
	ivValue( 0 )
{
	for(unsigned int i=0;i<10;i++)
		ivBitmaps[i].loadImage( gvDefaultImages[i] );
	ivRect.setSize( getPreferredSize() );
}

MCounter::MCounter( unsigned int range, unsigned int *ar_bitmapIDs ) :
	MWnd(),
	ivRange( range ),
	ivValue( 0 )
{
	for(unsigned int i=0;i<10;i++)
		ivBitmaps[i].loadImage( ar_bitmapIDs[i] );
	ivRect.setSize( getPreferredSize() );
}

MCounter::MCounter( unsigned int range, String *arBitmapResources ) :
	MWnd(),
	ivRange( range ),
	ivValue( 0 )
{
	for(unsigned int i=0;i<10;i++)
		ivBitmaps[i].loadImage( arBitmapResources[i] );
	ivRect.setSize( getPreferredSize() );
}

MCounter::~MCounter()
{
}

void MCounter::paint( IGraphics* ptGraphics, const MRect &rect )
{
	ptGraphics->fillRect( rect, ivBkColor );
	MSize prefSize = getPreferredSize();
	int xOff = (rect.getWidth() - prefSize.getWidth()) / 2;
	int yOff = (rect.getHeight() - prefSize.getHeight()) / 2;
	unsigned int temp = ivValue;
	for(unsigned int i=0;i<ivRange;i++)
	{
		MImage* ptImg = &( ivBitmaps[temp%10] );
		ptGraphics->drawImage(
			ptImg,
			xOff + (ivRange-1-i)*ptImg->getWidth(),
			yOff );
		temp/=10;
	}
}

void MCounter::setRange( unsigned int range )
{
	ivRange = range;
}

unsigned int MCounter::getRange() const
{
	return ivRange;
}

void MCounter::setValue( unsigned int value )
{
	if(value != ivValue)
	{
		ivValue	= value;
		repaint();
	}
}

unsigned int MCounter::getValue() const
{
	return ivValue;
}

MSize MCounter::getPreferredSize()
{
	return MSize( ivRange * ivBitmaps[0].getWidth(), ivBitmaps[0].getHeight() );
}

/** store ui state into treenode */
MTreeNode* MCounter::save()
{
	MTreeNode* pBack = MWnd::save();
	pBack->setAttribute( "range", MInt::toString( getRange() ) );
	return pBack;
}

/** load ui state from treenode */
void MCounter::load( MTreeNode* pNode )
{
	MWnd::load( pNode );
	setRange( MInt::parse( pNode->getAttribute( "range", 3 ) ) );
}