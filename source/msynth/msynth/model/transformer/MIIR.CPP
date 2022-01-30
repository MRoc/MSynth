/*

	(C)2001-2003 MRoc hifiShock

	reference: tiezNSchenk s.1175

*/

#include "MIIR.h"
#include <gui/dialog/MMessageBox.h>

#define MAX_CUTOFF 18000.0f
#define MIN_CUTOFF 20.0f
#define DEF_CUTOFF 5000.0f
#define DEFAULT_CHANNELCOUNT 1

MRtti MIIR::gvRtti = MRtti( "MIIR", MIIR::createInstance );

const char *MIIR::TYPE_STRINGS[] =
{
	"Lowpass",
	"Highpass",
	"Bandpass",
	"Bandstop"
};

const int MIIR::TYPE_VALUES[] =
{
	LOWPASS,
	HIGHPASS,
	BANDPASS,
	BANDSTOP
};

const char *MIIR::ORDER_STRINGS[] =
{
	"2 - 12db/octave",
	"4 - 24db/octave"
};

const int MIIR::ORDER_VALUES[] =
{
	ORDER2,
	ORDER4
};

const char *MIIR::APPROX_STRINGS[] =
{
	"Critical",
	"Bessel",
	"Butterworth",
	"Tschebyscheff1",
	"Tschebyscheff2",
	"Tschebyscheff3"
};

const int MIIR::APPROX_VALUES[] =
{
	PROTO_CRITICAL,
	PROTO_BESSEL,
	PROTO_BUTTERWORTH,
	PROTO_TSCHEBISCHEF1,
	PROTO_TSCHEBISCHEF2,
	PROTO_TSCHEBISCHEF3
};

MIIR::MIIR()
{
#ifdef _DEBUG
	ivInitialized = false;
#endif
}

bool MIIR::create( unsigned int channelCount )
{
	ivChannelCount = channelCount;
	initFilter();
	return true;
}

void MIIR::initFilter()
{
#ifdef _DEBUG
	ASSERT( ! ivInitialized );
	ivInitialized = true;
#endif

	ivSectionCount	= 0;
	ivSections		= 0;
	ivCutoffMulti	= 1;
	ivEnvelope		= new MIIR_ENVELOPE();
	ivLfo			= new LFO();

	ivPtFilterTypeControl = new MDiscreteComboControl( String( "Type" ), TYPE_VALUES, TYPE_STRINGS, 4, LOWPASS );
	ivPtFilterTypeControl->addControlListener( this );

	ivPtOrderControl = new MDiscreteComboControl( String( "Order" ), ORDER_VALUES, ORDER_STRINGS, 2, ORDER2 );
	ivPtOrderControl->addControlListener( this );

	ivPtApproxControl = new MDiscreteComboControl( String( "Approx" ), APPROX_VALUES, APPROX_STRINGS, 5, PROTO_BUTTERWORTH );
	ivPtApproxControl->addControlListener( this );

	ivPtCutoffControl = new MFloatControl( String( "Cutoff" ), MIN_CUTOFF, MAX_CUTOFF, DEF_CUTOFF, false, MFloatControl::CURVE_QUAD, String( "Hz" ), 1.0f );
	ivPtCutoffControl->addControlListener( this );

	ivPtResoControl = new MFloatControl( String( "Resonance" ), 1.0f, 40.0f, 1.0f, true, MFloatControl::CURVE_QUAD );
	ivPtResoControl->addControlListener( this );

	if( ! createFilter( ivChannelCount ) )
		MLogger::logError( "IIR_CREATE_FAILED" );
	else
		setFilterType( getFilterType() );
}

MIIR::~MIIR()
{
	ivPtCutoffControl->removeControlListener( this );
	SAFE_DELETE( ivPtCutoffControl );

	ivPtResoControl->removeControlListener( this );
	SAFE_DELETE( ivPtResoControl );

	ivPtFilterTypeControl->removeControlListener( this );
	SAFE_DELETE( ivPtFilterTypeControl );

	ivPtOrderControl->removeControlListener( this );
	SAFE_DELETE( ivPtOrderControl );

	ivPtApproxControl->removeControlListener( this );
	SAFE_DELETE( ivPtApproxControl );

	SAFE_DELETE( ivEnvelope );
	SAFE_DELETE( ivLfo );

	deleteFilter();
}

unsigned int MIIR::getControlCount()
{
	return 5;
}

