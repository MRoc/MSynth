#ifndef __MSimpleScrollWnd
#define __MSimpleScrollWnd
#include "../MyInteractiveWnd.h"

class MSimpleScrollWnd :
	public MyInteractiveWnd
{
public:

	MSimpleScrollWnd();
	virtual ~MSimpleScrollWnd();

	virtual void	setScrollOffset( int offset ) = 0;
	virtual int		getScrollOffset() = 0;
};

#endif