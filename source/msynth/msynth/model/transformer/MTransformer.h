/**
 * (C)2000-2003 MRoc
 */

#ifndef __MTransformer
#define __MTransformer

#include <framework/MObject.h>
#include <framework/sync/MSynchronize.h>

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

#include "../../../MSynthConstants.h"

class MTransformerInitParam
{
public:

	MTransformerInitParam()
	{
	}

	virtual ~MTransformerInitParam()
	{
	}
};

class MTransformer :
	public MObject,
	public IProcessor,
	public IDescriptor,
	public ISerializeable
{
public:

	static MRtti gvRtti;

protected:

	/**
	 * the channel count if the transformer
	 */
	unsigned int ivChannelCount;

public:


	/**
	 * constructor
	 */
	MTransformer();

	/**
	 * destructor
	 */
	virtual ~MTransformer();

	/**
	 * when creating the effect, this method must be invoked
	 * before invoking the create method.
	 * if the effect loads itself, init must not be invoked.
	 */
	virtual bool		init( MTransformerInitParam* params );

	/**
	 * creates all buffers required by the effect
	 */
	virtual bool		create( unsigned int channelCount );

	/**
	 * fire a event into the processor
	 */
	virtual void processEvent( MEvent* ptEvent );

	/**
	 * processes the given data
	 */
	virtual void goNext(
		MSoundBuffer *buffer,
		unsigned int startFrom,
		unsigned int stopAt ) = 0;

	virtual String		toString();

	virtual IEventTarget* getTarget( unsigned int index );
	virtual unsigned int getTargetCount();

	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int getChildCount();

	virtual void		registerTargets( IAutomationTracker* tracker );

	virtual String		getType();
	virtual String		getName() = 0;
	virtual int			getViewId();

	// ISerializeable
	virtual MTreeNode*	save();
	virtual void		load( MTreeNode* node );

	// MObject
	virtual IRtti*		getRtti() const;
	virtual void*		getInterface( const String &className ) const;

protected:

	IControl*			getControlByName( String name );
};

#endif