#ifndef __MFxSlot
#define __MFxSlot

#include <framework/MObject.h>
#include <framework/MDefaultFactory.h>
#include <framework/primitive/MObjString.h>
#include <framework/list/MSelectableObjectList.h>
#include <framework/treedocument/ISerializeable.h>

#include "transformer/MTransformer.h"
#include "transformer/MTransformerRegistry.h"

/**
 * interface for getting informed about changes
 * in the fx slot
 */
class IFxSlotListener :
	public IListener
{
public:

	/**
	 * invoked when a processor changed
	 * ptOld - the previous slected transformer, can be null will be deleted after
	 * ptNew - the new selected transformer, can be null
	 */
	virtual void onProcessorChanged( MTransformer* ptOld, MTransformer* ptNew ) = 0;
};
/**
 * a slot of a effect rack. it is able
 * to host one effect.
 */
class MFxSlot :
	public MObject,
	public ISelectionListener,
	public ISerializeable
{
public:

	/**
	 * runtime type info
	 */
	static MRtti gvRtti;

	/**
	 * the current selected transformer,
	 * can be null
	 */
	MTransformer* ivPtCurrentTransformer;

	/**
	 * the list of available transformers
	 */
	MSelectableObjectList ivPtTransformerRttis;

	/**
	 * a list of IXfSlotListener
	 */
	MListenerList ivListeners;

	/**
	 * the none string
	 */
	MObjString ivNoneString;

	/**
	 * hack to prevent fx change when loading and setting selection
	 */
	bool ivLoadRecursionLock;

public:

	/**
	 * constructor
	 */
	MFxSlot();

	/**
	 * destructor
	 */
	virtual ~MFxSlot();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * invoked when selection changed in the rtti list
	 * newSelection = the new selected object
	 * ptSource = the object list
	 */
	virtual void selectionChanged( MObject* newSelection, MObject* ptSource );

	/**
	 * resets the slot (transformer=0)
	 */
	virtual void reset();

	/**
	 * returns the selected transformer,
	 * can be null.
	 */
	virtual MTransformer* getTransformer();

	/**
	 * adds a IFxSlotListener
	 */
	virtual void addListener( IFxSlotListener* ptListener );

	/**
	 * removes a IFxSlotListener
	 */
	virtual void removeListener( IFxSlotListener* ptListener );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * returns the list of the available transformers,
	 * selecting in this list selects a new effect
	 */
	virtual MSelectableObjectList* getTransformerRttis();

protected:

	/**
	 * sets the current transformer
	 */
	virtual void setTransformer( MTransformer* ptTransformer );

	/**
	 * fires a fx changed notif
	 */
	virtual void fireFxChanged( MTransformer* ptOld, MTransformer* ptNew );

};

#endif