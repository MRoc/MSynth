/**
 *(C)2000-2003 MRoc
 */

#include "MButton.h"
#include "MArrowButtonRenderer.h"
#include "MTransparentBitmapButtonRenderer.h"
#include "MTextButtonRenderer.h"
#include <framework/io/MLogger.h>
#include <framework/treedocument/MTreeDocumentUtils.h>

/** the runtime type info */
MRtti MButton::gvRtti = MRtti( "MButton", MButton::createInstance );

/** constructor */
MButton::MButton() :
	MyInteractiveWnd(),
	ivPtRenderer( 0 )
{
}

/** constructor with renderer */
MButton::MButton( IWndRenderer* pRenderer ) :
	MyInteractiveWnd(),
	ivPtRenderer( pRenderer )
{
}

/** constructor for icon button */
MButton::MButton( unsigned int bmIdUnpressed, unsigned int bmIdPressed, MColor tColor1, MColor tColor2, MColor bkColor ) :
	MyInteractiveWnd(),
	ivPtRenderer( 0 )
{
	ivPtRenderer =
		new MTransparentBitmapButtonRenderer(
			bmIdPressed,
			bmIdUnpressed,
			tColor2,
			tColor1  );

	ivBkColor = bkColor;
}

/** constructor for text button */
MButton::MButton( String text, MColor textColor, MColor disabledColor, MColor bkColor ) :
	MyInteractiveWnd(),
	ivPtRenderer( 0 )
{
	ivPtRenderer = 
		new MTextButtonRenderer(
			text,
			textColor,
			disabledColor );

	ivBkColor = bkColor;
}

/** constructor for arrow button */
MButton::MButton( MArrowRenderer::Direction direction, MColor bkColor ) :
	MyInteractiveWnd(),
	ivPtRenderer( 0 )
{
	ivPtRenderer = new MArrowButtonRenderer( direction );
	ivBkColor = bkColor;
}

/** destructor */
MButton::~MButton()
{
	ivButtonListener.clear();
	ivActionListener.clear();
	SAFE_DELETE( ivPtRenderer );
}

/** paints the button */
void MButton::paint( IGraphics* ptGraphics, const MRect &rect )
{
	if( ivPtRenderer )
		ivPtRenderer->paint( this, ptGraphics, rect );
	else
		MyInteractiveWnd::paint( ptGraphics, rect );
	/*if( getShowingTooltip() )
		paintHighLight( ptGraphics, rect, RGB( 255, 255, 0 ) );*/
}

/** invoked when mouse button pressed */
void MButton::onMouseDown( MMouseEvent* ptEvent )
{
	//TRACE( "MButton::onMouseDown: %s\n", ptEvent->getPoint().toString().getData() );
	MyInteractiveWnd::onMouseDown( ptEvent );
	fireButtonPressed();
}

/** invoked when mouse button released */
void MButton::onMouseUp( MMouseEvent* ptEvent )
{
//	TRACE( "MButton::onMouseUp\n" );
	if( getEnabled() &&
		getMouseDown( ptEvent->getButton() ) &&
		getMouseIn() )
	{
		clicked();
		fireActionPerformed();
	}
	MyInteractiveWnd::onMouseUp( ptEvent );
	fireButtonReleased();
}

/** invoked when the button was clicked, to be overwritten */
void MButton::clicked()
{
}

/** adds a actionlistener */
void MButton::addActionListener( IActionListener* ptListener )
{
	ivActionListener.addListener( ptListener );
}

/** removes a actionlistener */
void MButton::removeActionListener( IActionListener* ptListener )
{
	ivActionListener.removeListener( ptListener );
}

/** adds a buttonlistener */
void MButton::addButtonListener( IButtonListener* ptListener )
{
	ivButtonListener.addListener( ptListener );
}

/** removes a buttonlistener */
void MButton::removeButtonListener( IButtonListener* ptListener )
{
	ivButtonListener.removeListener( ptListener );
}

/** fires a action notify */
void MButton::fireActionPerformed()
{
	unsigned int count = ivActionListener.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IActionListener*)ivActionListener.getListener(i))->onActionPerformed( (void*) this );
}

/** fires a button pressed notify */
void MButton::fireButtonPressed()
{
	unsigned int count = ivButtonListener.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IButtonListener*)ivButtonListener.getListener(i))->onButtonPressed( this );
}

/** fires a button released notify */
void MButton::fireButtonReleased()
{
	unsigned int count = ivButtonListener.getListenerCount();
	for( unsigned int i=0;i<count;i++ )
		((IButtonListener*)ivButtonListener.getListener(i))->onButtonReleased( this );
}

/** if this method returns true, the renderer paints a sunken button */
bool MButton::isSunken()
{
	return getMouseDown( MMouseEvent::BUTTON1 ) && getMouseIn();
}

/** query superclass */
void* MButton::getInterface( const String& className ) const
{
	if( className == "MButton" )
		return (void*)((MButton*)this);
	else 
		return MyInteractiveWnd::getInterface( className );
}

/** returns the runtime type info */
IRtti* MButton::getRtti() const
{
	return &gvRtti;
}

/** loads from the given tree node */
void MButton::load( MTreeNode* ptNode )
{
	MyInteractiveWnd::load( ptNode );
	MTreeNode* pRendererNode = ptNode->getFirstNode( ELEM_OBJECT );
	if( pRendererNode )
	{
		MObject* pObj = MTreeDocumentUtils::createObject( pRendererNode );
		if( pObj )
		{
			ivPtRenderer = (IWndRenderer*) pObj->getInterface( "IWndRenderer" );
			if( ivPtRenderer == 0 )
			{
				ivPtRenderer = new MTextButtonRenderer( "loading renderer1 failed", MColorMap::BLACK, MColorMap::GRAY );
				delete pObj;
				MLogger::logWarning( " MButton::load: loading renderer class\"%s\" failed", pRendererNode->getAttribute( ATTR_CLASS ).getData() );
			}
		}
		else
		{
			ivPtRenderer = new MTextButtonRenderer( "loading renderer failed2", MColorMap::BLACK, MColorMap::GRAY );
			MLogger::logWarning( " MButton::load: loading renderer class\"%s\" failed", pRendererNode->getAttribute( ATTR_CLASS ).getData() );
		}
	}
	else
		ivPtRenderer = new MTextButtonRenderer( "text", MColorMap::BLACK, MColorMap::GRAY );
}

/** stores as tree node */
MTreeNode* MButton::save()
{
	MTreeNode* pBack = MyInteractiveWnd::save();

	if( ivPtRenderer && ivPtRenderer->getInterface( "ISerializeable" ) )
		pBack->addChild( ((ISerializeable*)ivPtRenderer->getInterface( "ISerializeable" ))->save() );

	return pBack;
}

/** creates an instance */
MObject* MButton::createInstance()
{
	return new MButton();
}

/** returns the renderer */
IWndRenderer* MButton::getRenderer()
{
	return this->ivPtRenderer;
}