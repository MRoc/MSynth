#include "MTestWindow.h"

#include <gui/MUiGraphics.h>
#include <gui/event/MMouseEvent.h>
#include <gui/MWndCollection.h>
#include <framework/performance/MClock.h>
#include <framework/io/MLogger.h>
#include <framework/io/MFilePrinter.h>
#include <framework/primitive/MObjString.h>
#include <framework/treedocument/MTreeDocumentUtils.h>
#include <gui/MWndUtils.h>
#include <gui/button/MFlatTextButtonRenderer.h>
#include <gui/button/MCheckBoxRenderer.h>
#include <gui/button/MRadioButtonRenderer.h>
#include <gui/button/MToggleButton.h>
#include <gui/label/MLabel.h>
#include <gui/progress/MProgressControl.h>
#include <gui/counter/MCounter.h>
#include <gui/counter/MCountControl.h>
#include <gui/scrollbar/MScrollBar.h>
#include <gui/net/MLink.h>
#include <gui/list/MListView.h>
#include <gui/list/MScrolledListView.h>
#include <gui/combobox/MComboBox.h>
#include <gui/textfield/MTextField.h>
#include <gui/scrollbar/MScrollPane.h>
#include <gui/dialog/MDialog.h>
#include "MTestDialog.h"
#include <gui/dialog/MFileDialog.h>
#include <gui/win32impl/MTopWndWin32Impl.h>

