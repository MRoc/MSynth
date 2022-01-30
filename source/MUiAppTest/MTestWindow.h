#ifndef __MTestWindow
#define __MTestWindow

#include <framework/listener/IActionListener.h>
#include <gui/framewnd/MFrameWnd.h>
#include <gui/event/MMouseEvent.h>
#include <gui/button/MButton.h>

class MTestWindow :
	public MFrameWnd,
	public IActionListener
{
protected:

	MButton* pButton2;
	MButton* pButton3;

public:

	/** constructor */
	MTestWindow();

	/** destructor */
	virtual ~MTestWindow();

	/** creates the native window */
	virtual bool create( MRect rect, MTopWnd* pParent );

	/** paints the windows background */
	virtual void paint( IGraphics* pG, const MRect& rect );

	/** invoked on some button clicks */
	virtual void onActionPerformed( void* ptSrc );

	/** query for interface */
	virtual void* getInterface( const String& className ) const;
};

#endif