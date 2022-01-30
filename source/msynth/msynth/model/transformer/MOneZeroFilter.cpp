#include "MOneZeroFilter.h"

/**
 * the runtime type info
 */
MRtti MOneZeroFilter::gvRtti = MRtti( "MOneZeroFilter", MOneZeroFilter::createInstance );

/**
 * constructor
 */
MOneZeroFilter::MOneZeroFilter() :
	MTransformer(),
	ivPtAControl( 0 ),
	ivArXZ( 0 )
{
	this->ivPtAControl =
		new MFloatControl(
			"A",
			-1.0f, 
			+1.0f,
			0.9f,
			false,
			MFloatControl::CURVE_LINEAR,
			"",
			1.0f );
}

/** 
 * destructor
 */
MOneZeroFilter::~MOneZeroFilter()
{
	SAFE_DELETE( ivPtAControl );
	SAFE_DELETE( ivArXZ );
}

/**
 * creates all buffers required by the effect
 */
bool MOneZeroFilter::create( unsigned int channelCount )
{
	ivChannelCount = channelCount;
	this->ivArXZ = new FP[ ivChannelCount ];
	processEvent( &MEvent( MEvent::RESET ) );
	return true;
}

/**
 * process event
 */
void MOneZeroFilter::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
		for(unsigned int i=0;i<ivChannelCount;i++ )
			ivArXZ[ i ] = 0.0f;
}

/**
 * renders the given amount of data
 */
void MOneZeroFilter::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	float a = this->ivPtAControl->getValue();
	float g = 1.0f / ( 1.0f + (( a < 0.0f ? (a * -1.0f) : a )) );
	TRACE( "A=%f G=%f\n", a, g );

	for( unsigned int i=0;i<buffer->getChannelCount();i++ )
	{
		FP *ptStop = buffer->getData(i) + stopAt;
		for( FP *pt=buffer->getData(i)+startFrom; pt<ptStop; pt++ )
		{
			/**
			 *
			 * y(n) = g * ( x(n) + a*x(n-1) )
			 *
			 * g = 1 / (1+|a|)
			 *
			 */
			float in = (*pt);
            (*pt) = g * ( in + a * ivArXZ[i] );
			ivArXZ[i] = in;
		}
	}
}

/**
 * returns the number of controls
 */
unsigned int MOneZeroFilter::getControlCount()
{
	return 1;
}

/** 
 * returns the control at the specified index
 */
IControl* MOneZeroFilter::getControl( unsigned int index )
{
	return this->ivPtAControl;
}

/**
 * returns the name of the transformer
 */
String MOneZeroFilter::getName()
{
	return "OneZeroFilter";
}

/**
 * return the short name of the filter
 */
String MOneZeroFilter::getShortName()
{
	return "OneZeroFilter";
}

/** 
 * returns the runtime type info
 */
IRtti* MOneZeroFilter::getRtti() const
{
	return &gvRtti;
}

/**
 * creates a instance of this class
 */
MObject* MOneZeroFilter::createInstance()
{
	return new MOneZeroFilter();
}