IControl* MIIR::getControl( unsigned int index )
{
	ASSERT( index < getControlCount() );

	switch( index )
	{
		case TYPE: return ivPtFilterTypeControl; break;
		case ORDER:	return ivPtOrderControl; break;
		case APPROX: return ivPtApproxControl; break;
		case CUTOFF: return ivPtCutoffControl; break;
		case RESONANCE:	return ivPtResoControl; break;
		default: return 0;
	}
}

IControl* MIIR::getControlByName( String name )
{
	IControl* back = 0;
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++ )
		if( getControl( i ) != 0 &&
			getControl( i )->getName() == name )
			back = getControl( i );

	// not found in own controls? try in lfo and envelope
	if( ! back )
		if( ! (back=ivLfo->getControlByName( name )) )
			back = ivEnvelope->getControlByName( name );

	return back;
}

IControlCollection* MIIR::getChild( unsigned int index )
{
	IControlCollection* back = 0;
	switch( index )
	{
		case 0:
			back = ivEnvelope;
			break;
		case 1:
			back = ivLfo;
			break;
		default:
			ASSERT( false );
			break;
	}
	return back;
}

unsigned int MIIR::getChildCount()
{
	return 2;
}

void MIIR::registerTargets( IAutomationTracker* tracker )
{
	MTransformer::registerTargets( tracker );
	ivLfo->registerTargets( tracker );
	ivEnvelope->registerTargets( tracker );
}

bool MIIR::createFilter( int channelCount )
{
#ifdef _DEBUG
	ASSERT( getOrder() == ORDER2 || getOrder() == ORDER4 );
	ASSERT( channelCount == 1 || channelCount == 2 );
#endif

	MSynchronize cls( &ivCriticalSection );

	bool back = true;

	deleteFilter();

	ivSectionCount = getOrder() / 2;
	ivSections = new MIIR_SECTION*[ ivSectionCount ];
	for( unsigned int i=0;i<ivSectionCount;i++ )
	{
		ivSections[ i ] = new MIIR_SECTION();
		if( ! ivSections[ i ]->createSection( SECTION_ORDER2, channelCount ) )
			back = false;
		else if( ! ivSections[ i ]->putFilterCoeffs( getOrder(), SECTION_ORDER2, getApproxType() ) )
			back = false;			
	}

	
	return back;
}

// make shure this function only called from locked context
void MIIR::deleteFilter()
{
	if( ivSections )
	{
		for( unsigned int i=0;i<ivSectionCount;i++ )
			delete ivSections[ i ];

		SAFE_DELETE( ivSections );
	}
}

void MIIR::calculateFilterCoeffs() 
{
	FP gain = 1.0;
	for( unsigned int i=0;i<ivSectionCount;i++ )
		gain = calculateSectionCoeffs( ivSections[ i ], gain );
}

void MIIR::processEvent( MEvent* ptEvent )
{
	MSynchronize cls( &ivCriticalSection );
	switch( ptEvent->getType() )
	{
	case MEvent::RESET:
		{
			for( unsigned int i=0;i<ivSectionCount;i++ )
				ivSections[ i ]->reset();
		}
	case MEvent::NOTE_ON:
		{
			ivEnvelope->triggerNoteOn();
			ivLfo->reset();
		}
		break;
	case MEvent::NOTE_OFF:
		{
			ivEnvelope->triggerNoteOff();
		}
		break;
	}
}

void MIIR::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	MSynchronize cls( &ivCriticalSection );

	unsigned int envSteps = 0;
	FP *envData = 0;
	unsigned int envResolution = 2;
	if( ivEnvelope->getState() || ivLfo->getLfoState() )
	{
		unsigned int renderLength = stopAt - startFrom;
		FP temp = FP(renderLength) / FP(envResolution);
		envSteps = int( temp );
		envData = 0;
		if( envSteps )
		{
			envData = new FP[ envSteps ];
			FP* ptStop = envData + envSteps;
			for( FP* pt=envData;pt<ptStop;pt++ )
				(*pt) = 1.0f;
			if( ivEnvelope->getState() )
				ivEnvelope->createEnvelope( envData, 0, envSteps );
			if( ivLfo->getLfoState() )
				ivLfo->goNext( envData, 0, envSteps );
		}
	}

	for( unsigned int i=0;i<ivSectionCount;i++ )
		filterSection(	buffer,
						startFrom,
						stopAt,
						ivSections[ i ],
						envSteps,
						envResolution,
						envData );

	if( envSteps )
		delete envData;
}

void MIIR::setCutoff(FP cutoff)
{
	MSynchronize cls( &ivCriticalSection );
	ivPtCutoffControl->setValue( cutoff );
}

FP MIIR::getCutoff()
{	
	return ivPtCutoffControl->getValue();
}

