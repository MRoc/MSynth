/*
(C)2001-2003 MRoc hifiShock
reference: tiezNSchenk s.1175
*/

#ifndef __MIIR
#define __MIIR

#include "../../../resource.h"
#include "MTransformer.h"
#include "../source/MLfo.h"

class MIIR_BUFFER
{
public:

	int				channelCount;
	int				historyLength;
	FP**			data;

	MIIR_BUFFER( int _channelCount, int _historyLength )
	{
		channelCount = _channelCount;
		historyLength = _historyLength;
		data = new FP*[ channelCount ];
		for( int i=0;i<channelCount;i++ )
			data[ i ] = new FP[ historyLength ];
		reset();
	};

	virtual ~MIIR_BUFFER()
	{
		for( int i=0;i<channelCount;i++ )
			delete data[ i ];
		delete data;
	};

	void reset()
	{
		for( int i=0;i<channelCount;i++ )
			memset( data[ i ], 0, sizeof( FP ) * historyLength );
	};
};

class MIIR_SECTION
{
public:

	int				order;
	FP				protoCoeffs[ 6 ];
	FP				coeffs[ 5 ];
	MIIR_BUFFER*	history;

public:

	MIIR_SECTION()
	{
		order		= 2;
		history		= 0;
	};

	virtual ~MIIR_SECTION()
	{
		SAFE_DELETE( history );
	}

	void reset()
	{
		history->reset();
	};

	bool createSection( int _order, int channelCount )
	{
		order = _order;
		memset( protoCoeffs, 0, 6 * sizeof( FP ) );
		memset( coeffs, 0, 5 * sizeof( FP ) );
		history = new MIIR_BUFFER( channelCount, 6 );
		return true;
	};

	bool putFilterCoeffs( int filterOrder, int sectionOrder, int type )
	{
		switch( type )
		{
			case 0:	// CRITICAL
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.2872f;
								protoCoeffs[ 5 ] = 0.4142f;
								break;
							default:
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.8700f;
								protoCoeffs[ 5 ] = 0.1892f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.8700f;
								protoCoeffs[ 5 ] = 0.1892f;
								break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
			case 1:	// BESSEL
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.3617f;
								protoCoeffs[ 5 ] = 0.6180f;
								break;
							default:
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.3397f;
								protoCoeffs[ 5 ] = 0.4889f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.7743f;
								protoCoeffs[ 5 ] = 0.3890f;
								break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
			case 2:	// BUTTERWORTH
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.4142f;
								protoCoeffs[ 5 ] = 1.0f;
								break;
							default:
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.8478f;
								protoCoeffs[ 5 ] = 1.0f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.7654f;
								protoCoeffs[ 5 ] = 1.0f;
								break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
			case 3:	// TSCHEBISCHEFF 1
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.3022f;
								protoCoeffs[ 5 ] = 1.5515f;
							break;
							default:	
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 2.5904f;
								protoCoeffs[ 5 ] = 4.1301f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.3039f;
								protoCoeffs[ 5 ] = 1.1697f;
								break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
			case 4:	// TSCHEBISCHEFF 2
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.1813f;
								protoCoeffs[ 5 ] = 1.7775f;
								break;
							default:
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 2.4025f;
								protoCoeffs[ 5 ] = 4.9862f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.2374f;
								protoCoeffs[ 5 ] = 1.1896f;
							break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
			case 5:	// TSCHEBISCHEF (3db)
				switch( filterOrder )
				{
					case 2:
						switch( sectionOrder )
						{
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 1.0650f;
								protoCoeffs[ 5 ] = 1.9305f;
								break;
							default:
								return false;
						};
						break;
					case 4:
						switch( sectionOrder )
						{
							case 1:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 2.1853f;
								protoCoeffs[ 5 ] = 5.5339f;
								break;
							case 2:
								protoCoeffs[ 3 ] = 1.0f;
								protoCoeffs[ 4 ] = 0.1964f;
								protoCoeffs[ 5 ] = 1.2009f;
								break;
							default:
								return false;
						};
						break;
					default:
						return false;
				};
				break;
	
			default:
				return false;
		};

		return true;
	};
};

