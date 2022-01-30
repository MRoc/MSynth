#include "MFIR.h"
#include <framework/primitive/MArrayUtils.h>

#define FIR_LENGTH 9

/**
 * runtime type info
 */
MRtti MFIR::gvRtti = MRtti( "MFIR", MFIR::createInstance );

/**
 * constructor
 */
MFIR::MFIR() :
	ivPtCutoff( 0 )
{
	calculateCoeffs( 0.05f, FIR_LENGTH );
	ivPtCutoff = new MFloatControl( "Cutoff", 0.0, 0.5, 0.5, true );
	ivPtCutoff->addControlListener( this );
}

/**
 * destructor
 */
MFIR::~MFIR()
{
	ivPtCutoff->removeControlListener( this );
	SAFE_DELETE( ivPtCutoff );
}

/**
 * query interface
 */
void* MFIR::getInterface( const String &className ) const
{
	if( className == "MFIR" )
		return (void*) ((MFIR*)this);
	else
		return MTransformer::getInterface( className );
}

/**
 * returns the runtime type info
 */
IRtti* MFIR::getRtti() const
{
	return &gvRtti;
}

/**
 * calculates new filter coeffs
 */
void MFIR::calculateCoeffs(
	float normalizedCutoffFrequency,
	unsigned int coeffsLength )
{
	ASSERT( coeffsLength > 2 );
	ASSERT( coeffsLength % 2 == 1 );

	ivCoeffs.setLength( coeffsLength );

	FP* pCoeffs = ivCoeffs.getData();
	int offset = coeffsLength / 2;

	for( int i=0;i<offset;i++ )
	{
		int rIndex = i-offset;
		pCoeffs[ i ] = (float) sin( 2 * PI * normalizedCutoffFrequency * rIndex ) / rIndex;
	}
	pCoeffs[ offset ] = 1;
	for( int i=0;i<offset;i++ )
	{
		pCoeffs[ offset + 1 + i ] = pCoeffs[ offset - i - 1 ];
	}

	MFloatArrayUtils::div( &ivCoeffs, MFloatArrayUtils::sum( &ivCoeffs ) );

	ivBuffer.setLength( coeffsLength );
	ivBuffer.setReadOffset( offset - 1 );

	TRACE( "MFIR::calculateCoeffs...(w=%i, r=%i, offset=%i)\n", ivBuffer.getWriteIndex(), ivBuffer.getReadIndex(), (offset-1) );
	for( unsigned int i=0;i<ivCoeffs.getDataLength();i++ )
		TRACE( "coeff[%i] = %f\n", i, ivCoeffs[ i ] );
}

/**
 * creates an instance of this class
 */
MObject* MFIR::createInstance()
{
	return new MFIR();
}

/** 
 * returns the number of embedded controls
 */
unsigned int MFIR::getControlCount()
{
	return 1;
}

/**
 * returns a embedded control by index
 */
IControl* MFIR::getControl( unsigned int index )
{
	return ivPtCutoff;
}

/**
 * does the filtering
 */
void MFIR::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	FP* pCoeffsSrc = ivCoeffs.getData();
	FP* pCoeffs = pCoeffsSrc;

	float out = 0.0f;
	int i=0;
	int length = ivCoeffs.getDataLength();
	int length2 = length / 2;

	FP* pBegin = buffer->getData( 0 ) + startFrom;
	FP* pEnd = pBegin + (stopAt - startFrom);
	for( FP* pt = pBegin; pt<pEnd; pt++ )
	{
		// enqueue sample
		ivBuffer.write( (*pt) );

		// outputside convolution 1 sample
		out = 0.0f;
		pCoeffs = pCoeffsSrc;
		for( i=0;i<length;i++, pCoeffs++ )
			out += ivBuffer.read( i - length2 ) * (*pCoeffs);

		// write back
		(*pt) = out;
	}
}

/**
 * returns the name
 */
String MFIR::getName()
{
	return "FIR";
}

/** 
 * returns the short name
 */
String MFIR::getShortName()
{
	return getName();
}

/**
 * invoked when value changed
 */
void MFIR::valueChanged( MControlListenerEvent *anEvent )
{
	calculateCoeffs( ivPtCutoff->getValue(), FIR_LENGTH );
}