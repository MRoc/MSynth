/**
 * (C)2000-2003 MRoc
 */

#ifndef __MOSC
#define __MOSC

#include <framework/MUtil.h>
#include <framework/MObject.h>

#include "../event/MEventNoteOn.h"
#include "../event/MEventPortamento.h"

#include "../interfaces/IProcessor.h"
#include "../interfaces/IName.h"
#include "../interfaces/IDescriptor.h"
#include "../control/IControlCollection.h"
#include "../control/MIntControl.h"
#include "../control/MFloatControl.h"
#include "../control/MRangeControl.h"
#include "../control/MBoolControl.h"
#include "../control/MStringControl.h"
#include "../control/MDiscreteComboControl.h"
#include "../automation/IAutomationContext.h"
#include "../automation/ITargetRegistry.h"

#define MOSC_CONTROL_COUNT 4

/**
 * superclass of all oscilators
 */
class MOsc :
	public MObject,
	public IProcessor,
	public IDescriptor,
	public ISerializeable,
	public IControlListener
{
public:

	enum OSCPARAM
	{
		OCTAVE,
		VOLUME,
		TUNE,
		PHASE
	};

protected:

	MIntControl*			ivPtOctaveControl;
	MFloatControl*			ivPtVolumeControl;
	MFloatControl*			ivPtTuneControl;
	MFloatControl*			ivPtPhaseControl;

	unsigned int			ivPortamentoLength,
							ivPortamentoStep;
	FP						ivPortamentoTargetFrequence;
	bool					ivIsPortamento;

	FP						ivFreq;
	FP						ivOctaveMultiplier;

public:

	/**
	 * constructor
	 */
	MOsc();

	/**
	 * destructor
	 */
	virtual ~MOsc();

	/**
	 * queries for a interface
	 */
	virtual void*			getInterface( const String &className ) const;

	/**
	 * returns a string representation
	 */
	virtual String			toString();

	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int	getChildCount();

	virtual unsigned int	getControlCount();
	virtual IControl*		getControl( unsigned int index );

	// inheritanced from IControlListener
	virtual void			valueChanged( MControlListenerEvent *anEvent );

	/**
	 * fire a event into the processor
	 */
	virtual void processEvent( MEvent* ptEvent );

	/**
	 * renders a wave
	 */
	virtual void goNext( MSoundBuffer *buffer,
		unsigned int startFrom,
		unsigned int stopAt ) = 0;

	virtual void	setFreq( FP freq );
	virtual void	addPitch( FP delta );
	virtual void	reset();

	virtual void	updateOctaveMultiplier();

	String			getType();
	virtual	String	getName()					= 0;
	virtual String	getShortName()				= 0;

	virtual void	triggerPortamento( FP targetFrequence, unsigned int portamentoLength );
	virtual void	stopPortamento();
	virtual bool	getPortamento();

	virtual MTreeNode* save();
	virtual void	load( MTreeNode* node );

	virtual int		getViewId();

	virtual void	registerTargets( IAutomationTracker* tracker );

protected:

	virtual FP getFreq();

	IControl* getControlByName( String name );
};

#endif