#ifndef __MComb
#define __MComb

#include "MTransformer.h"

class MComb :
	public MTransformer
{
private:

	FP** ivPtBuffer;
	unsigned int ivChannelCount;
	unsigned int ivBufLen;
	unsigned int ivStep;
	unsigned int ivStep2;

public:

	static MRtti gvRtti;

	static MObject* createInstance()
	{
		return new MComb();
	}

	MComb() :
		ivStep( 0 ),
		ivStep2( 0 ),
		ivBufLen( 0 ),
		ivPtBuffer( 0 )
	{
	}

	virtual ~MComb()
	{
		for( unsigned int i=0;i<ivChannelCount;i++ )
			delete ivPtBuffer[ i ];
		delete ivPtBuffer;
	}

	virtual bool create( unsigned int channelCount )
	{
		ivStep = 0;
		ivBufLen = 256;
		ivStep2 = ivBufLen / 2;

		ivChannelCount = channelCount;
		ivPtBuffer = new FP*[ivChannelCount];
		for( unsigned int i=0;i<ivChannelCount;i++ )
		{
			ivPtBuffer[ i ] = new FP[ ivBufLen ];
			memset( ivPtBuffer[ i ], 0, ivBufLen*sizeof(FP) );
		}

		return true;
	}

	virtual void goNext(
		MSoundBuffer *buffer,
		unsigned int startFrom,
		unsigned int stopAt )
	{
		ASSERT( buffer->getChannelCount() == ivChannelCount );

		unsigned int samplesToRender = stopAt-startFrom;

		for( unsigned int i=0;i<ivChannelCount;i++ )
		{
			unsigned int s1=ivStep, s2=ivStep2;
			FP* pt = buffer->getData( i ) + startFrom;
			FP* ptEnd = pt + stopAt;
			for( ;pt<ptEnd;pt++ )
			{
				ASSERT( s1 < ivBufLen );
				ASSERT( s2 < ivBufLen );
				FP value = *pt;
				ivPtBuffer[i][s1] += value;
				ivPtBuffer[i][s2] *= 0.5;
				if( ++s1 == ivBufLen )
					s1 = 0;
				if( ++s2 == ivBufLen )
					s2 = 0;
				*pt = ivPtBuffer[i][s1];
			}

			if( i == ivChannelCount - 1 )
			{
				ivStep = s1;
				ivStep2 = s2;
			}
		}
	}

	virtual String getName(){return "MComb";}
	virtual String getShortName(){return getName();}
	virtual unsigned int getControlCount(){return 0;};
	virtual IControl* getControl( unsigned int index ){return 0;};
};

#endif