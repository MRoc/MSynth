/**
 * (C)2000-2003 MRoc
 */

#ifndef __MABSTRACTDESCRIPTORVIEW
#define __MABSTRACTDESCRIPTORVIEW

#include <gui/MWndCollection.h>
#include "../../../resource.h"
#include "../../model/interfaces/IDescriptor.h"

class MAbstractDescriptorView :
	public MWndCollection
{
protected:

	IDescriptor *ivPtDescriptor;

public:

	MAbstractDescriptorView() :
		MWndCollection(),
		ivPtDescriptor( 0 )
	{
		ivBkColor = MColorMap::create(255,100,255);
	}

	MAbstractDescriptorView( IDescriptor *descriptor, MColor bkColor ) :
		MWndCollection(),
		ivPtDescriptor( descriptor )
	{
		ivBkColor = bkColor;
	}

	virtual ~MAbstractDescriptorView()
	{
	}

	virtual void setDescriptor(IDescriptor *descriptor)
	{
		ivPtDescriptor = descriptor;
	}

	virtual IDescriptor *getDescriptor()
	{
		return ivPtDescriptor;
	}

	virtual void updateFromModel()
	{
	}
};

#endif

