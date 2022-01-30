#ifndef __MOkButton
#define __MOkButton

#include "MButton.h"

class MOkButton :
	public MButton
{
protected:

	MTopWnd* ivPtToDelete;

public:

	MOkButton( String text, MColor textColor, MColor disabledColor, MColor bkColor );
	virtual ~MOkButton();

	virtual void	setOnDelete( MTopWnd* pWnd );
	virtual void	onMouseUp( MMouseEvent* ptEvent );
};

#endif