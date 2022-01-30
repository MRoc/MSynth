/**
 * (C) 20001-2003 MRoc
 */

#ifndef __MFXRACK
#define __MFXRACK

#include <framework/MUtil.h>
#include "MFxSlot.h"

/**
 * implementation of a effect rack with 4 effect slots
 */
class MFxRack :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

	/**
	 * sync
	 */
	MCriticalSection	ivCriticalSection;

protected:

	/**
	 * list of slots
	 */
	MObjectList ivFxSlots;

public:

	/**
	 * constructor
	 */
	MFxRack();

	/**
	 * destructor
	 */
	virtual ~MFxRack();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the number of fx slots in this effect rack
	 */
	virtual unsigned int getSlotCount();

	/**
	 * returns the specified slot
	 */
	virtual MFxSlot* getSlot( unsigned int index );

	/**
	 * deletes all effects stored in slots
	 */
	virtual void deleteSlots();

	/**
	 * resets all effects stored in slots
	 */
	virtual void resetSlots();

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * creates a instance of this class
	 */
	static MObject* createInstance();
};

#endif