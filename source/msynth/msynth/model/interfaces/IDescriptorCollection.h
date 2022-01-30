/*

	(C)2000-2001 MRoc hifiShock

*/

#ifndef __MDESCRIPTORCOLLECTION
#define __MDESCRIPTORCOLLECTION

#include "IName.h"
#include "IDescriptor.h"

class IDescriptorCollection
{
	public:
		
		virtual unsigned int getMDescriptorCount() = 0;
		virtual IDescriptor *getMDescriptor( unsigned int index ) = 0;
};

#endif