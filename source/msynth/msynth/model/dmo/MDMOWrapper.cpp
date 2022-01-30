#include "MDMOWrapper.h"

static String dxError( HRESULT err );

MRtti MDMOWrapper::gvRtti = MRtti( "MDMOWrapper", createInstance );

MDMOWrapper::MDMOWrapper(): 
	m_name( "" ),
    m_pObject(0),
    m_pObjectInPlace(0),
    m_pwfx(0),
	m_pParamInfo(0)
{
}

bool MDMOWrapper::init( MTransformerInitParam* ptParams )
{
	MDmoWrapperInitParam* ptDmoParam = (MDmoWrapperInitParam*) ptParams;

	m_clsidDMO = ptDmoParam->getClsid();
	m_name = ptDmoParam->getName();

	return true;
}

bool MDMOWrapper::create( unsigned int channelCount )
{
	if( FAILED( CoInitialize( NULL ) ) )
		throw MException( "MDMOWrapper: CoInitialize failed!\nCould not initialize COM Library!" );

	ivChannelCount = channelCount;

	m_pwfx = new WAVEFORMATEX();
	m_pwfx->wFormatTag			= WAVE_FORMAT_PCM;
	m_pwfx->nChannels			= ivChannelCount;
	m_pwfx->nSamplesPerSec		= 44100;
	m_pwfx->wBitsPerSample		= 16;
	m_pwfx->nBlockAlign			= m_pwfx->nChannels * m_pwfx->wBitsPerSample / 8;
	m_pwfx->nAvgBytesPerSec		= m_pwfx->nSamplesPerSec * m_pwfx->nBlockAlign;
	m_pwfx->cbSize				= 0;

    m_mt.majortype  = MEDIATYPE_Audio;
    m_mt.subtype    = MEDIASUBTYPE_PCM;
    m_mt.formattype = FORMAT_WaveFormatEx;
    m_mt.cbFormat = sizeof(WAVEFORMATEX);
    m_mt.pbFormat = (unsigned char*)(m_pwfx);
    m_mt.pUnk = NULL; // CopyMediaType will crash if we don't intialize this

	init();

	return true;
}

bool MDMOWrapper::init()
{
	HRESULT back = init( m_pwfx->nChannels,	m_clsidDMO );
	if( FAILED( back ) )
	{
		MMessageBox::showError( "DMO_INIT_FAILED", dxError( back ) );
		return false;
	}

	DWORD paramCount = 0;
	back = m_pParamInfo->GetParamCount( &paramCount );
	if( FAILED( back ) )
	{
		MMessageBox::showError(	"DMO_GETPARAMCOUNT_FAILED", dxError( back ) );
		return false;
	}

	unsigned int count = (unsigned int) paramCount;
	unsigned int found = 0;
	for( unsigned int i=0;i<count;i++ )
	{
		MP_PARAMINFO paramInfo;
		back = m_pParamInfo->GetParamInfo( (DWORD)i, &paramInfo );
		if( FAILED( back ) )
		{
			MMessageBox::showError( "DMO_GETPARAMINFO_FAILED", dxError( back ) );
			break;
		}
		else
		{
			if( paramInfo.mpType == MPT_FLOAT )
				ivDmoControls.push_back(
					new MDmoControl( 
						i,
						m_pObject,
						m_pParamInfo,
						m_mMediaParams ) );
			else
				MLogger::logMessage(
					"<mdmowrapper::init uncatched_control=\"%s\" index=\"%i\"/>\n",
					GetMpTypeString( paramInfo.mpType ).getData(),
					i );
		}
	}
	//MLogger::logMessage( "<mdmowrapper controlcount=\"%i\"/>\n", ivDmoControls.size() );

	return true;
};

unsigned int MDMOWrapper::getControlCount()
{
	return ivDmoControls.size();
};

IControl* MDMOWrapper::getControl( unsigned int i )
{
	ASSERT( i < ivDmoControls.size() );
	return ivDmoControls[ i ]->getFloatControl();
};

//-----------------------------------------------------------------------------
// Name: CAppStream::~CAppStream()
// Desc: Destructor
//-----------------------------------------------------------------------------
MDMOWrapper::~MDMOWrapper() 
{
	unsigned int count = ivDmoControls.size();
	for( unsigned int i=0;i<count;i++ )
		delete ivDmoControls[ i ];
	ivDmoControls.clear();

	SAFE_RELEASE( m_pParamInfo );
	SAFE_RELEASE( m_mMediaParams );
	SAFE_RELEASE( m_pObject);
	SAFE_RELEASE( m_pObjectInPlace);
	SAFE_DELETE( m_pwfx );
	CoUninitialize();
}

