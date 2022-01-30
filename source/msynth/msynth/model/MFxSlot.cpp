#include "MFxSlot.h"
#include <gui/dialog/MMessageBox.h>

/**
 * runtime type info
 */
MRtti MFxSlot::gvRtti = MRtti( "MFxSlot", 0 );

/**
 * constructor
 */
MFxSlot::MFxSlot() :
	ivPtCurrentTransformer( 0 ),
	ivLoadRecursionLock( false )
{
	ivNoneString = MObjString( "none" );

	ivPtTransformerRttis.add( &ivNoneString );
	MTransformerRegistry* ptRegistry = MTransformerRegistry::getInstance();
	int transformerCount = ptRegistry->getTransformerCount();
	for(int j=0;j<transformerCount;j++)
		ivPtTransformerRttis.add( ptRegistry->getTransformer( j ) );

	ivPtTransformerRttis.setSelection( (unsigned int)0 );
	ivPtTransformerRttis.addSelectionListener( this );
}

/**
 * destructor 
 */
MFxSlot::~MFxSlot()
{
	ivPtTransformerRttis.removeSelectionListener( this );
	ivListeners.clear();

	SAFE_DELETE( ivPtCurrentTransformer );
}

/**
 * returns the runtime type info
 */
IRtti* MFxSlot::getRtti() const
{
	return &gvRtti;
}

/**
 * queries for a interface
 */
void* MFxSlot::getInterface( const String &className ) const
{
	if( className == "MFxSlot" )
		return (void*) ((MFxSlot*)this);
	else if( className == "ISelectionListener" )
		return (void*)  ((ISelectionListener*)this);
	else
		return MObject::getInterface( className );
}

/**
 * resets the slot (transformer=0)
 */
void MFxSlot::reset()
{
	this->ivPtTransformerRttis.setSelection( (unsigned int) 0 );
}

/**
 * invoked when selection changed in the rtti list
 * newSelection = the new selected object
 * ptSource = the object list
 * TODO: validate this code is working ;-) !!!
 */
void MFxSlot::selectionChanged( MObject* newSelection, MObject* ptSource )
{
	if( ivLoadRecursionLock == false )
	{
		TRACE( "<mfxslot::selectionchanged/>\n" );
		MObject* ptSel = this->ivPtTransformerRttis.getSelectedObject();
		if( ptSel && ptSel != &ivNoneString )
		{
			bool create = true;
			MTransformerRtti* ptRtti = (MTransformerRtti*) ptSel;
			if( create )
				setTransformer(
					MTransformerRegistry::getInstance()->createTransformer(
						((MTransformerRtti*)ivPtTransformerRttis.getSelectedObject()),
						2 ) );
		}
		else
			this->setTransformer( 0 );
	}
}

/**
 * sets the current transformer
 */
void MFxSlot::setTransformer( MTransformer* ptTransformer )
{
	MTransformer* ptOld = ivPtCurrentTransformer;
	ivPtCurrentTransformer = ptTransformer;
	fireFxChanged( ptOld, ivPtCurrentTransformer );
	delete ptOld;
}

/**
 * returns the selected transformer,
 * can be null.
 */
MTransformer* MFxSlot::getTransformer()
{
	return this->ivPtCurrentTransformer;
}

/**
 * returns the list of the available transformers,
 * selecting in this list selects a new effect
 */
MSelectableObjectList* MFxSlot::getTransformerRttis()
{
	return &ivPtTransformerRttis;
}

/**
 * adds a IFxSlotListener
 */
void MFxSlot::addListener( IFxSlotListener* ptListener )
{
	this->ivListeners.addListener( ptListener );
}

/**
 * removes a IFxSlotListener
 */
void MFxSlot::removeListener( IFxSlotListener* ptListener )
{
	this->ivListeners.removeListener( ptListener );
}

/**
 * fires a fx changed notif
 */
void MFxSlot::fireFxChanged( MTransformer* ptOld, MTransformer* ptNew )
{
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IFxSlotListener*)ivListeners.getListener( i ))->onProcessorChanged( ptOld, ptNew );
}

/**
 * loads from the given tree node
 */
void MFxSlot::load( MTreeNode* ptNode )
{
	if( ptNode->getName() == MObject::ELEM_OBJECT &&
		ptNode->getAttribute( MObject::ATTR_CLASS ) == getRtti()->getClassName() )
	{
		if( ptNode->getChildCount() > 0 )
		{
			MTreeNodeIterator iter = ptNode->getIterator();
			MTreeNode* ptFxNode = iter.nextTreeNode();
			if( strcmp( ptFxNode->getName(), "MTransformator" ) == 0 ||
				strcmp( ptFxNode->getName(), "processor" ) == 0 )
			{
				String className = ptFxNode->getAttribute( "value" );
				MObject* ptObject = MDefaultFactory::getInstance()->createObject( className );
				if( ptObject )
				{
					MTransformer* ptTransformer = (MTransformer*) ptObject->getInterface( "MTransformer" );
					if( ptTransformer )
					{
						// backup old transformer...
						MTransformer* pOld = ivPtCurrentTransformer;

						// load fx node...
						ptTransformer->load( ptFxNode );

						// set current transformer...
						ivPtCurrentTransformer = ptTransformer;

						// select right transformer from list...
						ivLoadRecursionLock = true;
						ivPtTransformerRttis.setSelection( ptTransformer->getName() );
						fireFxChanged( pOld, ivPtCurrentTransformer );
						ivLoadRecursionLock = false;

						// delete old transformer if nescessary...
						if( pOld )
							delete pOld;
					}
					else
					{
						delete ptObject;
						MMessageBox::showError( "MFxSlot could not load fx class: " + className + " (not a MTransformer)" );
					}
				}
				else
					MMessageBox::showError( "MFxSlot could not load fx class: " + className + " (not a MObject)" );
			}
		}
	}
	else
		ASSERT( false );
}

/**
 * stores as tree node
 */
MTreeNode* MFxSlot::save()
{
	MTreeNode* ptBack = new MTreeNode( MObject::ELEM_OBJECT );
	ptBack->setAttribute( MObject::ATTR_CLASS, getRtti()->getClassName() );
	if( this->ivPtCurrentTransformer )
		ptBack->addChild( ivPtCurrentTransformer->save() );
	return ptBack;
}