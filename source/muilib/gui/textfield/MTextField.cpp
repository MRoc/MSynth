#include "MTextField.h"
#include <framework/performance/MClock.h>
#include "MTextFieldRenderer.h"

/** runtime type info */
MRtti MTextField::gvRtti = MRtti( "MTextField", MTextField::createInstance );

const unsigned int MTextField::SHIFT_DOWN = 1024;

MTextField::MTextField() :
	ivOffset( 0 ),
	ivVisibleCharacters( 0 ),
	ivPtSelectionTimer( 0 ),
	ivPtCursor( 0 ),
	ivPtRenderer( 0 ),
	ivPtModel( 0 )
{
	ivPtSelectionTimer = new MAutoSelection( this );
	ivPtCursor = new MTextFieldCursor( this );
	ivPtRenderer = new MTextFieldRenderer();
	ivPtModel = new MTextFieldModel();
	ivPtModel->addListener( this );
}

MTextField::~MTextField()
{
	if( ivPtModel )
	{
		ivPtModel->removeListener( this );
		delete ivPtModel;
	}
	SAFE_DELETE( ivPtSelectionTimer );
	SAFE_DELETE( ivPtCursor );
	SAFE_DELETE( ivPtRenderer );
}

/** returns the current text offset in characters */
int MTextField::getOffset() const
{
	return ivOffset;
}

/** returns the number of visible characters */
int MTextField::getVisibleCharacters() const
{
	return ivVisibleCharacters;
}

void MTextField::paint( IGraphics* pG, const MRect& rect )
{
	ivPtRenderer->paint( this, pG, rect );
}

void MTextField::onMouseUp( MMouseEvent* pEvent )
{
	MyInteractiveWnd::onMouseUp( pEvent );
	if( pEvent->getButton() == MMouseEvent::BUTTON1 && ivPtSelectionTimer->isOn() )
		ivPtSelectionTimer->stop();
}

void MTextField::onMouseDown( MMouseEvent* pEvent )
{
	MyInteractiveWnd::onMouseDown( pEvent );
	ivPtModel->setCursorAndSelection( getHitIndex( pEvent->getPoint().getX() ) );
}

void MTextField::onMouseMove( MMouseEvent* pEvent )
{
	MyInteractiveWnd::onMouseMove( pEvent );
	if( getMouseDownLeft() )
	{
		MRect rect = getTextRect();
		MPoint point = pEvent->getPoint();
		if( rect.contains( point ) )
			ivPtModel->setCursor( getHitIndex( pEvent->getPoint().getX() ) );
		else
		{
			MInsets insets = ivPtRenderer->getInsets();
			if( pEvent->getPoint().getX() < insets.left() )
			{
				if( ! ivPtSelectionTimer->isOn() )
					ivPtSelectionTimer->start( MAutoSelection::DEC );
			}
			else if( pEvent->getPoint().getX() > getWidth() - insets.right() )
			{
				if( ! ivPtSelectionTimer->isOn() )
					ivPtSelectionTimer->start( MAutoSelection::INC );
			}
			else if( ivPtSelectionTimer->isOn() )
				ivPtSelectionTimer->stop();
		}
	}
}

/** returns the substring of text from ivOffset to end */ 
String MTextField::getTextToDraw() const
{
	String text = ivPtModel->getText();

	ASSERT( ivOffset >= 0 && ivVisibleCharacters >= 0 );
	ASSERT( ivOffset + ivVisibleCharacters <= text.GetLength() );

	return text.Mid( ivOffset, ivVisibleCharacters );
}

/** returns the text from offset to cursor */
String MTextField::getTextToCursor() const
{
	String text = ivPtModel->getText();
	int cursor = ivPtModel->getCursor();

	ASSERT( ivOffset >= 0 && ivOffset <= text.GetLength() );
	ASSERT( ivOffset + (cursor-ivOffset) >= 0 );
	ASSERT( ivOffset + (cursor-ivOffset) <= text.GetLength() );

	return text.Mid( ivOffset, cursor-ivOffset ); // everyting from offset
}

/** returns the index of the character hidden by x in the textfield */
int MTextField::getHitIndex( const int _x ) const
{
	String text = ivPtModel->getText();
	MFont* pFont = MFont::getFont();
	const int* pWidths = pFont->getABCWidth();

	const char* pSrc = text.getData();
	int len=text.GetLength(), width = 0, back=-1, x=_x-ivPtRenderer->getInsets().left();
	for( int i=ivOffset;i<len&&back==-1;i++ )
	{
		width += pWidths[ (unsigned char)(pSrc[ i ]) ];
		if( width >= x )
			back = i;
	}

	if( back == -1 )
		back = len;

#ifdef _DEBUG
	ASSERT( back >= 0 );
	ASSERT( back <= text.GetLength() );
#endif

	return back;
}

