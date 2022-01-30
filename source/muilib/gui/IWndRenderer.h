#ifndef __IWndRenderer
#define __IWndRenderer

#include "MWnd.h"

/**
 * a renderer is used for painting a window,
 * delegating the paining in a window to a renderer.
 */
class IWndRenderer :
	public IMUnknown
{
public:

	/** destructor */
	virtual ~IWndRenderer(){}

	/** does the paining for the given compoent */
	virtual void paint( MWnd* pWnd, IGraphics* pG, const MRect& rect ) = 0;
};

#endif