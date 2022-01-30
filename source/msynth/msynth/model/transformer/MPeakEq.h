/*

	(C)2001 MRoc hifiShock

*/

#ifndef __MPeakEq
#define __MPeakEq

#include "MTransformer.h"

#include "MBiquad.h"

struct MEqSection
{
	FP frequence;
	FP gain;
	FP q;
		
	int stackCount;
	MBiquadCoeffs coeffs;
	MBiquadStack** stacks;
};

class MPeakEq : public MTransformer, public IControlListener
{
public:

	static MRtti gvRtti;

protected:
	
	static const FP LOG10;
	static const FP TWOPI;

	static const int gvSectionCount;
	static const FP gvFrequencies[];

	unsigned int	ivSectionCount;	
	MEqSection**	ivArPtSections;
	MFloatControl**	ivArPtGainControls;
	MBiquad			ivBiquad;

public:

	MPeakEq();
	MPeakEq( int channelCount );
	virtual ~MPeakEq();

	virtual bool create( unsigned int channelCount );

	virtual String	getName();
	virtual String toString();

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int index );
	virtual void valueChanged( MControlListenerEvent *anEvent );

	virtual void	initFx( int channelCount );

	virtual void	calculateFilterCoefficients();
	virtual void	calculateFilterCoefficients( unsigned int section );

	virtual void	processEvent( MEvent* ptEvent );
	virtual void	goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt );

	virtual String getShortName();
	
	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	static MObject* createInstance();
};

#endif

