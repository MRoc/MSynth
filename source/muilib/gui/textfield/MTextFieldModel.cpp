#include "MTextFieldModel.h"

/** constructor */
MTextFieldModelEvent::MTextFieldModelEvent( const MTextFieldModelEvent::Type type ) :
	ivType( type )
{
}

/** destructor */
MTextFieldModelEvent::~MTextFieldModelEvent()
{
}

/** returns the textfield event's type */
MTextFieldModelEvent::Type MTextFieldModelEvent::getType() const
{
	return ivType;
}


/** constructor */
MTextFieldModel::MTextFieldModel() :
	ivCursor( 0 ),
	ivSelection( 0 )
{
}

/** destructor */
MTextFieldModel::~MTextFieldModel()
{
}

/** returns the text */
String MTextFieldModel::getText() const
{
	return ivText;
}

/** returns the selected string, if any, or a empty string */
String MTextFieldModel::getSelectedText() const
{
	if( hasSelection() )
		return ivText.Mid( getSel1(), getSel2() - getSel1() );
	else
		return "";
}

/** sets the textfields text */
void MTextFieldModel::setText( const String& text )
{
	ivText = text;
	fireEvent( MTextFieldModelEvent::TEXT_CHANGE );
}

/** returns the cursor position */
int MTextFieldModel::getCursor() const
{
	return ivCursor;
}

/** sets the cursor position */
void MTextFieldModel::setCursor( const int cursor )
{
	ivCursor = cursor;
	fireEvent( MTextFieldModelEvent::CURSOR_CHANGE );
}

/** sets the cursor and selection position -> no selection anymore */
void MTextFieldModel::setCursorAndSelection( const int position )
{
	ivCursor = position;
	ivSelection = position;
	fireEvent( MTextFieldModelEvent::SELECTION_AND_CURSOR_CHANGE );
}

/** returns the selection */
int MTextFieldModel::getSelection() const
{
	return ivSelection;
}

/** sets the selection */
void MTextFieldModel::setSelection( const int selection )
{
	ivSelection = selection;
	fireEvent( MTextFieldModelEvent::SELECTION_CHANGE );
}

/** returns sel pos 1: min(cursor, selection) */
int MTextFieldModel::getSel1() const
{
	return ivCursor < ivSelection ? ivCursor : ivSelection;
}

/** returns sel pos 2: max(cursor, selection) */
int MTextFieldModel::getSel2() const
{
	return ivCursor > ivSelection ? ivCursor : ivSelection;
}

/** returns true if there is a selection: cursor==selection */
bool MTextFieldModel::hasSelection() const
{
	return ivCursor != ivSelection;
}

/** insets the given character at the current cursor position (deletes selection) */
void MTextFieldModel::insert( const char character )
{
	if( hasSelection() )
		delSelection();

	setText(
		ivText.Left( ivCursor ) +
		character +
		ivText.Right( ivText.GetLength() - ivCursor ) );

	incCursor( true );
}

/** insets the given character at the current cursor position (deletes selection) */
void MTextFieldModel::insert( const String& text )
{
	if( hasSelection() )
		delSelection();

	setText(
		ivText.Left( ivCursor ) +
		text +
		ivText.Right( ivText.GetLength() - ivCursor ) );

	incCursor( true );
}

/** deletes the selection OR the character right of the cursor */
void MTextFieldModel::del()
{
	if( hasSelection() )
		delSelection();
	else if( ivCursor < ivText.GetLength() )
		setText( ivText.Left( ivCursor ) + ivText.Right( ivText.GetLength() - (ivCursor + 1) ) );
}

/** deletes the selection OR the character left of the cursor */
void MTextFieldModel::backspace()
{
	if( hasSelection() )
		delSelection();
	else if( ivCursor > 0 )
	{
		ivText = ivText.Left( ivCursor-1 ) + ivText.Right( ivText.GetLength() - ivCursor );
		ivCursor = ivSelection = ivCursor - 1;
		fireEvent( MTextFieldModelEvent::TEXT_CHANGE );
	}
}

/** fires a textfield notify to all listeners */
void MTextFieldModel::fireEvent( MTextFieldModelEvent::Type type )
{
	MTextFieldModelEvent anEvent( type );
	ivListeners.lock();
	unsigned int count = ivListeners.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((ITextFieldModelListener*)ivListeners.getListener( i ))->onModelChanged( &anEvent );
	ivListeners.unlock();
}

/** adds a listener */
void MTextFieldModel::addListener( ITextFieldModelListener* pListener )
{
	ivListeners.addListener( pListener );
}

/** removes a listener */
void MTextFieldModel::removeListener( ITextFieldModelListener* pListener )
{
	ivListeners.removeListener( pListener );
}

/** increments cursor, flag for incrementing selection */
void MTextFieldModel::incCursor( bool setSel )
{
	if( ivCursor < ivText.GetLength() )
	{
		if( setSel )
			setCursorAndSelection( ivCursor + 1 );
		else
			setCursor( ivCursor + 1 );
	}
}

/** decrements cursor, flag for decrementing selection */
void MTextFieldModel::decCursor( bool setSel )
{
	if( ivCursor > 0 )
	{
		if( setSel )
			setCursorAndSelection( ivCursor - 1 );
		else
			setCursor( ivCursor - 1 );
	}
}

/** deletes a selection, fires notify */
void MTextFieldModel::delSelection()
{
	int min = getSel1();
	int max = getSel2();
	ivText = ivText.Left( min ) + ivText.Right( ivText.GetLength() - max );
	ivCursor = min;
	ivSelection = min;
	fireEvent( MTextFieldModelEvent::TEXT_CHANGE );
}