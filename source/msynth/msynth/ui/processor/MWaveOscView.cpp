/**
 * (C)2000-2003 MRoc
 */

#include "MWaveOscView.h"
#include "../../../MAppData.h"
#include <framework/resource/text/MTextResource.h>
#include <gui/dialog/MFileDialog.h>
#include <gui/dialog/MMessageBox.h>
#include <gui/MTopWnd.h>

INIT_RTTI( MWaveOscView::MSaveButtonHandler, "MWaveOscView::MSaveButtonHandler" );
INIT_RTTI( MWaveOscView::MLoadButtonHandler, "MWaveOscView::MLoadButtonHandler" );
INIT_RTTI( MWaveOscView::MReverseButtonHandler, "MWaveOscView::MReverseButtonHandler" );
INIT_RTTI( MWaveOscView::MLoopButtonHandler, "MWaveOscView::MLoopButtonHandler" );

MWaveOscView::MWaveOscView() :
	MAbstractDescriptorView(),
	ivPtSaveButtonHandler( 0 ),
	ivPtLoadButtonHandler( 0 ),
	ivPtReverseButtonHandler( 0 ),
	ivPtLoopButtonHandler( 0 ),
	ivLabelName( 0 ),
	ivPtLabelSampleName( 0 ),
	ivPtButtonLoad( 0 ),
	ivPtButtonSave( 0 ),
	ivPtButtonLoop( 0 ),
	ivPtButtonReverse( 0 ),
	ivPtView( 0 )
{
	setSize( MSize( 400, 65 ) );
	ivLayout.load( "resource/layout/MWaveOscView.xml" );
	ivPtSaveButtonHandler = new MSaveButtonHandler( this );
	ivPtLoadButtonHandler = new MLoadButtonHandler( this );
	ivPtReverseButtonHandler = new MReverseButtonHandler( this );
	ivPtLoopButtonHandler = new MLoopButtonHandler( this );
	setSize( MSize( 400, 65 ) );
} 

MWaveOscView::MWaveOscView( IDescriptor *descriptor, MColor bkColor ) :
	MAbstractDescriptorView( descriptor, bkColor ),
	ivPtSaveButtonHandler( 0 ),
	ivPtLoadButtonHandler( 0 ),
	ivPtReverseButtonHandler( 0 ),
	ivPtLoopButtonHandler( 0 ),
	ivLabelName( 0 ),
	ivPtLabelSampleName( 0 ),
	ivPtButtonLoad( 0 ),
	ivPtButtonSave( 0 ),
	ivPtButtonLoop( 0 ),
	ivPtButtonReverse( 0 ),
	ivPtView( 0 )
{
	setSize(MSize(300,12));
	setDescriptor(descriptor);
	ivPtSaveButtonHandler = new MSaveButtonHandler( this );
	ivPtLoadButtonHandler = new MLoadButtonHandler( this );
	ivPtReverseButtonHandler = new MReverseButtonHandler( this );
	ivPtLoopButtonHandler = new MLoopButtonHandler( this );
	setSize( MSize( 400, 65 ) );
}

MWaveOscView::~MWaveOscView()
{
	SAFE_DELETE( ivPtSaveButtonHandler );
	SAFE_DELETE( ivPtLoadButtonHandler );
	SAFE_DELETE( ivPtReverseButtonHandler );
	SAFE_DELETE( ivPtLoopButtonHandler );

	while( ivFloatGuis.size() > 0 )
	{
		MWnd* ptWnd = ivFloatGuis[ 0 ];
		ivFloatGuis.erase( ivFloatGuis.begin() );
		delete ptWnd;
	}
	SAFE_DELETE( ivLabelName );
	SAFE_DELETE( ivPtLabelSampleName );
	SAFE_DELETE( ivPtButtonLoad );
	SAFE_DELETE( ivPtButtonSave );
	SAFE_DELETE( ivPtButtonReverse );
	SAFE_DELETE( ivPtButtonLoop );
	SAFE_DELETE( ivPtView );
}

void MWaveOscView::doLayout()
{
	ivLayout.doLayout( this );
}

void MWaveOscView::updateFromModel()
{
	ASSERT( ivPtDescriptor != 0 );

	ivPtButtonLoop->setValue( ((MWaveOsc*)ivPtDescriptor)->getLooped() );
	ivPtButtonReverse->setValue( ((MWaveOsc*)ivPtDescriptor)->getReversed() );

	String sampleName = ((MWaveOsc*)ivPtDescriptor)->getSampleName();
	if( sampleName.GetLength() > 0 )
		ivPtLabelSampleName->setText( sampleName );
	else
		ivPtLabelSampleName->setText( "no sample" );

	ivPtView->update();
}

