#ifndef __MTextFieldModel
#define __MTextFieldModel

#include <framework/MTypes.h>
#include <framework/listener/MListenerList.h>

/**
 * a event sent by a textfield
 */
class MTextFieldModelEvent
{
public:

	enum Type
	{
		TEXT_CHANGE,
		CURSOR_CHANGE,
		SELECTION_CHANGE,
		SELECTION_AND_CURSOR_CHANGE
	};

protected:

	/** the textfield event type */
	Type ivType;

public:

	/** constructor */
	MTextFieldModelEvent( const Type type );

	/** destructor */
	virtual ~MTextFieldModelEvent();

	/** returns the textfield event's type */
	virtual Type getType() const;
};

/** listener interface for beeing notified 'bout changes in model */
class ITextFieldModelListener :
	public IListener
{
public:

	/** invoked when model changed */
	virtual void onModelChanged( const MTextFieldModelEvent* pEvent ) = 0;
};

/**
 * the model of a textfield
 */
class MTextFieldModel
{
protected:

	/** the text */
	String ivText;

	/** the cursor position */
	int ivCursor;

	/** the selection position (has selection if sel!=cursor */
	int ivSelection;

	/** list of listeners */
	MListenerList ivListeners;

public:

	/** constructor */
	MTextFieldModel();

	/** destructor */
	virtual ~MTextFieldModel();

	/** returns the text */
	virtual String getText() const;

	/** returns the selected string, if any, or a empty string */
	virtual String getSelectedText() const;

	/** sets the textfields text */
	virtual void setText( const String& text );

	/** returns the cursor position */
	virtual int getCursor() const;

	/** sets the cursor position */
	virtual void setCursor( const int cursor );

	/** sets the cursor and selection position -> no selection anymore */
	virtual void setCursorAndSelection( const int position );

	/** returns the selection */
	virtual int getSelection() const;

	/** sets the selection */
	virtual void setSelection( const int selection );

	/** returns sel pos 1: min(cursor, selection) */
	virtual int getSel1() const;

	/** returns sel pos 2: max(cursor, selection) */
	virtual int getSel2() const;

	/** returns true if there is a selection: cursor==selection */
	virtual bool hasSelection() const;


	/** insets the given character at the current cursor position (deletes selection) */
	virtual void insert( const char character );

	/** insets the given character at the current cursor position (deletes selection) */
	virtual void insert( const String& text );

	/** deletes the selection OR the character right of the cursor */
	virtual void del();

	/** deletes the selection OR the character left of the cursor */
	virtual void backspace();

	/** adds a listener */
	virtual void addListener( ITextFieldModelListener* pListener );

	/** removes a listener */
	virtual void removeListener( ITextFieldModelListener* pListener );

	/** increments cursor, flag for incrementing selection */
	void incCursor( bool setSel );

	/** decrements cursor, flag for decrementing selection */
	void decCursor( bool setSel );

private:

	/** deletes a selection, fires notify */
	void delSelection();

	/** fires a textfield notify to all listeners */
	void fireEvent( MTextFieldModelEvent::Type type );
};

#endif