MTestWindow::MTestWindow():
	pButton2( 0 ),
	pButton3( 0 )
{
	/*MWndCollection* pWndB = new MWndCollection();
	pWndB->setRect( MRect( 40, 40, 70, 80 ) );
	pWndB->setScrollPos( MPoint( 0, 30 ) );
	addChild( pWndB );

	MWndCollection* pWndC = new MWndCollection();
	pWndC->setRect( MRect( 10, 10, 40, 70 ) );
	pWndC->setScrollPos( MPoint( 0, 20 ) );
	pWndB->addChild( pWndC );

	MWndCollection* pWndD = new MWndCollection();
	pWndD->setRect( MRect( 10, 5, 20, 20 ) );
	pWndD->setScrollPos( MPoint( 0, 0 ) );
	pWndC->addChild( pWndD );

	MWndCollection* pWndE = new MWndCollection();
	pWndE->setRect( MRect( 10, 30, 20, 20 ) );
	pWndE->setScrollPos( MPoint( 0, 0 ) );
	pWndC->addChild( pWndE );

	MWndCollection* pWndF = new MWndCollection();
	pWndF->setRect( MRect( 10, 60, 20, 20 ) );
	pWndF->setScrollPos( MPoint( 0, 0 ) );
	pWndC->addChild( pWndF );

	MWndCollection* pWndG = new MWndCollection();
	pWndG->setRect( MRect( 10, 90, 20, 20 ) );
	pWndG->setScrollPos( MPoint( 0, 0 ) );
	pWndC->addChild( pWndG );*/

	ivBkColor = MColorMap::create( 180, 180, 210 );

	MWndCollection* ptRoot = this;

	// test button "MTextButton"
	MButton* pButton = new MToggleButton( new MFlatTextButtonRenderer( "MTextButton", MColorMap::create( 64, 64, 128 ) ) );
	pButton->setBkColor( ivBkColor );
	pButton->setRect( MRect( 10, 10, 100, 30 ) );
	ptRoot->addChild( pButton );

	// test button "M3DButton"
	pButton2 =
		new MButton(
			"M3DButton/text",
			MColorMap::create( 32, 32, 64 ),
			MColorMap::create( 64, 64, 64 ),
			ivBkColor );
	pButton2->setRect( MRect( 10, 50, 100, 30 ));
	ptRoot->addChild( pButton2 );
	pButton2->addActionListener( this );

	// test button "M3DButton" arrowed
	pButton3 =
		new MButton(
			MArrowRenderer::RIGHT,
			ivBkColor );
	pButton3->setRect( MRect( 10, 90, 100, 30 ));
	ptRoot->addChild( pButton3 );
	pButton3->addActionListener( this );

	// test button "M3DButton" arrowed
	MToggleButton* pButton4 =
		new MToggleButton(
			"M3DToggleButton",
			MColorMap::create( 32, 32, 64 ),
			MColorMap::create( 64, 64, 64 ),
			ivBkColor );
	pButton4->setRect( MRect( 10, 130, 100, 30 ));
	ptRoot->addChild( pButton4 );

	// test button "MLabel"
	MLabel* pLabel =
		new MLabel(
			"MLabel",
			MColorMap::create( 32, 32, 64 ),
			ivBkColor );
	pLabel->setRect( MRect( 10, 170, 100, 30 ));
	ptRoot->addChild( pLabel );

	// test button "MProgressControl"
	MProgressControl* pProgress = new MProgressControl();
	pProgress->onProgress( 0.3f );
	pProgress->setRect( MRect( 10, 210, 100, 30 ));
	ptRoot->addChild( pProgress );

	// test "MCountControl"
	MCounter* pCounter = new MCounter( 3 );
	pCounter->setBkColor( ivBkColor );
	pCounter->setValue( 234 );
	pCounter->setRect( MRect( 10, 250, 100, 30 ) );
	ptRoot->addChild( pCounter );

	// test "MCountControl"
	MCountControl* pCountControl = new MCountControl(
		3,
		0,
		999,
		ivBkColor,
		MColorMap::create( 32, 32, 64 ),
		"count",
		40 );
	pCountControl->setRect( MRect( 10, 290, 100, 30 ) );
	ptRoot->addChild( pCountControl );

	// scrollbar
	MScrollBar* pScrollBar = new MScrollBar( MScrollBar::HORIZONTAL );
	pScrollBar->setRect( MRect( 10, 330, 100, 30 ) );
	ptRoot->addChild( pScrollBar );

	// link
	MLink* pLink = new MLink( "http://www.msynth.com" );
	pLink->setRect( MRect( 10, 370, 320, 30 ) );
	pLink->setBkColor( ivBkColor );
	ptRoot->addChild( pLink );

	// listview
	MListView* pListView = new MListView();
	pListView->getModel()->add( new MObjString( "HELLO1" ) );
	pListView->getModel()->add( new MObjString( "HELLO2" ) );
	pListView->getModel()->add( new MObjString( "HELLO3" ) );
	pListView->setRect( MRect( 120, 10, 100, 110 ) );
	pListView->setBkColor( ivBkColor );
	pListView->setPreSelectionEnabled( false );
	ptRoot->addChild( pListView );

	MScrolledListView* pListView2 = new MScrolledListView();
	for( int i=0;i<100;i++ )
		pListView2->getModel()->add( new MObjString( "HELLO " + MInt::toString( i ) ) );
	pListView2->setRect( MRect( 120, 130, 100, 110 ) );
	pListView2->setBkColor( ivBkColor );
	ptRoot->addChild( pListView2 );

	MComboBox* pCombo = new MComboBox();
	pCombo->setRect( MRect( 120, 250, 100, 30 ) );
	pCombo->getModel()->add( new MObjString( "HELLO1" ) );
	pCombo->getModel()->add( new MObjString( "HELLO2" ) );
	pCombo->getModel()->add( new MObjString( "HELLO3" ) );
	ptRoot->addChild( pCombo );

	MTextField* pFWTF = new MTextField();
	pFWTF->setRect( MRect( 120, 290, 100, 30 ) );
	ptRoot->addChild( pFWTF );

	// test button "Checkbox"
	MToggleButton* pCheckBox =
		new MToggleButton(
			new MCheckBoxRenderer(
				"CheckBox",
				MColorMap::create( 32, 32, 64 ),
				MColorMap::create( 64, 64, 64 ) ) );
	pCheckBox->setBkColor( ivBkColor );
	pCheckBox->setRect( MRect( 120, 330, 100, 30 ));
	ptRoot->addChild( pCheckBox );


	MWndCollection* pScrollView = new MWndCollection();
	pScrollView->setRect( MRect( 0, 0, 500, 500 ) );
	MScrollPane* pScrollPane = new MScrollPane( pScrollView );
	pScrollPane->setRect( MRect( 230, 10, 100, 310 ) );
	for( int i=0;i<10000/30;i++ )
	{
		MWnd* pWnd = 0;
		if( i % 2 == 0 )
			pWnd = new MButton(
				"text" + MInt::toString( i ),
				MColorMap::create( 32, 32, 64 ),
				MColorMap::create( 64, 64, 64 ),
				ivBkColor );
		else
			pWnd = new MTextField();
		pWnd->setRect( MRect( 0, i*30, 100, 30 ));
		pScrollView->addChild( pWnd );
	}
	pScrollPane->setScrollSize( MSize( 100, 10000 ) );
	ptRoot->addChild( pScrollPane );

	// test button "Checkbox"
	MToggleButton* pRadio =
		new MToggleButton(
			new MRadioButtonRenderer(
				"CheckBox",
				MColorMap::create( 32, 32, 64 ),
				MColorMap::create( 64, 64, 64 ) ) );
	pRadio->setBkColor( ivBkColor );
	pRadio->setRect( MRect( 230, 330, 100, 30 ));
	ptRoot->addChild( pRadio );
}

