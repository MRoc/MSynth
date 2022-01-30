#include "MDescriptorViewFactory.h"

MDescriptorViewFactory::MDescriptorViewFactory()
{
};

MDescriptorViewFactory::~MDescriptorViewFactory()
{
	ivRegisteredViewIds.clear();
	ivRegisteredViews.clear();
};

void MDescriptorViewFactory::registerDescriptorView(int id, CREATEDESCVIEWINSTANCE creator)
{
	ivRegisteredViewIds.push_back(id);
	ivRegisteredViews.push_back(creator);
};

MAbstractDescriptorView *MDescriptorViewFactory::getView(int id)
{
	MAbstractDescriptorView *ptView = NULL;

	for( unsigned int i=0;i<ivRegisteredViewIds.size();i++ )
	{
		if( ((int)ivRegisteredViewIds.at(i)) == id )
		{
			ptView = (MAbstractDescriptorView*) ((CREATEDESCVIEWINSTANCE)ivRegisteredViews.at(i))();
		}
	}

	return ptView;
};

int MDescriptorViewFactory::getRegisterdViewCount()
{
	return ivRegisteredViewIds.size();
};

int MDescriptorViewFactory::getRegisteredViewId(int index)
{
	return ivRegisteredViewIds.at(index);
};

MDescriptorViewFactory* MDescriptorViewFactory::getInstance()
{
	if( ivSharedInstance == NULL )
	{
		ivSharedInstance = new MDescriptorViewFactory();
	}

	return ivSharedInstance;
};

void MDescriptorViewFactory::release()
{
	SAFE_DELETE( ivSharedInstance );
};

MDescriptorViewFactory* MDescriptorViewFactory::ivSharedInstance = NULL;
