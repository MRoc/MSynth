#include "MAudioTriggerControl.h"

#include <gui/MGuiUtils.h>

MAudioTriggerControl::MAudioTriggerControl( MAudioTrigger* ptTrigger ) :
	MDCControl(),
	ivPtTrigger( (MAudioTrigger*) ptTrigger ),
	ivPeaksPerPixel( 0 ),
	ivStepWidthInPixel( 30 )
{
}

MAudioTriggerControl::~MAudioTriggerControl()
{
}

void MAudioTriggerControl::setPeaksPerPixel( FP peaksPerPixel )
{
	this->ivPeaksPerPixel = peaksPerPixel;
}

FP MAudioTriggerControl::getPeaksPerPixel()
{
	return ivPeaksPerPixel;
}

void MAudioTriggerControl::setStepWidthInPixel( unsigned int width )
{
	ivStepWidthInPixel = width;
}

unsigned int MAudioTriggerControl::getStepWidthOnPixel()
{
	return ivStepWidthInPixel;
}

void MAudioTriggerControl::setTrigger( MAudioTrigger *ptTrigger )
{
	ivPtTrigger = ptTrigger;
}

MDefaultNote* MAudioTriggerControl::getTrigger()
{
	return ivPtTrigger;
}

void MAudioTriggerControl::paintPeak( IGraphics* pDC, unsigned char* ptPeaks, unsigned int len, MRect &rect )
{
	unsigned int xOffset = rect.getX();
	unsigned int yOffset = rect.getY();

	int halfHeight = rect.getHeight() / 2;
	int peak = 0;

	unsigned int count = rect.getWidth(); 
	for( unsigned int i=0;i<count;i++ )
	{
		FP pos = this->ivPeaksPerPixel * i * 4;
		unsigned int posI1 = (unsigned int) pos;
		FP offset = pos-posI1;
		if( posI1 < len-4 )
		{
			float max1 = ((float)(signed char)ptPeaks[ posI1 ]) / 127.0f;
			float min1 = ((float)(signed char)ptPeaks[ posI1 + 1 ]) / 127.0f;
			float max2 = ((float)(signed char)ptPeaks[ posI1 + 2 ]) / 127.0f;
			float min2 = ((float)(signed char)ptPeaks[ posI1 + 3 ]) / 127.0f;

			float max = ((max2-max1) * (offset) + max1); // linear interpolate
			float min = ((min2-min1) * (offset) + min1); // linear interpolate

			int stepy1 = (int) (yOffset + halfHeight - halfHeight * max);
			int stepy2 = (int) (yOffset + halfHeight - halfHeight * min);

			int x = xOffset + i;

			pDC->drawLine( x, stepy1, x, stepy2, MColorMap::FG_COLOR3 );
		}
	}
}

void MAudioTriggerControl::paintControl( IGraphics* pDC )
{
	ASSERT( ivPtTrigger != 0 );

	MColor bkColor, fgColor;
	MColor color = MColorMap::BK_COLOR4;
	MColor colorSel = getBkColorSelected();

	if( getSelected() )
	{
		fgColor = color;
		bkColor = colorSel;
	}
	else
	{
		fgColor = colorSel;
		bkColor = color;
	}
	MGuiUtils::paintRaisedBorder(
		pDC,
		MRect( ivRect.getX(), ivRect.getY(), ivRect.getWidth(), ivRect.getHeight()-2),
		bkColor );

	MPeakFile* ptPeak = ivPtTrigger->getFile()->getPeakFile();

	unsigned int channels = ptPeak->getChannelCount();

	int channelHeight = ivRect.getHeight() / channels;
	for( unsigned int i=0;i<channels;i++ )
		paintPeak(
			pDC,
			ptPeak->getData( i )->getData(),
			ptPeak->getData( i )->getFill(),
			MRect(
					ivRect.getX(),
					ivRect.getY() + (channelHeight*i),
					ivRect.getWidth(),
					channelHeight
				));

	//pDC->drawRect( ivRect, fgColor );
}