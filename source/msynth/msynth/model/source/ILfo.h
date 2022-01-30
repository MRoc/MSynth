#ifndef __ILfo
#define __ILfo

#include <framework/MTypes.h>

class ILfo
{
public:

	virtual void	setLfoState( bool onOff ) = 0;
	virtual bool	getLfoState() = 0;

	virtual void	setLfoBandWidth( FP bandWidth ) = 0;
	virtual FP		getLfoBandWidth() = 0;

	virtual void	setLfoFrequence( FP frequence ) = 0;
	virtual FP		getLfoFrequence() = 0;
};

#endif