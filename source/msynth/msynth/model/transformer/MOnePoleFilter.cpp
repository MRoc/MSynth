#include "MOnePoleFilter.h"

/**
 * the runtime type info
 */
MRtti MOnePoleFilter::gvRtti = MRtti( "MOnePoleFilter", MOnePoleFilter::createInstance );

/**
 * constructor
 */
MOnePoleFilter::MOnePoleFilter() :
	MTransformer(),
	ivPtRControl( 0 ),
	ivArRZ( 0 )
{
	this->ivPtRControl =
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
MOnePoleFilter::~MOnePoleFilter()
{
	SAFE_DELETE( ivPtRControl );
	SAFE_DELETE( ivArRZ );
}

/**
 * creates all buffers required by the effect
 */
bool MOnePoleFilter::create( unsigned int channelCount )
{
	ivChannelCount = channelCount;
	this->ivArRZ = new FP[ ivChannelCount ];
	processEvent( &MEvent( MEvent::RESET ) );
	return true;
}

/**
 * process event
 */
void MOnePoleFilter::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET )
		for(unsigned int i=0;i<ivChannelCount;i++ )
			ivArRZ[ i ] = 0.0f;
}

/**
 * renders the given amount of data
 */
void MOnePoleFilter::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	float r = this->ivPtRControl->getValue();
	float g = 1.0f - ( r < 0.0f ? (r * -1.0f) : r );
	TRACE( "R=%f G=%f\n", r, g );

	for( unsigned int i=0;i<buffer->getChannelCount();i++ )
	{
		FP *ptStop = buffer->getData(i) + stopAt;
		for( FP *pt=buffer->getData(i)+startFrom; pt<ptStop; pt++ )
		{
			/**
			 * implementation of a one pole filter
			 *
			 * y(n) = g * x(n) + r*y(n-1)
			 *
			 * g = 1 / (1+|a|)
			 *
			 */
			float in = (*pt);

            float out = g * in + r * ivArRZ[i];

			ivArRZ[i] = out;
			(*pt) = out;
		}
	}
}

/**
 * returns the number of controls
 */
unsigned int MOnePoleFilter::getControlCount()
{
	return 1;
}

/** 
 * returns the control at the specified index
 */
IControl* MOnePoleFilter::getControl( unsigned int index )
{
	return this->ivPtRControl;
}

/**
 * returns the name of the transformer
 */
String MOnePoleFilter::getName()
{
	return "OnePoleFilter";
}

/**
 * return the short name of the filter
 */
String MOnePoleFilter::getShortName()
{
	return "OnePoleFilter";
}

/** 
 * returns the runtime type info
 */
IRtti* MOnePoleFilter::getRtti() const
{
	return &gvRtti;
}

/**
 * creates a instance of this class
 */
MObject* MOnePoleFilter::createInstance()
{
	return new MOnePoleFilter();
}