void MIIR::setResonance(FP resonance)
{
	ivPtResoControl->setValue( resonance );
}

FP MIIR::getResonance()
{
	return ivPtResoControl->getValue();
}

bool MIIR::setOrder( int order )
{
	MSynchronize cls( &ivCriticalSection );
	ivPtOrderControl->setValue( order );
	return true;
}

int MIIR::getOrder()
{
	return ivPtOrderControl->getValue();
}

int MIIR::getApproxType()
{
	return ivPtApproxControl->getValue();
}

void MIIR::setApproxType( int approxType )
{
	MSynchronize cls( &ivCriticalSection );
	ivPtApproxControl->setValue( approxType );
}

int MIIR::getFilterType()
{
	return ivPtFilterTypeControl->getValue();
}
		
void MIIR::setFilterType( int type )
{
	MSynchronize cls( &ivCriticalSection );
	ivPtFilterTypeControl->setValue( type );
}

FP MIIR::calculateSectionCoeffs( MIIR_SECTION* section, FP prevSectionGain )
{
	FP cutTemp = getCutoff() * ivCutoffMulti;

	if( cutTemp > MAX_CUTOFF )
		cutTemp = MAX_CUTOFF;
	else if( cutTemp < MIN_CUTOFF )
		cutTemp = MIN_CUTOFF;

	FP f	= cutTemp / FP( MW_SAMPLINGRATE );
	FP l	= 1.0f / FP(tan( PI * f ));
	//FP l	= 1.0f / ( 1.2 * PI * f );
	FP lQ	= l * l;

	FP* protoCoeffs = section->protoCoeffs;
	FP a0 = (*protoCoeffs);
	FP a1 = (*(++protoCoeffs));
	FP a2 = (*(++protoCoeffs));
	FP b0 = (*(++protoCoeffs));
	FP b1 = (*(++protoCoeffs)) / getResonance();
	FP b2 = (*(++protoCoeffs));

	FP a2lQ = a2 * lQ;
	FP b2lQ = b2 * lQ;
	FP a1l = a1 * l;
	FP b1l = b1 * l;

	FP aA = a0 + a1l + a2lQ;
	FP bB = b0 + b1l + b2lQ;
	FP k = (aA / bB);

	FP* coeffs = section->coeffs;

	(*coeffs) = k;
	(*(++coeffs)) = ( 2 * ( a0 - a2lQ ) ) / aA;	// alpha1

	// begin optimisation
	int fType = getFilterType();
	if( fType == LOWPASS ||
		fType == HIGHPASS )
		(*(++coeffs)) = 1.0;	// alpha2
	else
		(*(++coeffs)) = ( a0 - a1l + a2lQ ) / aA;	// alpha2
	// end optimisation

	(*(++coeffs)) = ( 2 * ( b0 - b2lQ ) ) / bB;	// beta1
	(*(++coeffs)) = (  b0 - b1l + b2lQ ) / bB;	// beta2

	return k;
}

void MIIR::filterSection(	MSoundBuffer* data, unsigned int startFrom, unsigned int stopAt,
							MIIR_SECTION* section,
							unsigned int envSteps, unsigned int envResolution, FP* envData )
{
	ASSERT( data->getChannelCount() == section->history->channelCount );

	FP *pt, *bufferStop, *envPt, *history, *coeffs;
	int step;

	for( unsigned int i=0;i<data->getChannelCount();i++ )
	{
		step = 0;
		envPt = envData;
		history = section->history->data[ i ];
		coeffs = section->coeffs;
		bufferStop = data->getData(i) + stopAt;
		for(
			pt=data->getData(i) + startFrom;
			pt < bufferStop;
			pt++, step++ )
		{
			// shift...

			// clean part...
			history[2] = history[1];
			history[1] = history[0];
			history[0] = coeffs[ 0 ] * (*pt); // <- input
					
			// wet part...
			history[5] = history[4];
			history[4] = history[3];

			// filter...
			history[3] =	history[ 0 ] +
							coeffs[ 1 ] * history[ 1 ] +
							coeffs[ 2 ] * history[ 2 ] -
							coeffs[ 3 ] * history[ 4 ] -
							coeffs[ 4 ] * history[ 5 ];

			// output...
			(*pt) = history[3];

			// check update coeffs...
			if( step == envResolution && envSteps && envData )
			{
				step = 0;
	
				ivCutoffMulti = (*envPt);
				envPt++;
				calculateFilterCoeffs();
			}
		}
	}
}

int MIIR::getViewId()
{
	return DESC_VIEW_MIIR;
}

