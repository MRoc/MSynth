/**
 * (C) 2001-2003 MRoc
 *
 * The PianoRenderer is designed to render one octave of a piano
 */

#ifndef __MNotePianoRenderer
#define __MNotePianoRenderer

#include <framework/MUtil.h>
#include <framework/MObject.h>
#include <framework/primitive/MPointF.h>
#include <gui/IGraphics.h>

class Poly
{
public:
	MPointF* npoints;
	MPoint* points;

	unsigned int num;

	Poly( unsigned int _pointCount ) :
		npoints( 0 ),
		points( 0 )
	{
		num = _pointCount;
		npoints = new MPointF[ num ];
		points = new MPoint[ num ];
	}

	virtual ~Poly()
	{
		delete[]( npoints );
		delete[]( points );
	}

	void calcPoints( MSize keyBoardSize, unsigned int yOffset )
	{
		MSize s2 = MSize( keyBoardSize.getWidth() - 1, keyBoardSize.getHeight() );
		for( unsigned int i=0;i<num;i++ )
		{
			points[ i ].setX( int( s2.getWidth() * npoints[ i ].getX() ) );
			points[ i ].setY( int( yOffset + npoints[ i ].getY() * s2.getHeight() ) );
		}
	}
};

class MNotePianoRenderer
{
protected:

	enum KeyType
	{
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D
	};

	static int		KEY_COUNT;
	static bool		IS_BLACK_KEY[12];
	static int		KEY_TYPES[12];

	static float	NORMALIZED_DIVIDER_X;

	Poly*			POLIES[12];

public:

	MNotePianoRenderer();
	~MNotePianoRenderer();

	MSize			GetPianoSize();

	void			paint( MSize keyBoardSize, IGraphics *dc, int x, int y, int octave, int pressedKey = -1 );

private:

	void initialize();
	void uninitialize();

	static Poly*	GetNormPolyTypeA( unsigned int index );
	static Poly*	GetNormPolyTypeB( unsigned int index );
	static Poly*	GetNormPolyTypeC( unsigned int index );
	static Poly*	GetNormPolyTypeD( unsigned int index );
};

#endif