/** returns the last character's index not clipped by x */
int MTextField::getIndexNotClipped( const String& text, const int x ) const
{
	int back = -1;

	MFont* pFont = MFont::getFont();
	const char* pSrc = text.getData();
	const int* pWidths = pFont->getABCWidth();

	int width = 0, len = text.GetLength();
	for( int i=0;i<len;i++ )
	{
		unsigned char index = pSrc[ i ];
		ASSERT( (index) >= 0 );
		ASSERT( (index) < 256 );
		width += pWidths[ index ];
		if( width >= x )
		{
			back = i - 1;
			break;
		}
	}

	if( back == -1 )
		back = len;

#ifdef _DEBUG
	ASSERT( back >= 0 );
	ASSERT( back <= getText().GetLength() );
#endif

	return back;
}

/** returns the offset of the first character fitting in width when indexed is the last one */
int MTextField::getOffsetForCharReverse( const String& text, const int index, const int width ) const
{
	ASSERT( index >= 0 );

	int iIndex = index;
	if( iIndex >= text.GetLength() )
		iIndex = text.GetLength()-1;

	MFont* pFont = MFont::getFont();
	const char* pSrc = text.getData();
	const int* pWidths = pFont->getABCWidth();

	bool breaked = false;
	int w = 0;
	for( int i=iIndex;i>=0; )
	{
		w += pWidths[ (unsigned char)pSrc[ i ] ];
		if( w > width )	// if next char exceeds limit,
		{
			i++; // one back...
			breaked = true;
			break;			// and break.
		}
		else
			i--;
	}
	if( i < 0 )
		i=0;

	return i;
}

/** returns width of given text */
int MTextField::getTextWidth( const String& text ) const
{
	int back = 0;

	MFont* pFont = MFont::getFont();
	const char* pSrc = text.getData();
	const int* pWidths = pFont->getABCWidth();
	while( *pSrc )
	{
		back += pWidths[ (unsigned char) *pSrc ];
		pSrc++;
	}

	return back;
}

bool MTextField::processEvent( IUiEvent* pEvent )
{
	switch( pEvent->getEventType() )
	{
	case IUiEvent::MEventType::EVENT_FOCUS:
		{
			MFocusEvent* pFEvent = (MFocusEvent*) pEvent;
			switch( pFEvent->getType() )
			{
			case MFocusEvent::MFocusEventType::FOCUS_GAINED: ivPtCursor->start(); break;
			case MFocusEvent::MFocusEventType::FOCUS_LOST: ivPtCursor->stop(); break;
			}
		}
		break;
	}
	return MyInteractiveWnd::processEvent( pEvent );
}

/** query interface */
void* MTextField::getInterface( const String& className ) const
{
	if( className == "MTextField" )
		return (void*) ((MTextField*)this);
	else
		return MyInteractiveWnd::getInterface( className );
}

void MTextField::onKeyUp( MKeyEvent* pEvent )
{
	MyInteractiveWnd::onKeyUp( pEvent );
	switch( pEvent->getScanCode() )
	{
	case MKeyEvent::MVirtualKey::MVK_SHIFT:
		setShiftDown( false );
		break;
	}
}

void MTextField::onKeyDown( MKeyEvent* pEvent )
{
	MyInteractiveWnd::onKeyDown( pEvent );
	switch( pEvent->getScanCode() )
	{
	case MKeyEvent::MVirtualKey::MVK_ENTER:
	case MKeyEvent::MVirtualKey::MVK_ESCAPE:
		break;
	case MKeyEvent::MVirtualKey::MVK_DELETE: ivPtModel->del(); break;
	case MKeyEvent::MVirtualKey::MVK_BACK_SPACE: ivPtModel->backspace(); break;
	case MKeyEvent::MVirtualKey::MVK_LEFT: ivPtModel->decCursor( ! getShiftDown() ); break;
	case MKeyEvent::MVirtualKey::MVK_RIGHT: ivPtModel->incCursor( ! getShiftDown() ); break;
	case MKeyEvent::MVirtualKey::MVK_HOME:
		if( getShiftDown() ) ivPtModel->setCursor( 0 );
		else ivPtModel->setCursorAndSelection( 0 );
		break;
	case MKeyEvent::MVirtualKey::MVK_END:
		if( getShiftDown() ) ivPtModel->setCursor( ivPtModel->getText().GetLength() );
		else ivPtModel->setCursorAndSelection( ivPtModel->getText().GetLength() );
		break;
	case MKeyEvent::MVirtualKey::MVK_SHIFT: setShiftDown( true ); break;
	default:
		if( pEvent->getKeyChar() ) ivPtModel->insert( pEvent->getKeyChar() );
		break;
	}
}

void MTextField::setOffset( int offset )
{
	String text = ivPtModel->getText();

	ASSERT( offset >= 0 );
	ASSERT( offset <= text.GetLength() );

	ivOffset = offset;
	if( ivOffset > text.GetLength() )
		ivOffset = text.GetLength();
	ivVisibleCharacters = getHitIndex( getWidth()-ivPtRenderer->getInsets().right() ) - ivOffset;
	ivMaxPossibleCursor =
		ivOffset +
		getIndexNotClipped(
			text.Right( text.GetLength() - ivOffset ),
			getWidth()-ivPtRenderer->getInsets().width() );

	ASSERT( ivOffset + ivVisibleCharacters <= text.GetLength() );
}

