#ifndef __MNativeTypes
#define __MNativeTypes

	#ifdef _MFC
		#include <afxwin.h>
		#include <afxdlgs.h>
		#include <afxcmn.h>
	#else
		#include <windows.h>
		#include <winuser.h>
		#include <commctrl.h>
	#endif

	#define SAFE_RELEASE(p)\
		if(p){\
			(p)->Release();\
			(p)=NULL;\
		}

	#define SAFE_GDI_DELETE(p)\
		if(p){\
			(p)->DeleteObject();\
			delete (p);\
			(p)=NULL;\
		}

#endif