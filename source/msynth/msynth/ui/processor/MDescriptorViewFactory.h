/*(C)2001 MRoc hifiShock*/

#ifndef __MDescriptorViewFactory
#define __MDescriptorViewFactory

#include <framework/MTypes.h>
#include "MAbstractDescriptorView.h"

typedef MAbstractDescriptorView* (*CREATEDESCVIEWINSTANCE)();

class MDescriptorViewFactory
{
	protected:

		std::vector<CREATEDESCVIEWINSTANCE> ivRegisteredViews;
		std::vector<int> ivRegisteredViewIds;

		static MDescriptorViewFactory* ivSharedInstance;

	public:

		MDescriptorViewFactory();

		~MDescriptorViewFactory();

		void registerDescriptorView(int id, CREATEDESCVIEWINSTANCE creator);

		MAbstractDescriptorView *getView(int id);

		int getRegisterdViewCount();
		int getRegisteredViewId(int index);

		static MDescriptorViewFactory* getInstance();
		static void release();
};

#endif