class MIIR_ENVELOPE : public IControlCollection
{
public:

	enum MIIR_ENVELOPE_Controls
	{
		ENVON,
		OFFSET,
		ATTACK,
		GAIN,
		DECAY,
		SUSTAIN,
		RELEASE,
	};

protected:

	bool			noteOn;
	unsigned int	step;
	FP				lastValue;

	// filter envelope
	MBoolControl*	ivPtEnvOnControl;
	MFloatControl*	ivPtAttackControl;
	MFloatControl*	ivPtDecayControl;
	MFloatControl*	ivPtReleaseControl;
	MFloatControl*	ivPtOffsetControl;
	MFloatControl*	ivPtGainControl;
	MFloatControl*	ivPtSustainControl;

public:

	MIIR_ENVELOPE()
	{
		ivPtEnvOnControl = new MBoolControl( String( "Env" ), true );
		ivPtAttackControl = new MFloatControl( String( "Attack" ), 1.0f, 80000.0f, 1000.0f, false, MFloatControl::CURVE_QUAD, String("msec" ), 0.045351f );
		ivPtDecayControl = new MFloatControl( String( "Decay" ), 1.0f, 80000.0f, 1000.0f, false, MFloatControl::CURVE_QUAD, String("msec" ), 0.045351f );
		ivPtReleaseControl = new MFloatControl( String( "Release" ), 1.0f, 80000.0f, 1000.0f, false, MFloatControl::CURVE_QUAD, String("msec" ), 0.045351f );
		ivPtOffsetControl = new MFloatControl( String( "Offset" ), 0.01f, 2.0f, 0.3f, false );
		ivPtGainControl = new MFloatControl( String( "Gain" ), 0.01f, 2.0f, 1.8f, false );
		ivPtSustainControl = new MFloatControl( String( "Sustain" ), 0.01f, 2.0f, 1.0f, false );

		setNoteOn( true );
		setStep( 0 );
		setLastValue( ivPtOffsetControl->getValue() );
	}

	~MIIR_ENVELOPE()
	{
		SAFE_DELETE( ivPtEnvOnControl );
		SAFE_DELETE( ivPtAttackControl );
		SAFE_DELETE( ivPtDecayControl );
		SAFE_DELETE( ivPtReleaseControl );
		SAFE_DELETE( ivPtOffsetControl );
		SAFE_DELETE( ivPtGainControl );
		SAFE_DELETE( ivPtSustainControl );
	}

	virtual String toString()
	{
		return getName();
	}

	virtual IControlCollection* getChild( unsigned int index ){	return 0; }
	virtual unsigned int getChildCount(){ return 0; }

	unsigned int getControlCount()
	{
		return RELEASE+1;
	}

	IControl* getControl( unsigned int index )
	{
		ASSERT( index < getControlCount() );
	
		switch( index )
		{
			case ENVON: return ivPtEnvOnControl; break;
			case OFFSET: return ivPtOffsetControl; break;
			case GAIN: return ivPtGainControl; break;
			case ATTACK: return ivPtAttackControl; break;
			case DECAY: return ivPtDecayControl; break;
			case SUSTAIN: return ivPtSustainControl; break;
			case RELEASE: return ivPtReleaseControl; break;
			default: return 0;
		}
	}

	void triggerNoteOn()
	{
		step = 0;
		noteOn = true;
		lastValue = ivPtOffsetControl->getValue();
	}

	void triggerNoteOff()
	{
		step = 0;
		noteOn = false;
	}

