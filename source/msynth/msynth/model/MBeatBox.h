/*(C)2000-2003 MRoc hifiShock*/

#ifndef __MDRUMBOX
#define __MDRUMBOX

#include <framework/MTypes.h>
#include <framework/list/MSelectableObjectList.h>

#include "source/MWavePlayer.h"
#include "samplebank/MSampleBank.h"
#include "interfaces/MInstrument.h"
#include "interfaces/IBpmListener.h"
#include "note/MBeatBoxNoteList.h"
#include "../../resource.h"

class MBeatBox :
	public MInstrument,
	public IBpmListener
{
public:

	static const char*				CLASS_NAME;

	static int						TYPE_ID;
	static int						DRUMBOX_COUNT;

	static int						MESSAGE_CHANNEL_ADDED;
	static int						MESSAGE_CHANNEL_REMOVED;
	static int						MESSAGE_UPDATE_FROM_MODEL;
	static int						MESSAGE_CHANNEL_SELECTED;

protected:

	String							ivSampleBankName;
	MSelectableObjectList*			ivPtChannels;
	IBpmControl*					ivPtBpmControl;

	MCriticalSection				ivCriticalSection;

public:

	MBeatBox( IBpmControl* ptBpmControl = 0 );
	virtual ~MBeatBox();

	virtual String toString();

	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int getChildCount();

	virtual void createChannel();
	virtual void deleteChannel( MObject* ptChannel );

	virtual unsigned int getWavePlayerCount();
	virtual void setWavePlayerCount( unsigned int channelCount );
	virtual MWavePlayer* getWavePlayerAt( unsigned int index );
	virtual int getWavePlayerIndex( MObject* ptPlaya );
	virtual bool newSampleBank();
	virtual bool saveSampleBank( String fileName );
	virtual bool loadSampleBank( String fileName );

	virtual void setSampleBankName( String name );
	virtual String getPathName();
	virtual String getSampleBankName();
	virtual String getSampleBankFolder();

	virtual void setSelectedChannel( int index );
	virtual void setSelectedChannel( MWavePlayer* ptPlaya );
	virtual MWavePlayer* getSelectedChannel();
	virtual int getSelection();
	
	virtual void processEvent( MEvent* ptEvent );
	virtual void goNext( MSoundBuffer *buffer, unsigned int renderFrom, unsigned int renderTo );

	virtual unsigned int getChannelCount();
	virtual int getNoteListId();
	virtual bool noteListNeedsListener();
	virtual int getBitmapId( int id );
	virtual int getEditorId();

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	virtual String getName();

	/**
	 * invoked whent he bpm changed
	 * (inheritanced from IBpmListener)
	 */
	virtual void bpmChanged( unsigned int bpm );

	virtual void makeDefaultPlayer();

	virtual String getClassName();

protected:

	void parseChildNode( MTreeNode* pNode, unsigned int *curChannel );

	void fireUpdateFromModel();
	void fireChannelRemoved( MWavePlayer* ptPlayer, unsigned int index ); // NOT USED 
	void fireChannelAdded( MWavePlayer* ptPlayer ); // NOT USED
	void fireSelectedChannelChanged();
};

#endif