/**
 * (C) 2001-2003 MRoc
 *
 * v.0.1	hard coded
 * v.0.2	implemented MFxSlot and inheritanced from MObject
 */

#include "MFxRack.h"

#define FX_COUNT 4

/**
 * the runtime type info
 */
MRtti MFxRack::gvRtti = MRtti( "MFxRack", MFxRack::createInstance );

/**
 * constructor
 */
MFxRack::MFxRack() :
	MObject()
{
	for( unsigned int i=0;i<FX_COUNT;i++ )
		this->ivFxSlots.add( new MFxSlot() );
}

/**
 * destructor
 */
MFxRack::~MFxRack()
{
	this->ivFxSlots.deleteAll();
}

/**
 * returns the runtime type info
 */
IRtti* MFxRack::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for a interface
 */
void* MFxRack::getInterface( const String &className ) const
{
	if( className == "MFxRack" )
		return (void*) ((MFxRack*)this);
	else if( className == "ISerializeable" )
		return (void*) ((ISerializeable*)this);
	else
		return MObject::getInterface( className );
}

/**
 * returns the number of available effect slots
 */
unsigned int MFxRack::getSlotCount()
{
	return this->ivFxSlots.getLength();
}

/**
 * returns the specified slot
 */
MFxSlot* MFxRack::getSlot( unsigned int index )
{
	ASSERT( index >= 0 && index < getSlotCount() );
	return (MFxSlot*) this->ivFxSlots.get( index );
}

/**
 * deletes all effects stored in slots
 */
void MFxRack::deleteSlots()
{
	unsigned int count = getSlotCount();
	for( unsigned int i=0;i<count;i++ )
		getSlot( i )->reset();
}

/**
 * resets all effects stored in slots
 */
void MFxRack::resetSlots()
{
	unsigned int count = getSlotCount();
	for( unsigned int i=0;i<count;i++ )
	{
		MFxSlot* ptSlot = getSlot( i );
		if( ptSlot->getTransformer() )
			ptSlot->getTransformer()->processEvent( &MEvent(MEvent::RESET) );
	}
}

/**
 * stores as tree node
 */
MTreeNode* MFxRack::save()
{
	MSynchronize cls( &ivCriticalSection );

	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );

	unsigned int count = this->getSlotCount();
	for( unsigned int i=0;i<count;i++ )
		ptBack->addChild( getSlot( i )->save() );

	return ptBack;
}

/**
 * loads from the given tree node
 */
void MFxRack::load( MTreeNode* ptNode )
{
	MSynchronize cls( &ivCriticalSection );

	MTreeNodeIterator iter = ptNode->getIterator();
	int index = 0;
	while( iter.hasNext() )
	{
		MTreeNode* ptChild = iter.nextTreeNode();
		if( ptChild->getName() == MObject::ELEM_OBJECT &&
			ptChild->getAttribute( MObject::ATTR_CLASS ) == "MFxSlot" &&
			index < FX_COUNT )
		{
			getSlot( index++ )->load( ptChild );
		}
	}
}

/**
 * creates a new effect rack
 */
MObject* MFxRack::createInstance()
{
	return new MFxRack();
}