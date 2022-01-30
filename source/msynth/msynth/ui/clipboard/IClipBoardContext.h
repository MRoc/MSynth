#ifndef __IClipBoardContext
#define __IClipBoardContext

class IClipBoardContext
{
public:

	virtual void onCopy() = 0;
	virtual void onPaste() = 0;
	virtual void onCut() = 0;
	virtual void onDelete() = 0;
};

#endif