	void createEnvelope( FP* data, unsigned int startFrom, unsigned int stopAt )
	{
		FP gain = ivPtGainControl->getValue();
		FP offset = ivPtOffsetControl->getValue();
		FP sustain = ivPtSustainControl->getValue();
		unsigned int attack = (unsigned int) ivPtAttackControl->getValue();
		unsigned int decay = (unsigned int) ivPtDecayControl->getValue();
		unsigned int release = (unsigned int) ivPtReleaseControl->getValue();

		ASSERT( startFrom < stopAt );
		FP *bufferStop = data + stopAt;
		FP delta;

		if( noteOn )
		{
			int stepsToRender = stopAt - startFrom;
			int stepsRendered = 0;
			while( stepsRendered < stepsToRender )
			{
				unsigned int decayEnd = attack + decay;
				unsigned int stepsLeftForSection = 0;
				if( step < attack ) // Attack
				{
					//TRACE( "ATTACK %f /", envelope->lastValue );
					stepsLeftForSection = attack - step;
					delta = ( gain - lastValue ) / stepsLeftForSection;
				}
				else if( step < decayEnd ) //Decay
				{
					//TRACE( "DECAY %f /", envelope->lastValue );
					stepsLeftForSection = decayEnd - step;
					delta = ( lastValue - sustain ) / stepsLeftForSection * (-1);
				}
				else //Substain
				{
					//TRACE( "SUBSTAIN %f /", envelope->lastValue );
					stepsLeftForSection = stepsToRender - stepsRendered;
					delta = 0;
	
				}
	
				unsigned int start = stepsRendered + startFrom;
				unsigned int stop = start + stepsLeftForSection;
				if( stop > stopAt )
				{
					stop = stopAt;
					stepsLeftForSection = stop - start;
				}
				lastValue = iterativeLinearFunction(
								lastValue, 
								delta,
								data,
								start,
								stop );
	
				//TRACE( " %f\n", envelope->lastValue );
	
				stepsRendered += stepsLeftForSection;
				step += stepsLeftForSection;
			}
		}
		else // release and after
		{
			int stepsToRender = stopAt - startFrom;
			int stepsRendered = 0;
			while( stepsRendered < stepsToRender )
			{
				int stepsLeftForSection = 0;
				if( step >= release )
				{
					stepsLeftForSection = stepsToRender - stepsRendered;
					delta = 0;
					//TRACE( "ALL OVER: %f\n", envelope->lastValue );
				}
				else
				{
					//TRACE( "RELEASE: %f\n", envelope->lastValue );
					stepsLeftForSection = release - step;
					delta = ( offset - lastValue ) / stepsLeftForSection;
				}
	
				unsigned int start = stepsRendered + startFrom;
				unsigned int stop = start + stepsLeftForSection;
				if( stop > stopAt )
				{
					stop = stopAt;
					stepsLeftForSection = stopAt - stepsRendered;
				}
				lastValue = iterativeLinearFunction(
								lastValue, 
								delta,
								data,
								start,
								stop );
	
				stepsRendered += stepsLeftForSection;
				step += stepsLeftForSection;
			}
		}
	}

	bool getNoteOn()
	{
		return noteOn;
	}

	void setNoteOn( bool _noteOn )
	{
		noteOn = _noteOn;
	}

	int	getStep()
	{
		return step;
	}

	void setStep( int _step )
	{
		step = _step;
	}

	FP getLastValue()
	{
		return lastValue;
	}

	void setLastValue( FP _lastValue )
	{
		lastValue = _lastValue;
	}

	virtual String getType()
	{
		return String("individualprocessor" );
	}

	virtual String getName()
	{
		return String( "Env" );
	}
	
	virtual bool getState()
	{
		return ivPtEnvOnControl->getValue();
	}

	virtual void registerTargets( IAutomationTracker* tracker )
	{
		unsigned int count = getControlCount();
		for( unsigned int i=0;i<count;i++ )
		{
			IEventTarget* pTarget = (IEventTarget*) getControl( i )->getInterface( "IEventTarget" );
			if( pTarget )
				MTargetRegistry::getInstance()->registerContext(
					new MAutomationContext( pTarget, tracker ) );
		}
	}

	virtual IControl* getControlByName( String name )
	{
		IControl* back = 0;
		unsigned int count = getControlCount();
		for( unsigned int i=0;i<count;i++ )
			if( getControl( i ) != 0 &&
				getControl( i )->getName() == name )
				back = getControl( i );
		return back;
	}

protected:

