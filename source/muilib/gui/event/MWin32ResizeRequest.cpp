#include "MWin32ResizeRequest.h"


MWin32ResizeRequest::MWin32ResizeRequest( MINMAXINFO* info ) :
	ivNativeInfo( info )
{
}

MWin32ResizeRequest::~MWin32ResizeRequest()
{
}

void MWin32ResizeRequest::setMax( MSize max )
{
	int maxW = max.getWidth();
	int maxH = max.getHeight();
	if( maxW < ivNativeInfo->ptMaxSize.x )
		ivNativeInfo->ptMaxSize.x = maxW;
	if( maxH < ivNativeInfo->ptMaxSize.y )
		ivNativeInfo->ptMaxSize.y = maxH;
	if( maxW < ivNativeInfo->ptMaxTrackSize.x )
		ivNativeInfo->ptMaxTrackSize.x = maxW;
	if( maxH < ivNativeInfo->ptMaxTrackSize.y )
		ivNativeInfo->ptMaxTrackSize.y = maxH;
}

void MWin32ResizeRequest::setMin( MSize min )
{
	ivNativeInfo->ptMinTrackSize.x = min.getWidth();
	ivNativeInfo->ptMinTrackSize.y = min.getHeight();
}

MSize MWin32ResizeRequest::getMax()
{
	return MSize(
		ivNativeInfo->ptMaxSize.x,
		ivNativeInfo->ptMaxSize.y );
}

MSize MWin32ResizeRequest::getMin()
{
	return MSize(
		ivNativeInfo->ptMinTrackSize.x,
		ivNativeInfo->ptMinTrackSize.y );
}