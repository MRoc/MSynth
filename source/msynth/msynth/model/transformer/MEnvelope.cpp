/*(C)2000 MRoc hifiShock*/

#include "MEnvelope.h"

MRtti MEnvelope::gvRtti = MRtti( "MEnvelope", MEnvelope::createInstance );
#include "../interfaces/MSoundBufferUtils.h"

MEnvelope::MEnvelope() :
	ivPtAttackControl( 0 ),
	ivPtDecayControl( 0 ),
	ivPtReleaseControl( 0 ),
	ivPtSustainControl( 0 ),
	ivState( E_OFF ),
	ivStep( 0 ),
	ivValue( 0.0f )
{
	ivPtAttackControl = new MFloatControl( String( "Attack" ), 50.0f, 1000000.0f, 4000.0f, true, MFloatControl::CURVE_QUAD, String( "msec" ), 0.022676f );
	ivPtDecayControl = new MFloatControl( String( "Decay" ), 50.0f, 1000000.0f, 4000.0f, true, MFloatControl::CURVE_QUAD, String( "msec" ), 0.022676f );
	ivPtReleaseControl = new MFloatControl( String( "Release" ), 50.0f, 1000000.0f, 4000.0f, true, MFloatControl::CURVE_QUAD, String( "msec" ), 0.022676f );
	ivPtSustainControl = new MFloatControl( String( "Sustain" ), 0.001f, 1.0f, 0.3f, false );
}

MEnvelope::~MEnvelope()
{
	SAFE_DELETE( ivPtAttackControl );
	SAFE_DELETE( ivPtDecayControl );
	SAFE_DELETE( ivPtReleaseControl );
	SAFE_DELETE( ivPtSustainControl );
}

unsigned int MEnvelope::getControlCount()
{
	return 4;
}

IControl* MEnvelope::getControl( unsigned int index )
{
	switch( index )
	{
		case ATTACK: return ivPtAttackControl; break;
		case DECAY: return ivPtDecayControl; break;
		case RELEASE: return ivPtReleaseControl; break;
		case SUSTAIN: return ivPtSustainControl; break;
		default: return 0; break;
	}
}

void MEnvelope::processEvent( MEvent* ptEvent )
{
	switch( ptEvent->getType() )
	{
	case MEvent::NOTE_ON:
		{
			//TRACE( "MEnvelope->processEvent: NOTE_ON\n" );
			ivValue = ivValueBeginState = 0.0f;
			ivStep = 0;
			ivState = E_ATTACK;
		}
		break;
	case MEvent::NOTE_OFF:
		{
			//TRACE( "MEnvelope->processEvent: NOTE_OFF\n" );
			ivStep = 0;
			ivValueBeginState = ivValue;
			ivState = E_RELEASE;
		}
		break;
	case MEvent::FALLDOWN:
		{
			//TRACE( "MEnvelope->processEvent: FALLDOWN\n" );
			ivStep = 0;
			ivValueBeginState = ivValue;
			ivState = E_FALLDOWN;
		}
		break;
	case MEvent::RESET:
		{
			//TRACE( "MEnvelope->processEvent: RESET\n" );
			ivValue = ivValueBeginState = 0.0f;
			ivStep = 0;
			ivState = E_OFF;
		}
		break;
	}
}

void MEnvelope::validateState( unsigned int attack, unsigned int decay, unsigned int release, unsigned int fallDown )
{
	// update state if attack/decay/release currently changed
	switch( ivState )
	{
	case E_ATTACK:
		if( ivStep >= attack )
		{
			ivState = E_DECAY;
			ivValueBeginState = ivValue;
			ivStep = 0;
		}
		break;
	case E_DECAY:
		if( ivStep >= decay )
		{
			ivState = E_SUSTAIN;
			ivValueBeginState = ivValue;
			ivStep = 0;
		}
		break;
	case E_RELEASE:
		if( ivStep >= release )
		{
			ivState = E_OFF;
			ivStep = 0;
		}
		break;
	case E_FALLDOWN:
		if( ivStep >= fallDown )
		{
			ivState = E_OFF;
			ivStep = 0;
		}
		break;
	}
}

