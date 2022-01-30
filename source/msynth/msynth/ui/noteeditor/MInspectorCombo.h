#ifndef __MINSPECTORCOMBO
#define __MINSPECTORCOMBO

#include <framework/MTypes.h>
#include <framework/primitive/MObjString.h>
#include <gui/combobox/MComboBox.h>
#include "MNoteInspector.h"
#include "../control/inspector/MVolumeInspector.h"
#include "../control/inspector/MFloatEventInspector.h"
#include "../../model/MBar.h"

class MInspectorCombo : public MComboBox
{
protected:
	class MInspectorSelectionListener;
	class MTargetRegistryListener;
private:

	MBar* ivPtBar;
	MNoteInspector* ivPtInspector;
	MInspectorSelectionListener* ivPtListener;
	MTargetRegistryListener* ivPtRegistryListener;

public:

	MInspectorCombo();
	virtual ~MInspectorCombo();
	void setBar( MBar* ptBar );
	MBar* getBar();
	void setNoteInspector( MNoteInspector* ptInspector );
	MNoteInspector* getNoteInspector();
	void init();
	void selectTarget( IEventTarget* ptTarget );
	bool hasTarget( IEventTarget* ptTarget );
	int getIndex( IEventTarget* ptTarget );

public:	

	virtual void onselectionChanged();

protected:

	class MInspectorComboItem : public MObjString
	{
	protected:
		IBarInspector* ivPtInspector;
		IEventTarget* ivPtTarget;
	public:
		MInspectorComboItem( String name, IBarInspector* ptInspector, IEventTarget* ptTarget ) :
			MObjString( name ),
			ivPtInspector( ptInspector ),
			ivPtTarget( ptTarget )
		{
			ASSERT( this->ivData != "" );
			ASSERT( ivPtInspector );
		}
		virtual ~MInspectorComboItem(){}
		IBarInspector* getInspector(){return ivPtInspector;}
		IEventTarget* getTarget(){return ivPtTarget;}
	};

	class MInspectorSelectionListener :
		public MObject,
		public ISelectionListener
	{
	protected:
		MInspectorCombo* ivPtInstance;
	public:
		FULL_RTTI( MInspectorCombo::MInspectorSelectionListener, "MInspectorCombo::MInspectorSelectionListener" );
		MInspectorSelectionListener( MInspectorCombo* ptInstance ) :
			ivPtInstance( ptInstance ){}
		virtual ~MInspectorSelectionListener(){}
		virtual void selectionChanged( MObject* newSelection, MObject* ptSource ){
			ivPtInstance->onselectionChanged();
		}
		virtual String getClassName(){
			return "MInspectorSelectionListener";
		}
	};

	friend class MTargetRegistryListener;

	class MTargetRegistryListener :
		public MObject,
		public ITargetRegistryListener
	{
	protected:
		MInspectorCombo* ivPtInstance;
	public:
		FULL_RTTI( MInspectorCombo::MTargetRegistryListener, "MInspectorCombo::MTargetRegistryListener" );
		MTargetRegistryListener( MInspectorCombo* ptInstance ) :
			ivPtInstance( ptInstance ){
			MTargetRegistry::getInstance()->addListener( this );
		}
		virtual ~MTargetRegistryListener(){
			MTargetRegistry::getInstance()->removeListener( this );
		}
		virtual void automationAdded( IEventTarget* ptControl ){
			TRACE( "<minspectorcombo::mtargetregistrylistener::oncontroladdedtoautomation>\n" );
			if( ivPtInstance->hasTarget( ptControl ) )
				TRACE( "\terror!!!\n" );
			else
			{
				TRACE( "\treiniting...\n" );
				int curSel = ivPtInstance->ivPtPopup->getModel()->getSelection();
				ivPtInstance->init();
				if( ivPtInstance->hasTarget( ptControl ) )
					ivPtInstance->selectTarget( ptControl );
				else
					ivPtInstance->ivPtPopup->getModel()->setSelection( curSel );
				ivPtInstance->repaint();
			}
			TRACE( "</minspectorcombo::mtargetregistrylistener::oncontroladdedtoautomation>\n" );
		}
		virtual void automationRemoved( IEventTarget* ptControl ){
			TRACE( "<minspectorcombo::oncontrolremovedfromautomation/>\n" );
			if( ivPtInstance->hasTarget( ptControl ) )
			{
				TRACE( "remove to do!!!\n" );
				ivPtInstance->init();
				IBarInspector* pti = ivPtInstance->ivPtInspector->getInspectorByName( "floatEvent" );
				MFloatEventInspector* ptf = (MFloatEventInspector*) pti;
				if( ptf->getCurrentTarget() == ptControl )
					ivPtInstance->ivPtPopup->getModel()->setSelection( 0 );
				ivPtInstance->repaint();
			}
		}
	};
};

#endif