/** returns the textfield's text */
String MTextField::getText() const
{
	return ivPtModel->getText();
}

/** sets the textfield's text */
void MTextField::setText( const String& text ) const
{
	ivPtModel->setText( text );
	ivPtModel->setCursorAndSelection( 0 );
	this->ivPtRenderer->setCursorVisible( false );
}

void MTextField::setRect( const MRect& rect )
{
	MyInteractiveWnd::setRect( rect );
	setOffset( ivOffset );
}
void MTextField::setSize( const MSize& size )
{
	MyInteractiveWnd::setSize( size );
	setOffset( ivOffset );
}

MRect MTextField::getTextRect() const
{
	MInsets insets = ivPtRenderer->getInsets();
	return
		MRect(
			insets.left(),
			insets.top(),
			getWidth() - (insets.left() + insets.right()),
			getHeight() - (insets.bottom() + insets.top()) );
}

/** sets the shift down flag */
void MTextField::setShiftDown( bool value )
{
	if( getShiftDown() != value )
		ivBitState ^= SHIFT_DOWN;
}

/** returns the shift down flag */
bool MTextField::getShiftDown()
{
	return (ivBitState & SHIFT_DOWN) != 0;
}

MTextField::MAutoSelection::MAutoSelection( MTextField* pOwner ) :
	ivDirection( INC ),
	ivPtTextField( pOwner )
{
	ivTimer.addListener( this );
}
MTextField::MAutoSelection::~MAutoSelection()
{
	ivTimer.removeListener( this );
	if( ivTimer.isRunnning() )
		ivTimer.stopTimer();
}
void MTextField::MAutoSelection::start( Direction direction )
{
	TRACE( "MTextField::MAutoSelection::start()\n" );
	ivDirection = direction;
	ivTimer.startTimer( 100 );
}
void MTextField::MAutoSelection::stop()
{
	TRACE( "MTextField::MAutoSelection::stop()\n" );
	ivTimer.stopTimer();
}
bool MTextField::MAutoSelection::isOn()
{
	return ivTimer.isRunnning();
}
void MTextField::MAutoSelection::onActionPerformed( void* ptSrc )
{
	TRACE( "MTextField::MAutoSelection::onTimer()\n" );
	switch( ivDirection )
	{
	case INC: ivPtTextField->ivPtModel->incCursor( false ); break;
	case DEC: ivPtTextField->ivPtModel->decCursor( false ); break;
	}
}

MTextField::MTextFieldCursor::MTextFieldCursor( MTextField* pTextField ) :
	ivPtTextField( pTextField )
{
	ivTimer.addListener( this );
}
MTextField::MTextFieldCursor::~MTextFieldCursor()
{
	ivTimer.removeListener( this );
}
void MTextField::MTextFieldCursor::start()
{
	if( ! ivTimer.isRunnning() )
		ivTimer.startTimer( 300 );
}
void MTextField::MTextFieldCursor::stop()
{
	if( ivTimer.isRunnning() )
	{
		ivTimer.stopTimer();
		ivPtTextField->ivPtRenderer->setCursorVisible( false );
		ivPtTextField->repaint();
	}
}
void MTextField::MTextFieldCursor::onActionPerformed( void* ptSrc )
{
	ivPtTextField->ivPtRenderer->toggleCursor();
	ivPtTextField->repaint();
}

/** invoked when model changed */
void MTextField::onModelChanged( const MTextFieldModelEvent* pEvent )
{
	switch( pEvent->getType() )
	{
	case MTextFieldModelEvent::TEXT_CHANGE:
	case MTextFieldModelEvent::SELECTION_AND_CURSOR_CHANGE:
	case MTextFieldModelEvent::SELECTION_CHANGE:
	case MTextFieldModelEvent::CURSOR_CHANGE:
		{
			String text = ivPtModel->getText();
			int cursor = ivPtModel->getCursor();

			ASSERT( cursor >= 0 && cursor <= text.GetLength() );

			if( cursor < ivOffset )
				setOffset( cursor );
			else if( cursor > ivMaxPossibleCursor )
				setOffset( getOffsetForCharReverse( text, cursor, getWidth() - ivPtRenderer->getInsets().width() ) );
			else
				setOffset( ivOffset );

			//ivPtRenderer->setCursorVisible( true );
			repaint();
		}
		break;
	}
}

/** returns the textfield model */
MTextFieldModel* MTextField::getModel() const
{
	return ivPtModel;
}

/** return runtime type info */
IRtti* MTextField::getRtti() const
{
	return &gvRtti;
}

/** creates an instance */
MObject* MTextField::createInstance()
{
	return new MTextField();
}