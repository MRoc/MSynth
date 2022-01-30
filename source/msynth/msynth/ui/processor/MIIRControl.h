/**
 * (C)2000-2003 MRoc
 */

#ifndef __MIIRCONTROL
#define __MIIRCONTROL

#include <framework/MTypes.h>
#include <gui/label/MLabel.h>

#include "MControlSubView.h"
#include "MAbstractDescriptorView.h"

#include "../control/MFloatGui.h"
#include "../control/MComboGui.h"
#include "../../model/interfaces/IDescriptor.h"
#include "../../model/transformer/MIIR.h"
#include "../../../resource.h"


#define COMBO_COUNT 3

class MIIRControl :
	public MAbstractDescriptorView
{
public:

	static const char*		MIIR_TYPES_STRINGS[];
	static const char*		MIIR_ORDER_STRINGS[];
	static const char*		MIIR_APPROXSCHEME_STRINGS[];

	static MPoint			COMBO_POINTS[ COMBO_COUNT ];

protected:

	MLabel*					ivLabelName;
	MFloatGui*				ivPtFloatGuis[2];
	MComboGui*				ivPtCombo[3];
	IntList					ivSliderIndexList;
	MControlSubView*		ivPtLfoControl;
	MControlSubView*		ivPtEnvControl;

public:

	MIIRControl();
	virtual ~MIIRControl();

	virtual void setDescriptor( IDescriptor* pDesctiptor );
	virtual void paint( IGraphics* ptGraphics, const MRect &rect );

	static MAbstractDescriptorView* createInstance();
};

#endif