MTestWindow::~MTestWindow()
{
	if( pButton2 )
		pButton2->removeActionListener( this );
	if( pButton3 )
		pButton3->removeActionListener( this );
}

void MTestWindow::paint( IGraphics* pG, const MRect& rect )
{
	/*MFrameWnd::paint( pG, rect );

	unsigned int gridX = 10;
	unsigned int gridY = 10;
	int stepsX = rect.getWidth() / gridX;
	int stepsY = rect.getHeight() / gridY;

	for( int i=0;i<stepsX;i++ )
		pG->drawLine( i*gridX, 0, i*gridX, rect.getHeight(), RGB( 50, 50, 50 ) );

	for( int i=0;i<stepsY;i++ )
		pG->drawLine( 0, i*gridY, rect.getWidth(), i*gridY, RGB( 50, 50, 50 ) );

	MRect r1( 310, 10, 80, 80 );
	MRect r2( 400, 100, 50, 30 );
	MRect rintersect = r1*r2;
	MRect runion = r1+r2;

	pG->fillRect( runion, RGB( 0, 255, 255 ) );
	pG->fillRect( r1, RGB( 0, 0, 255 ) );
	pG->fillRect( r2, RGB( 0, 255, 0 ) );
	pG->fillRect( rintersect, RGB( 255, 255, 0 ) );

	pG->drawText(
		String( "overlap....: " ) + (r1.overlap( r2 ) ? "true" : "false"),
		MRect( 300, 200, 500, 20 ),
		0x00FFFFFF );
	pG->drawText(
		"rect1......: " + r1.toString(),
		MRect( 300, 220, 500, 20 ),
		0x00FFFFFF );
	pG->drawText(
		"rect2......: " + r2.toString(),
		MRect( 300, 240, 500, 20 ),
		0x00FFFFFF );
	pG->drawText(
		"runion.....: " + runion.toString(),
		MRect( 300, 260, 500, 20 ),
		0x00FFFFFF );
	pG->drawText(
		"rintersect.: " + rintersect.toString(),
		MRect( 300, 280, 500, 20 ),
		0x00FFFFFF );

	pG->drawRect( MRect( 0, 0, rect.getWidth(), rect.getHeight() ), RGB( 255, 0, 0 ) );*/
}

bool MTestWindow::create( MRect rect, MTopWnd* pParent )
{
	bool back = MFrameWnd::create( rect, pParent );
	return back;
}

/** query for interface */
void* MTestWindow::getInterface( const String& className ) const
{
	if( className == "MTestWindow" )
		return (void*) ((MTestWindow*)this);
	else if( className == "IActionListener" )
		return (void*) ((IActionListener*)this);
	else
		return MTopWnd::getInterface( className );
}

/** invoked on some button clicks */
void MTestWindow::onActionPerformed( void* ptSrc )
{
	if( ptSrc == pButton2 )
	{
		//MTestDialog dlg;

		MFileDialog dlg;
		if( dlg.showOpenDialog(
			this,
			"Open UI from xml...",
			"xml\0*.xml\0all files\0*.*\0\0" ) )
		{
			MObject* pObj = MTreeDocumentUtils::parse( dlg.getSelectedFile() );
			if( pObj )
			{
				MTopWnd* pWnd = (MTopWnd*) pObj->getInterface( "MTopWnd" );
				if( pWnd )
				{
					pWnd->setActionOnClose( MFrameWnd::DESTROY );
					pWnd->create( MRect( 100, 100, 600, 400 ), this );
					MProgressControl* pProgress = (MProgressControl*) pWnd->getNamespace()->getObj( "prgsId" );
					if( pProgress )
						pProgress->onProgress( 0.9f );
				}
				else
				{
					MLogger::logWarning( "error loading \"%s\": loaded object is not from class MTopWnd", dlg.getSelectedFile().getData() );
					delete pObj;
				}
			}
			else
				MLogger::logWarning( "error loading \"%s\"", dlg.getSelectedFile().getData() );
		}
	}
	else if( ptSrc == pButton3 )
	{
		MFileDialog dlg;
		if( dlg.showSaveDialog(
			this,
			"Save UI as xml...",
			"xml\0*.xml\0all files\0*.*\0\0" ) )
		{
			MTreeNode* pNode = save();
			MFilePrinter out;
			if( out.open( dlg.getSelectedFile() ) )
			{
				pNode->toXml( &out );
				out.close();
			}
			else
				MLogger::logWarning(
					"Could not save file \"%s\"", dlg.getSelectedFile().getData() );
			SAFE_DELETE( pNode );
		}
	}
}