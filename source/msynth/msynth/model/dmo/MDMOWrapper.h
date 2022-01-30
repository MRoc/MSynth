#ifndef __MDMOWRAPPER
#define __MDMOWRAPPER

#include <framework/MTypes.h>
#include <framework/MNativeTypes.h>
#include <framework/MException.h>
#include <gui/dialog/MMessageBox.h>

#include <mmsystem.h>
#include <dsound.h>
#include <medparam.h>
#include <dmo.h>
#include <uuids.h>

#include "../transformer/MTransformerRegistry.h"
#include "../../../resource.h"
#include "../../../MAppData.h"

#define MAXNAMELENGTH 100
#define MAX_NUM 100

#define DMOLOCK 1
#define DMORELEASE 0	

/**
 * a wrapper arround one parameter of a DMO plugin
 */
class MDmoControl :
	public IControlListener
{
protected:

	/** 
	 * the media object
	 */
	IMediaObject* ivPtMediaObject;

	/**
	 * the dmo plugin's parameters
	 */
	IMediaParams* ivPtMediaParams;

	/**
	 * the index of the parameter in the
	 * dmo plugin's paratemers
	 */
	DWORD ivIndex;

	/**
	 * static information about parameter
	 */
	MP_PARAMINFO ivParamInfo;

	/**
	 * the embedded float control
	 */
	MFloatControl* ivPtFloatControl;

	/**
	 * the control's name
	 */
	String ivName;

public:

	/**
	 * constructor initializing the static information about parameter
	 */
	MDmoControl( DWORD index, IMediaObject* pMediaObject, IMediaParamInfo* pParamInfo, IMediaParams* pParam ) :
		ivIndex( index ),
		ivPtMediaObject( pMediaObject ),
		ivPtMediaParams( pParam ),
		ivPtFloatControl( 0 )
	{
		HRESULT res = pParamInfo->GetParamInfo( ivIndex, &ivParamInfo );
		if( FAILED( res ) )
			throw MException( "DMO_GETPARAMINFO_FAILED" );

		TCHAR tempStr[MAX_PATH];
		if( wcstombs( tempStr, ivParamInfo.szLabel, MAX_PATH ) == -1 )
			throw MException( "DMO_GETPARAMINFO_FAILED: wcstombs failed" );
		ivName = String( tempStr );

		ivPtFloatControl = new MFloatControl( 
			getName(),
			getMinValue(),
			getMaxValue(),
			getValue(),
			false );
		ivPtFloatControl->addControlListener( this );
	}

	/**
	 * destructor
	 */
	virtual ~MDmoControl()
	{
		if( ivPtFloatControl )
		{
			ivPtFloatControl->removeControlListener( this );
			delete ivPtFloatControl;
			ivPtFloatControl = 0;
		}
	}

	/**
	 * returns the control's name
	 */
	virtual String getName()
	{
		return ivName;
	}

	/**
	 * returns the control's minimal value
	 */
	virtual MP_DATA getMinValue()
	{
		return ivParamInfo.mpdMinValue;
	}

	/**
	 * returns the control's maximal value
	 */
	virtual MP_DATA getMaxValue()
	{
		return ivParamInfo.mpdMaxValue;
	}

	/**
	 * returns the control's default value
	 */
	virtual MP_DATA getDefaultValue()
	{
		return ivParamInfo.mpdNeutralValue;
	}

	/**
	 * returns the control's current value
	 */
	virtual MP_DATA getValue()
	{
		MP_DATA back;
		HRESULT res = ivPtMediaParams->GetParam( ivIndex, &back );
		if( FAILED( res ) )
			MMessageBox::showError( "DMO_GETPARAM_FAILED" );
		return back;
	}

	/**
	 * sets the control's current value
	 */
	virtual void setValue( MP_DATA value )
	{
		if( ivPtMediaObject )
		{
			if( FAILED( ivPtMediaObject->Lock( DMOLOCK ) ) )
				MMessageBox::showError( "DMO_LOCK_FAILED" );
			if( FAILED( ivPtMediaParams->SetParam( ivIndex, value ) ) )
				MMessageBox::showError( "DMO_SETPARAM_FAILED" );
			if( FAILED( ivPtMediaObject->Lock( DMORELEASE ) ) )
				MMessageBox::showError( "DMO_UNLOCK_FAILED" );
		}
	}

	/**
	 * returns the embedded float control
	 */
	virtual MFloatControl* getFloatControl()
	{
		return ivPtFloatControl;
	}

	/**
	 * invoked when embedded float control changed,
	 * inheritanced from IControlListener
	 */
	virtual void valueChanged( MControlListenerEvent *anEvent )
	{
		setValue( (MP_DATA) ivPtFloatControl->getValue() );
	}

	/**
	 * query interface
	 */
	virtual void* getInterface( const String &className ) const
	{
		return 0;
	};
};

class MDmoWrapperInitParam :
	public MTransformerInitParam
{
protected:

	CLSID ivClsidDMO;
	String ivName;

public:

	MDmoWrapperInitParam( CLSID clsidDMO, String name ) :
		MTransformerInitParam(),
		ivClsidDMO( clsidDMO ),
		ivName( name )
	{
	}

	virtual ~MDmoWrapperInitParam()
	{
	}

	virtual CLSID getClsid()
	{
		return this->ivClsidDMO;
	}

	virtual String getName()
	{
		return this->ivName;
	}
};

class MDMOWrapper :
	public MTransformer
{
public:

	static MRtti gvRtti;

protected:

	IMediaParams		*m_mMediaParams;
	IMediaParamInfo		*m_pParamInfo;
	IMediaObject        *m_pObject;
	IMediaObjectInPlace *m_pObjectInPlace;
	LPWAVEFORMATEX      m_pwfx;         // pointer to input/output waveformatex structure.
	DMO_MEDIA_TYPE      m_mt;
	CLSID				m_clsidDMO;
	String				m_name;

	std::vector<MDmoControl*> ivDmoControls;

public:

	MDMOWrapper();
	virtual ~MDMOWrapper();

	/**
	 * when creating the effect, this method must be invoked
	 * before invoking the create method.
	 * if the effect loads itself, init must not be invoked.
	 */
	virtual bool init( MTransformerInitParam* ptParams );

	/**
	 * creates all buffers required by the effect
	 */
	virtual bool create( unsigned int channelCount );

	virtual unsigned int getControlCount();
	virtual IControl* getControl( unsigned int i );

	virtual void processEvent( MEvent* ptEvent );
	virtual void goNext( MSoundBuffer *ptBuffer, unsigned int start, unsigned int stop );

	virtual String getType();
	virtual String getName();
	virtual String getShortName();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;
	static MObject* createInstance();

	virtual void load( MTreeNode* parentNode );
	virtual MTreeNode* save();
		
protected:

	virtual IControl* getControlByName( String name );

	bool init();

	HRESULT init( unsigned int channelCount, REFGUID rclsid );
	HRESULT ProcessOutputs( unsigned char **pbOutData);

	String GetMpTypeString( MP_TYPE type );
};

#endif