//-----------------------------------------------------------------------------
// Name: CAppStream::init()
// Desc: Load data from input file, create media object and set input&output type
//-----------------------------------------------------------------------------
HRESULT MDMOWrapper::init( unsigned int channelCount, REFGUID rclsid ) 
{
    // create DMO
    HRESULT hr = CoCreateInstance(
					rclsid,
					NULL,
					CLSCTX_INPROC,
					IID_IMediaObject,
					(void **) &m_pObject );

    if ( FAILED( hr ) )
	{
		MMessageBox::showError( "DMO_COCREATEINSTANCE_FAILED", dxError( hr ) );
        return hr;
    }
    hr = m_pObject->QueryInterface(IID_IMediaObjectInPlace, (void**)&m_pObjectInPlace);
    if ( FAILED( hr ) )
	{
        MMessageBox::showError( "DMO_FAILED_QUERY_IMEDIAOBJECTINPLACE", dxError( hr ) );
		return hr;
    }

    hr = m_pObject->SetInputType( 0,    //Input Stream index
                                  &m_mt, 
                                  0 );  // No flags specified
    if ( FAILED( hr ) )
	{
        MMessageBox::showError( "DMO_SETINPUTTYPE_FAILED", dxError( hr ) );
        return hr;
    }

    hr = m_pObject->SetOutputType( 0,       // Output Stream Index
                                   &m_mt, 
                                   0);  // No flags specified
    if ( FAILED( hr ) )
	{
       MMessageBox::showError( "DMO_SETOUTPUTTYPE_FAILED", dxError( hr ) );
       return hr;
    }

	m_pParamInfo = 0;
	hr = m_pObject->QueryInterface( IID_IMediaParamInfo, (void**) &m_pParamInfo );
	if( FAILED( hr ) )
	{
		MMessageBox::showError( "DMO_QUERY_IMEDIAPARAMINFO_FAILED", dxError( hr ) );
		return hr;
	}

	m_mMediaParams = 0;
	hr = m_pObject->QueryInterface( IID_IMediaParams, (void**) &m_mMediaParams );
	if( FAILED( hr ) )
	{
		MMessageBox::showError( "DMO_QUERY_IMEDIAPARAMS_FAILED", dxError( hr ) );
		return hr;
	}
 
    return S_OK;
}

void MDMOWrapper::processEvent( MEvent* ptEvent )
{
	if( ptEvent->getType() == MEvent::RESET && m_pObject )
	{
		if( FAILED( m_pObject->Lock( DMOLOCK ) ) )
			MMessageBox::showError( "DMO_LOCK_FAILED" );
		if( FAILED( m_pObject->Flush() ) )
			MMessageBox::showError( "DMO_FLUSH_FAILED" );
		if( FAILED( m_pObject->Lock( DMORELEASE ) ) )
			MMessageBox::showError( "DMO_UNLOCK_FAILED" );
	}
}

void MDMOWrapper::goNext( MSoundBuffer *ptBuffer, unsigned int start, unsigned int stop )
{
	if( m_pObject && FAILED( m_pObject->Lock( DMOLOCK ) ) )
		MMessageBox::showError( "DMO_LOCK_FAILED" );

	MBYTEBuffer b;
	b.importSoundBuffer( ptBuffer, start, stop );

    if( m_pObjectInPlace )
		m_pObjectInPlace->Process(
			b.getSize(), 
			b.getData(), 
			0, 
			DMO_INPLACE_NORMAL );

	b.exportSoundBuffer( ptBuffer, start, stop );

	if( m_pObject && FAILED( m_pObject->Lock( DMORELEASE ) ) )
		MMessageBox::showError( "DMO_UNLOCK_FAILED" );
}

