/*

	MSplitBar (C)2000/2001 MRoc hifiShock

*/
#ifndef __MSPLITBAR
#define __MSPLITBAR

#include <framework/listener/MActionListenerList.h>
#include "../MWnd.h"

class MSplitBar :
	public MWnd
{
public:

	enum Mode
	{
		HORIZONTAL,
		VERTICAL
	};

protected:

	Mode					ivMode;
	MColor					ivFgColor;
	bool					ivIsSelected;
	MActionListenerList		ivListenerList;

public:

	MSplitBar( Mode mode );
	virtual ~MSplitBar();
	
	virtual void	paint( IGraphics* ptGraphics, const MRect &rect );

	void			setSelected( bool value );
	bool			getSelected();

	void			setFgColor( MColor color );
	MColor			getFgColor();

	void			addActionListener( IActionListener* ptListener );
	void			removeActionListener( IActionListener* ptListener );

protected:

	virtual bool	processEvent( IUiEvent* pEvent );

	virtual bool	onSetCursor();
	void			fireActionPerformed( void* ptSrc );
};

#endif