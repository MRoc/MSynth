#ifndef __MScrolledFrameWnd
#define __MScrolledFrameWnd

#include "MFrameWnd.h"

class MScrolledFrameWnd :
	public MFrameWnd
{
public:

	MScrolledFrameWnd();
	MScrolledFrameWnd( MWnd* root );
	virtual ~MScrolledFrameWnd();
	virtual void doLayout();

	virtual void setScrollWnd( MWnd* pWnd );
};

#endif