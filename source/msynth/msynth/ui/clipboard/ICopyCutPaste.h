#ifndef __MCopyCutPaste
#define __MCopyCutPaste

class ICopyCutPaste
{
	public:

		virtual int					getTypeId() = 0;

		virtual	void				release() = 0;
		virtual ICopyCutPaste*		getCopy() = 0;

		virtual char*				getUserTypeName() = 0;
};
#define TYPE_BAR 1;
#define TYPE_NOTE 2;

#endif
