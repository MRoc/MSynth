/*

  (C) 2001 MRoc hifiShock

  The PianoRenderer is designed to render one octave of a piano

*/

#include "MNotePianoRenderer.h"
#include "../../../../resource.h"

float MNotePianoRenderer::NORMALIZED_DIVIDER_X = 0.66f;

int MNotePianoRenderer::KEY_COUNT = 12;

/*char *MNotePianoRenderer::OCTAVE_CHAR[12] =
{ "H", "A#", "A", "G#", "G", "F#", "#", "E", "D#", "D", "C#", "C" };*/

bool MNotePianoRenderer::IS_BLACK_KEY[12] =
{ false, true, false, true, false, true, false, false, true, false, true, false };

int	MNotePianoRenderer::KEY_TYPES[12] = 
{ KEY_A, KEY_B, KEY_C, KEY_B, KEY_C, KEY_B, KEY_D, KEY_A, KEY_B, KEY_C, KEY_B, KEY_D };

MNotePianoRenderer::MNotePianoRenderer()
{
	initialize();
};

MNotePianoRenderer::~MNotePianoRenderer()
{
	uninitialize();
};

MSize MNotePianoRenderer::GetPianoSize()
{
	return MSize( 80, 144 );
};

void MNotePianoRenderer::paint( MSize keyBoardSize, IGraphics *dc, int x, int y, int octave, int pressedKey )
{
	int div = 144 / 12;
	int xL = 0;
	int yL = 0;

	dc->fillRect( MRect( x, y, keyBoardSize.getWidth(), keyBoardSize.getHeight() ), MColorMap::create( 255, 255, 255 ) );

	for( int i=0;i<12;i++ )
	{
		POLIES[ i ]->calcPoints( keyBoardSize, y );

		MColor color;
		if( pressedKey == i )
			color = MColorMap::create( 255, 0, 0 );
		else if( IS_BLACK_KEY[ i ] )
			color = MColorMap::FG_COLOR3;
		else
			color = MColorMap::FG_COLOR2;

		dc->poly( POLIES[ i ]->points, POLIES[ i ]->num, color, MColorMap::FG_COLOR3 );
	}

	String octaveStr( "" );
	octaveStr.Format( "C%i", octave );
	dc->drawText( octaveStr, MRect( x + keyBoardSize.getWidth() - 15, y + 129, 20, 12 ), MColorMap::FG_COLOR3 );
};

Poly* MNotePianoRenderer::GetNormPolyTypeA( unsigned int index )
{
	/*

  normalized coordinates of one keyboardoctave

|-------------------> X
|
|	0-------------1
|   |     white   |
|   5-------4     |
|	        3-----2
|
Y
	*/

	float xDiv = NORMALIZED_DIVIDER_X;

	float yDiff = 1.0f / 12.0f;
	float y = index * yDiff;

	Poly* pt = new Poly( 6 );

	pt->npoints[ 0 ].setX( 0.0f );
	pt->npoints[ 0 ].setY( y );

	pt->npoints[ 1 ].setX( 1.0f );
	pt->npoints[ 1 ].setY( y );

	pt->npoints[ 2 ].setX( 1.0f );
	pt->npoints[ 2 ].setY( y + 1.5f * yDiff );

	pt->npoints[ 3 ].setX( xDiv );
	pt->npoints[ 3 ].setY( y + 1.5f * yDiff );

	pt->npoints[ 4 ].setX( xDiv );
	pt->npoints[ 4 ].setY( y + yDiff );

	pt->npoints[ 5 ].setX( 0.0f );
	pt->npoints[ 5 ].setY( y + yDiff );

	return pt;
};

