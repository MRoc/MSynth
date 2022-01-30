/*(C)2000/2001 MRoc hifiShock*/

#ifndef __MainApp
#define __MainApp

#pragma warning(disable:4786)

#include <framework/MTypes.h>
#include <framework/map/MInt2ObjMap.h>
#include <framework/map/MProperties.h>
#include <framework/MDefaultFactory.h>
#include <framework/resource/MResourceManager.h>

#include "MAppData.h"
#include "MainFrame.h"

#include "msynth/ui/splash/Splash1.h"

#include "msynth/ui/instrument/MInstrumentEditorFactory.h"
#include "msynth/ui/instrument/synth/MSynthEditor.h"
#include "msynth/ui/instrument/beatbox/MBeatBoxEditor.h"
#include "msynth/ui/instrument/audio/MAudioChannelEditor.h"

#include "msynth/ui/noteeditor/synth/MNoteFrame.h"
#include "msynth/ui/noteeditor/beatbox/MBeatBoxNoteEditor.h"
#include "msynth/ui/noteeditor/audio/MAudioTriggerFrame.h"

#include "msynth/ui/processor/MDescriptorViewFactory.h"
#include "msynth/ui/processor/MDescriptorView.h"
#include "msynth/ui/processor/MWaveOscView.h"
#include "msynth/ui/processor/MMultiOscControl.h"
#include "msynth/ui/processor/MIIRControl.h"

#include "gui/MFont.h"
#include "gui/tooltip/MToolTipCtrl.h"
#include "gui/layout/MLayout.h"
#include "gui/layout/MLayoutWnd.h"
#include "gui/layout/MLayoutRow.h"
#include "gui/layout/MLayoutCol.h"
#include "gui/layout/MLayoutRoot.h"

#include "msynth/ui/clipboard/MClipboard.h"

#include "msynth/model/source/MSourceRegistry.h"
#include "msynth/model/source/MOscSaw.h"
#include "msynth/model/source/MOscTriangle.h" 
#include "msynth/model/source/MOscSquare.h"
#include "msynth/model/source/MAnalogSquare.h"
#include "msynth/model/source/MOscSine.h"
#include "msynth/model/source/MWaveOsc.h"
#include "msynth/model/source/MOscString.h"
#include "msynth/model/source/MOrgan.h"
#include "msynth/model/source/MMultiOsc.h"
#include "msynth/model/source/MNoise.h"
#include "msynth/model/source/MAAOsc.h"

#include "msynth/model/transformer/MTransformerRegistry.h"
#include "msynth/model/transformer/MEnvelope.h"
#include "msynth/model/transformer/MClipper.h"
#include "msynth/model/transformer/MIIR.h"
#include "msynth/model/transformer/MDelay.h"
#include "msynth/model/transformer/MReverb.h"
#include "msynth/model/transformer/MFuzz.h"
#include "msynth/model/transformer/MLofi.h"
#include "msynth/model/transformer/MPeakEq.h"
#include "msynth/model/transformer/MOneZeroFilter.h"
#include "msynth/model/transformer/MOnePoleFilter.h"
#include "msynth/model/transformer/MFIR.h"
#include "msynth/model/transformer/MComb.h"

#include "msynth/model/dmo/MDMOWrapper.h"

#include "msynth/model/streamingengine/MAudioEngine.h"
#include "msynth/model/note/MNoteFactory.h"
#include "msynth/model/note/MDefaultSynthNoteList.h"
#include "msynth/model/note/MBeatBoxNoteList.h"
#include "msynth/model/interfaces/IDescriptor.h"
#include "msynth/model/interfaces/IDescriptorCollection.h"
#include "msynth/model/interfaces/MSoundbuffer.h"
#include "msynth/model/Synth.h"
#include "msynth/model/audiochannel/MAudioTriggerList.h"
#include "msynth/model/automation/ITargetRegistry.h"
#include "msynth/model/converter/MInt2FloatConverter.h"
#include "msynth/model/converter/MElementMatch.h"
#include "msynth/model/converter/MAttributeContentReplacer.h"
#include "msynth/model/converter/MAttributeMatch.h"
#include "msynth/model/converter/MAttributeAdder.h"
#include "msynth/model/converter/MChildElementEnumeration.h"
#include "msynth/model/converter/MAttributeSubStr2Attribute.h"
#include "msynth/model/converter/MElementRenamer.h"
#include "msynth/model/converter/MAttributeRenamer.h"
#include "msynth/model/converter/MAttribute2Element.h"
#include "msynth/model/converter/MSampleBankImport.h"
#include "msynth/model/converter/MSampleImport.h"
#include "gui/MApp.h"

class MainApp : public MApp
{
public:

	MainApp();
	virtual ~MainApp();

	virtual bool onInit();
	virtual bool onExit();
	virtual String getAppName(){ return "MSynth"; };

protected:

	virtual void initFactories();
	virtual String getAppDir();
	virtual void initAudioDMOList();
};


#endif