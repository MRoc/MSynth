#include "MEnvelopeFollower.h"

/**
 * constructor
 */
MEnvelopeFollower::MEnvelopeFollower() :
	ivBUp( 0.001f ),
	ivBDown( 0.99999f ),
	ivPeak( 0.0f )
{
}

/**
 * destructor
 */
MEnvelopeFollower::~MEnvelopeFollower()
{
}

/** 
 * does a envelope follow
 */
void MEnvelopeFollower::goNext( MSoundBuffer* pBuffer, unsigned int start, unsigned int stop )
{
	FP* pSrc = pBuffer->getData( 0 ) + start;
	FP* pStop = pSrc + (stop-start);

	for( ;pSrc<pStop;pSrc++ )
	{
		// fetch input...
		FP fCur = (*pSrc);

		// abs...
		if( fCur < 0.0f )
			fCur *= -1;

		// calc e...
		FP b = fCur > ivPeak ? ivBUp : ivBDown;
		FP g = 1-b;

		// iteration...
		ivPeak = g * fCur + ivPeak * b;
	}

	//fireEnvelopeChanged( ivPeak );
}

/**
 * when invoked, the envelope follower fires
 * a change notification.
 */
void MEnvelopeFollower::fireUpdate()
{
	fireEnvelopeChanged( ivPeak );
}

/** 
 * adds a listener to the enveloper
 */
void MEnvelopeFollower::addListener( IEnvelopeFollowerListener* pListener )
{
	ivListeners.addListener( pListener );
}

/** 
 * removes a listener from the enveloper
 */
void MEnvelopeFollower::removeListener( IEnvelopeFollowerListener* pListener )
{
	ivListeners.removeListener( pListener );
}

/**
 * fires a envelope changed event
 */
void MEnvelopeFollower::fireEnvelopeChanged( FP value )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IEnvelopeFollowerListener*)ivListeners.getListener( i ))->onEnvelopeChanged( value );

	/*{
		char buffer[ 256 ];
		sprintf( buffer, "          |\n" );
		int tmp = (int)(value*10);
		if( tmp > 10 )
			tmp = 10;
		for( int i=0;i<tmp;i++	)
			buffer[ i ] = '*';
		TRACE( buffer );
	}*/
}