void MWaveOscView::setDescriptor(IDescriptor *descriptor)
{
	ivPtDescriptor = descriptor;
	ASSERT( ivPtDescriptor != 0 );

	ivHorizontalSpace = 5;
	ivVerticalSpace = 5;

	unsigned int count = ivPtDescriptor->getControlCount();
	unsigned int foundSliders = 0;

	ivLabelName = new MLabel( ivPtDescriptor->getShortName(), MColorMap::create(255,255,255), MColorMap::create(120,120,140) );
	ivLabelName->setSize( MSize(getWidth() - 2, 10));
	addChild( ivLabelName );
	ivLayout.getRoot()->registerWnd( ivLabelName, "label.name" );

	ivPtLabelSampleName = new MLabel( "", MColorMap::create(0,0,0), MColorMap::create(255,255,255) );
	ivPtLabelSampleName->setSize( MSize(70, 10) );
	addChild( ivPtLabelSampleName );
	ivLayout.getRoot()->registerWnd( ivPtLabelSampleName, "label.sample" );

	ivPtButtonLoad = new MButton(
		"load",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	ivPtButtonLoad->setSize( MSize( 33, 15 ) );
	ivPtButtonLoad->setToolTipText( MTextResource::getInstance()->getString("TT_WAVEOSC_EDITOR_LOAD_BUTTON" ) );
	addChild( ivPtButtonLoad );
	ivLayout.getRoot()->registerWnd( ivPtButtonLoad, "button.load" );

	ivPtButtonSave = new MButton(
		"save",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	ivPtButtonSave->setSize( MSize( 33, 15 ) );
	ivPtButtonSave->setToolTipText( MTextResource::getInstance()->getString("TT_WAVEOSC_EDITOR_SAVE_BUTTON" ) );
	addChild( ivPtButtonSave );
	ivLayout.getRoot()->registerWnd( ivPtButtonSave, "button.save" );

	ivPtButtonLoop = new MToggleButton(
		"loop",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	ivPtButtonLoop->setSize( MSize( 33, 15 ) );
	ivPtButtonLoop->setToolTipText( MTextResource::getInstance()->getString("TT_WAVEOSC_EDITOR_LOOP_BUTTON" ) );
	addChild( ivPtButtonLoop );
	ivLayout.getRoot()->registerWnd( ivPtButtonLoop, "button.loop" );

	ivPtButtonReverse = new MToggleButton(
		"rev",
		MColorMap::FG_COLOR3,
		MColorMap::BK_COLOR5,
		MColorMap::BK_COLOR3 );
	ivPtButtonReverse->setSize( MSize( 33, 15 ) );
	ivPtButtonReverse->setToolTipText(MTextResource::getInstance()->getString("TT_WAVEOSC_EDITOR_REV_BUTTON" ) );
	addChild( ivPtButtonReverse );
	ivLayout.getRoot()->registerWnd( ivPtButtonReverse, "button.reverse" );

	ivPtView = new MWaveView();
	ivPtView->setSize( MSize( 220, 45 ) );
	ivPtView->setWave( ((MWaveOsc*)ivPtDescriptor)->getWave() );
	addChild( ivPtView );
	ivLayout.getRoot()->registerWnd( ivPtView, "wave.view" );

	MFloatGui* ptFloatGuiVol = new MFloatGui( (MFloatControl*)(ivPtDescriptor->getControl( MWaveOsc::VOLUME )), getBkColor() );
	MFloatGui* ptFloatGuiTune = new MFloatGui( (MFloatControl*)(ivPtDescriptor->getControl( MWaveOsc::TUNE )), getBkColor() );
	addChild( ptFloatGuiVol );
	addChild( ptFloatGuiTune );
	ivFloatGuis.push_back( ptFloatGuiVol );
	ivFloatGuis.push_back( ptFloatGuiTune );
	ivLayout.getRoot()->registerWnd( ptFloatGuiVol, "slider.vol" );
	ivLayout.getRoot()->registerWnd( ptFloatGuiTune, "slider.tune" );
}

void MWaveOscView::paintBackground( IGraphics* ptGraphics, MRect &rect )
{
	ptGraphics->fillRect( 
		MRect(
			rect.getX(),
			rect.getY(),
			rect.getWidth()-1,
			rect.getHeight() - 1 ),
		ivBkColor );
	ptGraphics->drawRect( rect, MColorMap::FG_COLOR3 );
}

MAbstractDescriptorView* MWaveOscView::createInstance()
{
	return new MWaveOscView();
}

MWaveOscView::MSaveButtonHandler::MSaveButtonHandler( MWaveOscView* ptView ) :
	ivPtView( ptView )
{
	ivPtView->ivPtButtonSave->addActionListener( this );
}
MWaveOscView::MSaveButtonHandler::~MSaveButtonHandler()
{
	ivPtView->ivPtButtonSave->removeActionListener( this );
}
void MWaveOscView::MSaveButtonHandler::onActionPerformed( void* ptSrc )
{
	if( ! ((MWaveOsc*)ivPtView->ivPtDescriptor)->isValid() )
		return;

	MFileDialog dlg;
	if( dlg.showSaveDialog(
		((MTopWnd*)ivPtView->getTopParent()),
		"Save wave...",
		"Windows PCM Wave\0*.wav\0All files\0*.*\0\0") )
	{
		String fileName = dlg.getSelectedFile();
		try
		{
			((MWaveOsc*)ivPtView->ivPtDescriptor)->saveFile( fileName );
		}
		catch( MException e )
		{	
			ivPtView->ivPtLabelSampleName->setEnabled( false );
			ivPtView->ivPtLabelSampleName->setText( "no sample" );
			MMessageBox::showError( "ERROR_SAVING", fileName, e.getExceptionDescripton() );
		}
	}
}

MWaveOscView::MLoadButtonHandler::MLoadButtonHandler( MWaveOscView* ptView ) :
	ivPtView( ptView )
{
	ivPtView->ivPtButtonLoad->addActionListener( this );
}
MWaveOscView::MLoadButtonHandler::~MLoadButtonHandler()
{
	ivPtView->ivPtButtonLoad->removeActionListener( this );
}
void MWaveOscView::MLoadButtonHandler::onActionPerformed( void* ptSrc )
{
	MFileDialog dlg;
	if( dlg.showOpenDialog(
		((MTopWnd*)ivPtView->getTopParent()),
		"Open wave...",
		"Windows PCM Wave\0*.wav\0All files\0*.*\0\0") )
	{
		String fileName = dlg.getSelectedFile();
		try
		{
			((MWaveOsc*)ivPtView->ivPtDescriptor)->loadFile( fileName );
		}
		catch(MException e)
		{	
			ivPtView->ivPtLabelSampleName->setEnabled( false );
			ivPtView->ivPtLabelSampleName->setText( "no sample" );
			MMessageBox::showError( "ERROR_LOADING", fileName, e.getExceptionDescripton() );
		}
		ivPtView->updateFromModel();
	}
}

MWaveOscView::MReverseButtonHandler::MReverseButtonHandler( MWaveOscView* ptView ) :
	ivPtView( ptView )
{
	ivPtView->ivPtButtonReverse->addActionListener( this );
}
MWaveOscView::MReverseButtonHandler::~MReverseButtonHandler()
{
	ivPtView->ivPtButtonReverse->removeActionListener( this );
}
void MWaveOscView::MReverseButtonHandler::onActionPerformed( void* ptSrc )
{
	((MWaveOsc*)ivPtView->ivPtDescriptor)->setReverse( ! ((MWaveOsc*)ivPtView->ivPtDescriptor)->getReversed() );
	ivPtView->ivPtView->update();
	ivPtView->ivPtButtonReverse->setValue( ((MWaveOsc*)ivPtView->ivPtDescriptor)->getReversed() );
}

MWaveOscView::MLoopButtonHandler::MLoopButtonHandler( MWaveOscView* ptView ) :
	ivPtView( ptView )
{
	ivPtView->ivPtButtonLoop->addActionListener( this );
}
MWaveOscView::MLoopButtonHandler::~MLoopButtonHandler()
{
	ivPtView->ivPtButtonLoop->removeActionListener( this );
}
void MWaveOscView::MLoopButtonHandler::onActionPerformed( void* ptSrc )
{
	((MWaveOsc*)ivPtView->ivPtDescriptor)->setLooped( ! ((MWaveOsc*)ivPtView->ivPtDescriptor)->getLooped() );
	ivPtView->ivPtButtonLoop->setValue( ((MWaveOsc*)ivPtView->ivPtDescriptor)->getLooped() );
}