String MIIR::getName()
{
	return gvRtti.getClassName();
}

String MIIR::getShortName()
{
	return "Filter";
}

void MIIR::valueChanged( MControlListenerEvent *anEvent )
{
	IControl* pSrc = anEvent->getSource();
	if( pSrc == ivPtCutoffControl )
		calculateFilterCoeffs();
	else if( pSrc == ivPtResoControl )
		calculateFilterCoeffs();
	else if( pSrc == ivPtApproxControl )
		recreateFilter();
	else if( pSrc == ivPtFilterTypeControl )
		putProtoCoeffs();
	else if( pSrc == ivPtOrderControl )
		recreateFilter();
}

void MIIR::recreateFilter()
{
#ifdef _DEBUG
	ASSERT( createFilter( ivChannelCount ) );
#else
	createFilter( ivChannelCount );
#endif
	setFilterType( getFilterType() );
}

void MIIR::putProtoCoeffs()
{
	ASSERT( getFilterType() >= 0 && getFilterType() < 4 );
	FP* protoCoeffs = 0;
	for( unsigned int i=0;i<ivSectionCount;i++ )
	{
		protoCoeffs = ivSections[ i ]->protoCoeffs;
		switch( getFilterType() )
		{
			case LOWPASS:
				protoCoeffs[ 0 ] = 1;
				protoCoeffs[ 1 ] = 0;
				protoCoeffs[ 2 ] = 0;
				break;
			case HIGHPASS:
				protoCoeffs[ 0 ] = 0;
				protoCoeffs[ 1 ] = 0;
				protoCoeffs[ 2 ] = 1;
				break;
			case BANDPASS:
				protoCoeffs[ 0 ] = 0;
				protoCoeffs[ 1 ] = 1;
				protoCoeffs[ 2 ] = 0;
				break;
			case BANDSTOP:
				protoCoeffs[ 0 ] = 1;
				protoCoeffs[ 1 ] = 0;
				protoCoeffs[ 2 ] = 1;
				break;
			default:
				protoCoeffs[ 0 ] = 1;
				protoCoeffs[ 1 ] = 0;
				protoCoeffs[ 2 ] = 0;
				break;
		};
	}
	calculateFilterCoeffs();
}

IRtti* MIIR::getRtti() const
{
	return &gvRtti;
}

MObject* MIIR::createInstance()
{
	return new MIIR();
}

MTreeNode* MIIR::save()	
{
	MTreeNode* ptBack = new MTreeNode( getType() );

	// store class name
	ptBack->setAttribute( "value", getRtti()->getClassName() );

	// channel count
	ptBack->setAttribute( "channelcount", MInt::toString( ivChannelCount ) );

	// controls
	unsigned int count = getControlCount();
	for( unsigned int i=0;i<count;i++)
		ptBack->addChild( getControl( i )->save() );

	// lfo controls
	count = ivLfo->getControlCount();
	for( i=0;i<count;i++ )
		ptBack->addChild( ivLfo->getControl( i )->save() );

	// envelope controls
	count = ivEnvelope->getControlCount();
	for( i=0;i<count;i++ )
		ptBack->addChild( ivEnvelope->getControl( i )->save() );

	return ptBack;
};

void MIIR::load( MTreeNode* node )
{
	// 1. fetch channel count
	this->ivChannelCount = MInt::parse( node->getAttribute( "channelcount" ), 1 );

	// 2. init effect
	this->create( ivChannelCount );

	// 3. load controls
	MTreeNodeIterator iter = node->getIterator();
	while( iter.hasNext() )
	{
		MTreeNode* controlNode = iter.nextTreeNode();
		String nodeName = controlNode->getName();
		if( nodeName == "control" )
		{
			String controlName = controlNode->getAttribute( "name" );
			IControl* ptControl = getControlByName( controlName );
			ASSERT( ptControl );
			ptControl->load( controlNode );
		}
	}
}

IControlCollection* MIIR::getLfo()
{
	return ivLfo;
}

IControlCollection* MIIR::getEnvelope()
{
	return ivEnvelope;
}

void* MIIR::getInterface( const String &className ) const
{
	if( className == "MIIR" )
		return (void*) ((MIIR*)this);
	else
		return MTransformer::getInterface( className );
}

#ifdef _DEBUG
void MIIR::traceFilterValues()
{
	TRACE( "<MIIR - Cutoff: %f Resonance %f FilterType: %i Order: %i ApproxType: %i>\n",
			getCutoff(),
			getResonance(),
			getFilterType(),
			getOrder(),
			getApproxType() );

}
#endif