String dxError( HRESULT err )
{
	switch( err )
	{
		case DS_OK:
			return "The method succeeded.";
			break;
#ifndef __VC6
		case DS_NO_VIRTUALIZATION: 
			return "The buffer was created, but another 3-D algorithm was substituted.";
			break;
		case DS_INCOMPLETE:
			return "The method succeeded, but not all the optional effects were obtained.";
			break;
		case DSERR_ACCESSDENIED:
			return "The request failed because access was denied.";
			break;
		case DSERR_BUFFERTOOSMALL:
			return "The buffer size is not great enough to enable effects processing.";
			break;
		case DSERR_DS8_REQUIRED:
			return "A DirectSound object of class CLSID_DirectSound8 or later is required for the requested functionality. For more information, see IDirectSound8.";
			break;
		case DSERR_OBJECTNOTFOUND:
			return "The requested object was not found.";
			break;
#endif
		case DSERR_ALLOCATED:
			return "The request failed because resources, such as a priority level, were already in use by another caller.";
			break;
		case DSERR_ALREADYINITIALIZED:
			return "The object is already initialized.";
			break;
		case DSERR_BADFORMAT:
			return "The specified wave format is not supported.";
			break;
		case DSERR_BUFFERLOST:
			return "The buffer memory has been lost and must be restored.";
			break;
		case DSERR_CONTROLUNAVAIL:
			return "The buffer control (volume, pan, and so on) requested by the caller is not available.";
			break;
		case DSERR_GENERIC:
			return "An undetermined error occurred inside the DirectSound subsystem.";
			break;
		case DSERR_INVALIDCALL:
			return "This function is not valid for the current state of this object.";
			break;
		case DSERR_INVALIDPARAM:
			return "An invalid parameter was passed to the returning function.";
			break;
		case DSERR_NOAGGREGATION:
			return "The object does not support aggregation.";
			break;
		case DSERR_NODRIVER:
			return "No sound driver is available for use, or the given GUID is not a valid DirectSound device ID.";
			break;
		case DSERR_NOINTERFACE:
			return "The requested COM interface is not available.";
			break;
		case DSERR_OTHERAPPHASPRIO:
			return "Another application has a higher priority level, preventing this call from succeeding.";
			break;
		case DSERR_OUTOFMEMORY:
			return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
			break;
		case DSERR_PRIOLEVELNEEDED:
			return "The caller does not have the priority level required for the function to succeed.";
			break;
		case DSERR_UNINITIALIZED:
			return "The IDirectSound8::Initialize method has not been called or has not been called successfully before other methods were called.";
			break;
		case DSERR_UNSUPPORTED:
			return "The function called is not supported at this time.";
			break;
		case DMO_E_INVALIDSTREAMINDEX:
			return "Invalid stream index.";
			break;
		case DMO_E_TYPE_NOT_ACCEPTED:
			return "Media type was not accepted.";
			break;
	}
	return "undefined";
}

String MDMOWrapper::GetMpTypeString( MP_TYPE type )
{
	switch( type )
	{
		case MPT_INT: return"MPT_INT"; break;
		case MPT_FLOAT: return "MPT_FLOAT"; break;
		case MPT_BOOL: return "MPT_BOOL"; break;
		case MPT_ENUM: return "MPT_ENUM"; break;
		case MPT_MAX: return "MPT_MAX"; break;
		default: return "unknown"; break;
	};
}

IRtti* MDMOWrapper::getRtti() const
{
	return &gvRtti;
}

void* MDMOWrapper::getInterface( const String &className ) const
{
	if( className == "MDMOWrapper" )
		return (void*) ((MDMOWrapper*) this );
	else
		return MTransformer::getInterface( className );
}

MObject* MDMOWrapper::createInstance()
{
	return new MDMOWrapper();
}

void MDMOWrapper::load( MTreeNode* parentNode )
{
	// 1. fetch DMO name from node
	String dmoName = parentNode->getAttribute( "type" );

	// 2. fetch transformer rtti from registry by DMO name
	MTransformerRtti* ptRtti = MTransformerRegistry::getInstance()->getTransformerByName( dmoName );

	// 3. init DMO with the MDmoWrapperInitParam
	init( ptRtti->getInitParams() );

	// 4. load and create transformer
	MTransformer::load( parentNode );
}

MTreeNode* MDMOWrapper::save()
{
	MTreeNode* ptBack = MTransformer::save();
	ptBack->setAttribute( "type", m_name );
	return ptBack;
}

String MDMOWrapper::getType()
{
	return "processor";
}

String MDMOWrapper::getName()
{
	return m_name;
}

String MDMOWrapper::getShortName()
{
	return m_name;
}

IControl* MDMOWrapper::getControlByName( String name )
{
	size_t count = ivDmoControls.size();
	for( size_t i=0;i<count;i++ )
		if( ivDmoControls[ i ]->getName() == name )
			return ivDmoControls[ i ]->getFloatControl();
	return 0;
}