#ifndef __MTextField
#define __MTextField

#include <framework/primitive/MInsets.h>
#include "../MyInteractiveWnd.h"
#include "../timer/MTimer.h"
#include "MTextFieldModel.h"

class MTextFieldRenderer;

/**
 * offset - is the first visible character in textfield
 *
 * visiblecharacters - is the number of visible characters, the last one can be 
 * e.g. clipped, depends on offset
 *
 * maxpossiblecursor - depends on visible character. if last visible character
 * is clipped, the maxpossiblecursor is length - 1, else its the length.
 *
 */
class MTextField :
	public MyInteractiveWnd,
	public ITextFieldModelListener
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the text offset in characters, minus=left plus=right */
	int ivOffset;

	/** the number of character currently visible, depends on offset and width */
	int ivVisibleCharacters;

	/** the maximum cursor position dependent from visible characters. */
	int ivMaxPossibleCursor;

	/** the shift down flag */
	static const unsigned int SHIFT_DOWN;

	/** a textfield renderer delegate for rendering */
	MTextFieldRenderer* ivPtRenderer;

	/** a textfield model included text, cursor and selection */
	MTextFieldModel* ivPtModel;

public:

	/** constructor */
	MTextField();

	/** destructor */
	virtual ~MTextField();

	/** paints the textfield */
	virtual void paint( IGraphics* pG, const MRect& rect );

	/** process fw event */
	virtual bool processEvent( IUiEvent* pEvent );

	/** query interface */
	virtual void* getInterface( const String& className ) const;

	/** return runtime type info */
	virtual IRtti* getRtti() const;

	/** sets the window's rect, update visible chars */
	virtual void setRect( const MRect& rect );

	/** sets the window's size, update visible chars */
	virtual void setSize( const MSize& size );

	/** returns the textfield's text */
	virtual String getText() const;

	/** sets the textfield's text */
	virtual void setText( const String& text ) const;

	/** returns the substring of text from ivOffset to end */ 
	virtual String getTextToDraw() const;

	/** returns the text from offset to cursor */
	virtual String getTextToCursor() const;

	/** returns the text rect (window rect minus renderer insets) */
	virtual MRect getTextRect() const;

	/** returns the current text offset in characters */
	virtual int getOffset() const;

	/** returns the number of visible characters */
	virtual int getVisibleCharacters() const;

	/** invoked when model changed */
	virtual void onModelChanged( const MTextFieldModelEvent* pEvent );

	/** returns the textfield model */
	virtual MTextFieldModel* getModel() const;

	/** creates an instance */
	static MObject* createInstance();

private:

	/** sets the shift down flag */
	void setShiftDown( bool value );

	/** returns the shift down flag */
	bool getShiftDown();

protected:

	/** returns the index of the character hidden by x in the textfield */
	virtual int getHitIndex( const int x ) const;

	/** returns the last character's index not clipped by x */
	virtual int getIndexNotClipped( const String& text, const int x ) const;

	/** returns the offset of the first character fitting in width when indexed is the last one */
	virtual int getOffsetForCharReverse( const String& text, const int index, const int width ) const;

	/** returns width of given text */
	virtual int getTextWidth( const String& text ) const;

	virtual void onMouseUp( MMouseEvent* pEvent );
	virtual void onMouseDown( MMouseEvent* pEvent );
	virtual void onMouseMove( MMouseEvent* pEvent );

	virtual void onKeyDown( MKeyEvent* pEvent );
	virtual void onKeyUp( MKeyEvent* pEvent );

private:

	void setOffset( int offset );

	class MTextFieldCursor :
		public IActionListener
	{
	protected:
		MTimer ivTimer;
		MTextField* ivPtTextField;
	public:
		MTextFieldCursor( MTextField* pTextField );
		virtual ~MTextFieldCursor();
		virtual void start();
		virtual void stop();
		virtual void onActionPerformed( void* ptSrc );
		virtual void* getInterface( const String& className ) const{ return 0; }
	};

	MTextFieldCursor* ivPtCursor;

	class MAutoSelection :
		public IActionListener
	{
	public:
		enum Direction{
			INC,
			DEC
		};
	protected:
		Direction ivDirection;
		MTimer ivTimer;
		MTextField* ivPtTextField;
	public:
		MAutoSelection( MTextField* pOwner );
		virtual ~MAutoSelection();
		virtual void start( Direction direction );
		virtual void stop();
		virtual bool isOn();
		virtual void onActionPerformed( void* ptSrc );
		virtual void* getInterface( const String& className ) const{ return 0; }
	};

	MAutoSelection* ivPtSelectionTimer;
};

#endif