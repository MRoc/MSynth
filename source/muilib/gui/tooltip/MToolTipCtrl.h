#ifndef __MTOOLTIPCTRL
#define __MTOOLTIPCTRL

#include "MToolTipWnd.h"
#include <framework/MUtil.h>

class MToolTipCtrl
{
	protected:

		static MToolTipCtrl*	gvInstance;
		MToolTipWnd*			ptToolTipWnd;

		MToolTipCtrl();

	public:

		~MToolTipCtrl();

		void showToolTipInfo( MWnd* ptWnd, String text );
		void showToolTipInfo( MWnd* ptWnd, String text, int x, int y );
		void hideToolTipInfo();

		static MToolTipCtrl* getInstance();
		static void release();
};

#endif