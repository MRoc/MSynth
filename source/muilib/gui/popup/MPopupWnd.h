#ifndef __MPopupWnd
#define __MPopupWnd

#include <framework/MTypes.h>
#include <framework/listener/MListenerList.h>
#include <framework/primitive/MSize.h>
#include "../MTopWnd.h"

/**
 * defines a popup listener to be informed about
 * when the popup hides itself.
 */
class IPopupListener :
	public IListener
{
public:

	/** invoked when the popup hides */
	virtual void onPopupChanged( void* ptSrc, bool visible ) = 0;
};

/**
 * defines a popup window.
 * a popupwindow hides when beeing deactivated
 */
class MPopupWnd :
	public MTopWnd
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** list of popup listeners */
	MListenerList ivListeners;

public:

	/** constructor */
	MPopupWnd();

	/** destructor */
	virtual ~MPopupWnd();

	/** creates the window */
	virtual bool create( MRect rect, MTopWnd* ptParent );

	/** paints the content */
	virtual void paint( IGraphics* ptDc, const MRect &rect );

	/** adds a popup listener */
	virtual void addListener( IPopupListener* ptListener );

	/** removes a popup listener */
	virtual void removeListener( IPopupListener* ptListener );

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** returns an instance of this class */
	static MObject* createInstance();

protected:

	virtual bool processEvent( IUiEvent* pEvent );

	/** fires a notification when the popup changed */
	void firePopupChanged( bool visible );

};

#endif