Poly* MNotePianoRenderer::GetNormPolyTypeB( unsigned int index )
{
	/*

  normalized coordinates of one keyboardoctave

|-------------------> X
|
|	0-------------1
|   |     black   |
|   3-------------2
|
Y
	*/

	float xDiv = NORMALIZED_DIVIDER_X;

	float yDiff = 1.0f / 12.0f;
	float y = index * yDiff;

	Poly* pt = new Poly( 4 );

	pt->npoints[ 0 ].setX( 0.0f );
	pt->npoints[ 0 ].setY( y );

	pt->npoints[ 1 ].setX( xDiv );
	pt->npoints[ 1 ].setY( y );

	pt->npoints[ 2 ].setX( xDiv );
	pt->npoints[ 2 ].setY( y + yDiff );

	pt->npoints[ 3 ].setX( 0.0f );
	pt->npoints[ 3 ].setY( y + yDiff );

	return pt;
};

Poly* MNotePianoRenderer::GetNormPolyTypeC( unsigned int index )
{
	/*

  normalized coordinates of one keyboardoctave

|-----------------------> X
|              2------3
|	0----------1      |
|   |     white       |
|   7----------6      |
|              5------4
Y
	*/

	float xDiv = NORMALIZED_DIVIDER_X;

	float yDiff = 1.0f / 12.0f;
	float y = index * yDiff;

	Poly* pt = new Poly( 8 );

	pt->npoints[ 0 ].setX( 0.0f );
	pt->npoints[ 0 ].setY( y );

	pt->npoints[ 1 ].setX( xDiv );
	pt->npoints[ 1 ].setY( y );

	pt->npoints[ 2 ].setX( xDiv );
	pt->npoints[ 2 ].setY( y - 0.5f * yDiff );

	pt->npoints[ 3 ].setX( 1.0f );
	pt->npoints[ 3 ].setY( y - 0.5f * yDiff );

	pt->npoints[ 4 ].setX( 1.0f );
	pt->npoints[ 4 ].setY( y + 1.5f * yDiff );

	pt->npoints[ 5 ].setX( xDiv );
	pt->npoints[ 5 ].setY( y + 1.5f * yDiff );

	pt->npoints[ 6 ].setX( xDiv );
	pt->npoints[ 6 ].setY( y + yDiff );

	pt->npoints[ 7 ].setX( 0.0f );
	pt->npoints[ 7 ].setY( y + yDiff );

	return pt;
};

Poly* MNotePianoRenderer::GetNormPolyTypeD( unsigned int index )
{
	/*

  normalized coordinates of one keyboardoctave

|-----------------------> X
|              2------3
|	0----------1      |
|   |     white       |
|   5-----------------4
Y
	*/

	float xDiv = NORMALIZED_DIVIDER_X;

	float yDiff = 1.0f / 12.0f;
	float y = index * yDiff;

	Poly* pt = new Poly( 6 );

	pt->npoints[ 0 ].setX( 0.0f );
	pt->npoints[ 0 ].setY( y );

	pt->npoints[ 1 ].setX( xDiv );
	pt->npoints[ 1 ].setY( y );

	pt->npoints[ 2 ].setX( xDiv );
	pt->npoints[ 2 ].setY( y - 0.5f * yDiff );

	pt->npoints[ 3 ].setX( 1.0f );
	pt->npoints[ 3 ].setY( y - 0.5f * yDiff );

	pt->npoints[ 4 ].setX( 1.0f );
	pt->npoints[ 4 ].setY( y + yDiff );

	pt->npoints[ 5 ].setX( 0.0f );
	pt->npoints[ 5 ].setY( y + yDiff );

	return pt;
};

void MNotePianoRenderer::initialize()
{
	for( unsigned int i=0;i<12;i++ )
	{
		switch( KEY_TYPES[ i ] )
		{
			case KEY_A: POLIES[ i ] = GetNormPolyTypeA( i ); break;
			case KEY_B: POLIES[ i ] = GetNormPolyTypeB( i ); break;
			case KEY_C: POLIES[ i ] = GetNormPolyTypeC( i ); break;
			case KEY_D: POLIES[ i ] = GetNormPolyTypeD( i ); break;
		}
	}
}

void MNotePianoRenderer::uninitialize()
{
	for( unsigned int i=0;i<12;i++ )
	{
		ASSERT( POLIES[i] );
		delete POLIES[ i ];
	}
}