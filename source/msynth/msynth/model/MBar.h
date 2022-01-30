/*

	(C)2001 MRoc hifiShock

	MBar - A collection of notes for one id (Synth/Drumbox/...)
	Has a Id of the linked soundmachine
	Has a resolution which can be 1/1, 1/2, 1/4, 1/8, 1/16, 1/32 or 1/64

*/

#ifndef __MBAR
#define __MBAR

#include <framework/MUtil.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/map/MProperties.h>
#include <framework/listener/MListenerList.h>

#include "../ui/clipboard/ICopyCutPaste.h"
#include "automation/MEventListCollection.h"

#include "note/MDefaultNoteList.h"
#include "note/MDefaultSynthNoteList.h"
#include "note/MDefaultMessageNoteList.h"
#include "note/MBeatBoxNoteList.h"
#include "list/MListItem.h"
#include "audiochannel/MAudioTriggerList.h"

class IBarListener :
	public IListener
{
	public:

		virtual void barColChanged() = 0;
		virtual void barRowChanged() = 0;
		virtual void barLengthChanged() = 0;
		virtual void barResolutionChanged() = 0;

		virtual void barReleasing() = 0;
};

class MBar :
	public MListItem,
	public ISerializeable,
	public ICopyCutPaste
{
public:

	static int					TYPE_ID;
	static char*				TYPE_USER_NAME;

protected:
		
	int							ivRow,
								ivBarLength,
								ivNotesPerBar;

	MDefaultNoteList*			ivPtNoteList;

	MListenerList				ivListeners;

	IEventListCollection*		ivPtAutomationEventList;

	IAutomationTracker*			ivPtTracker;

	MProperties*				ivPtProperties;

public:

	MBar( IAutomationTracker* ptTracker );
	MBar( IAutomationTracker*, int noteCount );
	MBar( MBar *bar );
	virtual ~MBar();

// SET/GET

	virtual void				setTracker( IAutomationTracker* ptTracker );
	virtual IAutomationTracker* getTracker();
	
	virtual void				setIndex( int index );
	virtual void				setRow( int row );
	virtual int					getRow();
	virtual int					getStopsAt(); // CALCULATED from getIndex() and getLengthInBars()
	virtual void				setStopsAt( int stop );

	virtual int					getLength();
	virtual void				setLength( int newBarLength );

	// Notes per bar
	virtual int					getNotesPerBar();
	virtual void				setNotesPerBar( int notesPerBar );

	// NOTE LIST
	virtual void				setNoteList( MDefaultNoteList *noteList );
	virtual MDefaultNoteList	*getNoteList();

// OPERATIONS

	virtual void				appendBar( MBar *bar );
	virtual MBar*				cutBar( int at );
	virtual void				clear(); // deletes all notes

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	virtual int					getTypeId();
	virtual char*				getUserTypeName();

	virtual	void				release();
	virtual ICopyCutPaste*		getCopy();

	void						addBarListener( IBarListener* ptListener );
	void						removeBarListener( IBarListener* ptListener );

	IEventListCollection*		getEventListCollection();
	void						setEventListCollection( IEventListCollection* ptCollection );

	MProperties*				getProperties();

protected:

	void						fireColChanged();
	void						fireRowChanged();
	void						fireLengthChanged();
	void						fireResolutionChanged();
	void						fireBarReleasing();

	virtual void				setUp();
	virtual void				cleanUp();
};

class MBarClipBoardWrapper : public ICopyCutPaste
{
private:

	MBar* ivPtBar;
	unsigned int ivChannelNum;

public:

	MBarClipBoardWrapper( MBar* ptBar, unsigned int channelNum )
		: ivPtBar( ptBar ),
		ivChannelNum( channelNum )
	{
		ASSERT( ivPtBar );
	};

	MBarClipBoardWrapper( MBarClipBoardWrapper* ptCBW )
		: ivPtBar( (MBar*) ptCBW->getBar()->getCopy() ),
		ivChannelNum( ptCBW->getChannelNum() )
	{
		ASSERT( ivPtBar );
	};

	virtual ~MBarClipBoardWrapper()
	{
		if( ivPtBar )
			ivPtBar->release();
	};

	MBar* getBar()
	{
		return ivPtBar;
	};

	unsigned int getChannelNum()
	{
		return ivChannelNum;
	};

	virtual int getTypeId()
	{
		return ivPtBar->getTypeId();
	};

	virtual	void release()
	{
		ivPtBar->release();
		ivPtBar = 0;
		delete this;
	};

	virtual ICopyCutPaste* getCopy()
	{
		return new MBarClipBoardWrapper( this );
	};

	virtual char* getUserTypeName()
	{
		return ivPtBar->getUserTypeName();
	};
};

class MBCBWList : public ICopyCutPaste
{
private:

	std::vector<MBarClipBoardWrapper*> ivBars;
	unsigned int ivLength;
	unsigned int ivStartsAt;

public:

	MBCBWList()
	{
		ivLength = 0;
		ivStartsAt = 0;
	};

	MBCBWList( MBCBWList* ptList )
	{
		setLength( ptList->getLength() );
		unsigned int count = ptList->getBCBWCount();
		for( unsigned int i=0;i<count;i++ )
			addBCBW( (MBarClipBoardWrapper*)ptList->getBCBW( i )->getCopy() );
	};

	virtual ~MBCBWList()
	{
		unsigned int count = ivBars.size();
		for( unsigned int i=0;i<count;i++ )
			ivBars[ i ]->release();
		ivBars.clear();

	};

	void addBCBW( MBarClipBoardWrapper* ptBar )
	{
		ivBars.push_back( ptBar );
	};

	unsigned int getBCBWCount()
	{
		return ivBars.size();
	};

	MBarClipBoardWrapper* getBCBW( unsigned int index )
	{
		return ivBars[ index ];
	};

	virtual	void release()
	{
		delete this;
	};

	virtual ICopyCutPaste* getCopy()
	{
		return new MBCBWList( this );
	};

	virtual int getTypeId()
	{
		return MBar::TYPE_ID;
	};

	virtual char* getUserTypeName()
	{
		return MBar::TYPE_USER_NAME;
	};

	void setLength( unsigned int length )
	{
		ivLength = length;
	};

	unsigned int getLength()
	{
		return ivLength;
	};

	void setIndex( unsigned int startsAt )
	{
		ivStartsAt = startsAt;
	};

	unsigned int getIndex()
	{
		return ivStartsAt;
	};
};

#endif