void MEnvelope::goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt )
{
	ASSERT( buffer );
	ASSERT( startFrom <= stopAt );

	unsigned int samplesRendered = 0;
	unsigned int samplesToRender = stopAt - startFrom;
	unsigned int attack = (unsigned int)getAttack();
	unsigned int decay = (unsigned int)getDecay();
	unsigned int release = (unsigned int)getRelease();
	double sustain = (double)getSustain();

	validateState( attack, decay, release, 300 );

	// now render...
	while( samplesRendered < samplesToRender )
	{
		switch( ivState )
		{
		case E_ATTACK:
			{
				ASSERT( ivStep <= attack );

				unsigned int stepsToRun = attack - ivStep;
				if( stepsToRun > samplesToRender - samplesRendered )
					stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );
				//TRACE( "\tattack step=%u len=%u value=%f\n", ivStep, stepsToRun, ivValue );
				ivValue =
					MSoundBufferUtils::fade(
						buffer,
						startFrom + samplesRendered,
						startFrom + samplesRendered + stepsToRun,
						ivValue,
						1.0f / double( attack ) );
				samplesRendered += stepsToRun;
				ivStep += stepsToRun;
				validateState( attack, decay, release, 300 );
			}
			break;
		case E_DECAY:
			{
				if( ivStep >= decay )
				{
					TRACE( "decay failed\n" );
					ASSERT( false );
				}

				unsigned int stepsToRun = decay - ivStep;
				if( stepsToRun > samplesToRender - samplesRendered )
					stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );

				ivValue =
					MSoundBufferUtils::fade(
						buffer,
						startFrom + samplesRendered,
						startFrom + samplesRendered + stepsToRun,
						ivValue,
						- ( (1.0f - sustain) / double(decay) ) );
				samplesRendered += stepsToRun;

				ivStep += stepsToRun;
				validateState( attack, decay, release, 300 );
			}
			break;
		case E_SUSTAIN:
			{
				unsigned int stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );
				//TRACE( "\tsustain step=%u len=%u value=%f\n", ivStep, stepsToRun, ivValue );
				MSoundBufferUtils::multiply(
					buffer,
					startFrom + samplesRendered,
					startFrom + samplesRendered + stepsToRun,
					(float) ivValue );
				samplesRendered += stepsToRun;
				ivStep += stepsToRun;
			}
			break;
		case E_RELEASE:
			{
				ASSERT( ivStep <= release );
				unsigned int stepsToRun = release - ivStep;
				if( stepsToRun > samplesToRender - samplesRendered )
					stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );
				//TRACE( "\trelease step=%u len=%u value=%f\n", ivStep, stepsToRun, ivValue );
				ivValue =
					MSoundBufferUtils::fade(
						buffer,
						startFrom + samplesRendered,
						startFrom + samplesRendered + stepsToRun,
						ivValue,
						-(ivValueBeginState / double(release)) );
				samplesRendered += stepsToRun;
				ivStep += stepsToRun;
				validateState( attack, decay, release, 300 );
			}
			break;
		case E_FALLDOWN:
			{
				unsigned int stepsToRun = 300 - ivStep;
				if( stepsToRun > samplesToRender - samplesRendered )
					stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );
				//TRACE( "\tfalldown step=%u len=%u value=%f\n", ivStep, stepsToRun, ivValue );
				ivValue =
					MSoundBufferUtils::fade(
						buffer,
						startFrom + samplesRendered,
						startFrom + samplesRendered + stepsToRun,
						ivValue,
						-( ivValueBeginState / double(300) ) );
				samplesRendered += stepsToRun;
				ivStep += stepsToRun;
				validateState( attack, decay, release, 300 );
			}
			break;
		case E_OFF:
			{
				unsigned int stepsToRun = samplesToRender - samplesRendered;
				ASSERT( stepsToRun );
				MSoundBufferUtils::multiply(
					buffer,
					startFrom + samplesRendered,
					startFrom + samplesRendered + stepsToRun,
					0.0f );
				samplesRendered += stepsToRun;
				ivStep += stepsToRun;
			}
			break;
		}
	}

	//TRACE( "MEnvelope::goNext after max=%f\n", MSoundBufferUtils::findMax( buffer ) );
}

String MEnvelope::getName()
{
	return String("Envelope ADSR");
}

String MEnvelope::getShortName()
{
	return String("Envelope");
}

IRtti* MEnvelope::getRtti() const
{
	return &gvRtti;
}

MObject* MEnvelope::createInstance()
{
	return new MEnvelope();
}