	FP iterativeLinearFunction( FP y, FP a, FP* data, unsigned int start, unsigned int stop  )
	{
		FP* pt = data + start;
		FP* ptStop = data + stop;

		for( ;pt<ptStop;pt++,y+=a )
			(*pt) = y;

		return y;
	}
};

class MIIR :
	public MTransformer,
	public IControlListener
{
public:

	static MRtti gvRtti;

	static const char *TYPE_STRINGS[];
	static const int TYPE_VALUES[];

	static const char *ORDER_STRINGS[];
	static const int ORDER_VALUES[];

	static const char *APPROX_STRINGS[];
	static const int APPROX_VALUES[];

	enum FilterType
	{
		LOWPASS,
		HIGHPASS,
		BANDPASS,
		BANDSTOP
	};

	enum FilterOrder
	{
		ORDER1 = 1, // not supported
		ORDER2 = 2,
		ORDER3 = 3, // not supported
		ORDER4 = 4
	};

	enum SectionOrder
	{
		SECTION_ORDER1 = 1, // not supported
		SECTION_ORDER2 = 2
	};

	enum ApproxType
	{
		PROTO_CRITICAL,
		PROTO_BESSEL,
		PROTO_BUTTERWORTH,
		PROTO_TSCHEBISCHEF1,
		PROTO_TSCHEBISCHEF2,
		PROTO_TSCHEBISCHEF3
	};

	enum Descriptor
	{
		TYPE,
		ORDER,
		APPROX,
		CUTOFF,
		RESONANCE,
	};

protected:

	// filter type
	MDiscreteComboControl*	ivPtFilterTypeControl;
	MDiscreteComboControl*	ivPtOrderControl;
	MDiscreteComboControl*	ivPtApproxControl;

	// filter controls
	MFloatControl*		ivPtCutoffControl;
	MFloatControl*		ivPtResoControl;

	// runtime values
	FP					ivCutoffMulti;
	unsigned int		ivSectionCount;

	// used objects
	MIIR_SECTION**		ivSections;
	MIIR_ENVELOPE*		ivEnvelope;
	LFO*				ivLfo;

	// thread safe
	MCriticalSection	ivCriticalSection;

public:

	MIIR();
	virtual ~MIIR();

	virtual void* getInterface( const String &className ) const;

	virtual bool create( unsigned int channelCount );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );
	virtual IControl* getControlByName( String name );
	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int getChildCount();
	virtual void registerTargets( IAutomationTracker* tracker );

	virtual void		processEvent( MEvent* ptEvent );
	virtual void		goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	void				setCutoff(FP cutoff);
	FP					getCutoff();

	void				setResonance(FP resonance);
	FP					getResonance();

	bool				setOrder( int order );
	int					getOrder();

	int					getApproxType();
	void				setApproxType( int approxType );

	int					getFilterType();
	void				setFilterType( int type );

	virtual String		getName();
	virtual String		getShortName();
	
	virtual int			getViewId();

	virtual void		valueChanged( MControlListenerEvent *anEvent );

	virtual MTreeNode*	save();
	virtual void		load( MTreeNode* node );

	virtual IRtti* getRtti() const;
	static MObject *createInstance();

	virtual IControlCollection* getLfo();
	virtual IControlCollection* getEnvelope();

private:

	void			initFilter();
	inline bool		createFilter( int channelCount );
	inline void		deleteFilter();

	__forceinline void	calculateFilterCoeffs();
	__forceinline FP calculateSectionCoeffs( MIIR_SECTION* section, FP prevSectionGain );

	inline void		filterSection( MSoundBuffer* data, unsigned int startFrom, unsigned int stopAt, MIIR_SECTION* section, unsigned int envSteps, unsigned int envResolution, FP* envData );

	void			putProtoCoeffs();
	void			recreateFilter();

#ifdef _DEBUG
	bool ivInitialized;
	void traceFilterValues();
#endif

};

#endif
