/*(C)2000 MRoc hifiShock*/

#ifndef __MReverb
#define __MReverb

#include "MTransformer.h"

#define MAXCOMBFILTER 4
#define MAXALLPASSFILTER 2

class MReverbBuffer
{
public:

	unsigned int	ivBufferLength;

	FP				*ivPtCombRingBuffers[ MAXCOMBFILTER ],
					*ivPtAllpassRingBuffers[ MAXALLPASSFILTER ],

					// lowpass
					ivIl,
					ivUc;

	MReverbBuffer()
	{
		ivBufferLength = 0;
		ivIl = 0;
		ivUc = 0;
		for( unsigned int i=0;i<MAXCOMBFILTER;i++ )
			ivPtCombRingBuffers[ i ] = 0;
		for( i=0;i<MAXALLPASSFILTER;i++ )
			ivPtAllpassRingBuffers[ i ] = 0;
	};

	MReverbBuffer( unsigned int bufferLength )
	{
		ivBufferLength = bufferLength;
		ivIl = 0;
		ivUc = 0;
		for( unsigned int i=0;i<MAXCOMBFILTER;i++ )
			ivPtCombRingBuffers[ i ] = 0;
		for( i=0;i<MAXALLPASSFILTER;i++ )
			ivPtAllpassRingBuffers[ i ] = 0;

		createBuffer();
	};

	~MReverbBuffer()
	{
		deleteBuffer();
	};

	void createBuffer()
	{
		for( unsigned int i=0;i<MAXCOMBFILTER;i++ )
		{
			ivPtCombRingBuffers[ i ] = new FP[ ivBufferLength ];
			ZeroMemory( ivPtCombRingBuffers[ i ], sizeof( ivPtCombRingBuffers[ i ] ) * ivBufferLength );
		}
		for( i=0;i<MAXALLPASSFILTER;i++ )
		{
			ivPtAllpassRingBuffers[ i ] = new FP[ ivBufferLength ];
			ZeroMemory( ivPtAllpassRingBuffers[ i ], sizeof( ivPtAllpassRingBuffers[ i ] ) * ivBufferLength );
		}
	};

	void resetBuffer()
	{
		ivIl = 0;
		ivUc = 0;

		for( unsigned int i=0;i<MAXCOMBFILTER;i++ )
			ZeroMemory( ivPtCombRingBuffers[ i ], sizeof( ivPtCombRingBuffers[ i ] ) * ivBufferLength );
		for( i=0;i<MAXALLPASSFILTER;i++ )
			ZeroMemory( ivPtAllpassRingBuffers[ i ], sizeof( ivPtAllpassRingBuffers[ i ] ) * ivBufferLength );
	};

	void deleteBuffer()
	{
		for( int i=0;i<MAXCOMBFILTER;i++ )
			SAFE_DELETE( ivPtCombRingBuffers[ i ] );
		for( i=0;i<MAXALLPASSFILTER;i++ )
			SAFE_DELETE( ivPtAllpassRingBuffers[ i ] );
		ivBufferLength = 0;
	};
};

class MReverb :
	public MTransformer,
	public IControlListener
{
#ifdef _DEBUG
	bool ivInitialized;
#endif

public:

	static MRtti gvRtti;

	enum REVERB_PARAM
	{
		MIX,
		INPUT,
		DECAY,
		FEEDBACK,
		LOWPASS,
		FEEDBACK2
	};

	static FP COMB_DECAY_COEFFS[ MAXCOMBFILTER ];
	static FP ALLPASS_DECAY_COEFFS[ MAXALLPASSFILTER ];

protected:

	MFloatControl	*ivPtMix,
					*ivPtInput,
					*ivPtDecay,
					*ivPtFeedback,
					*ivPtLowpass,
					*ivPtFeedback2;

	MReverbBuffer**	ivReverbBuffer;


	FP				ivMaxDelayTime,
					ivCombDelayTimes[ MAXCOMBFILTER ],
					ivCombFeedbacks[ MAXCOMBFILTER ],
					ivAllpassDelayTimes[ MAXALLPASSFILTER ];

	int				ivBufferLength,
					ivCurrentStep,
					ivAllpassOffsets[ MAXALLPASSFILTER ],
					ivCombOffsets[ MAXCOMBFILTER ];

	MCriticalSection ivCriticalSection;

public:

	MReverb();
	MReverb( unsigned int channelCount );
	virtual ~MReverb();

	virtual bool create( unsigned int channelCount );

	virtual void valueChanged( MControlListenerEvent *anEvent );

	void		setChannelCount( unsigned int channelCount );

	void		processEvent( MEvent* ptEvent );
	void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	void		setInput( FP input );
	FP			getInput();

	void		setDecay( FP decay );
	FP			getDecay();

	void		setFeedback( FP feedback );
	FP			getFeedback();

	void		setFeedback2( FP feedback2 );
	FP			getFeedback2();

	void		setHighdamp( FP highDump );
	FP			getHighdamp();

	String		getName();
	String		getShortName();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;
	static MObject *createInstance();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );

private:

	FP		getMax( FP* data, unsigned int dataCount );

	void	initReverb( unsigned int channelCount );
	void	updateCoeffsFromDecay();
	void	updateCoeffsFromFeedback();
};

#endif
