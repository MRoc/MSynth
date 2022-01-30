/*

	MClipBoard (C)2001 MRoc hifiShock

*/

#ifndef __MClipBoard
#define __MClipBoard

#include "ICopyCutPaste.h"
#include "IClipBoardContext.h"

class MClipBoardContext
{
protected:

	ICopyCutPaste*			ivUserObject;

public:

	MClipBoardContext( ICopyCutPaste* userObject )
	{
		ivUserObject = userObject;
	};

	virtual ~MClipBoardContext()
	{
		if( ivUserObject )
			ivUserObject->release();
	};

	virtual ICopyCutPaste* getUserObject()
	{
		return ivUserObject;
	};
};

class MClipBoard
{
protected:

	static MClipBoard*		ivInstance;
	MClipBoardContext*		ivContent;
	IClipBoardContext*		ivPtContext;

public:

	MClipBoard();
	~MClipBoard();

	MClipBoardContext*		getContent();

	void					setContent( ICopyCutPaste* content );
	void					setContent( MClipBoardContext* content );
	void					deleteContent();
	bool					hasContent();

	void					setContext( IClipBoardContext* pTarget );
	void					unsetContext( IClipBoardContext* pTarget );
	IClipBoardContext*		getContext();

	static MClipBoard*		getInstance();
	static void				release();
};

#endif