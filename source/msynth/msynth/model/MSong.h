/*

  MSong (C)2001 MRoc hifiShock

  A MSong contains some MBarContainer

*/

#ifndef __MSONG
#define __MSONG

#pragma warning(disable:4786)

#include <framework/MTypes.h>
#include <framework/MUtil.h>
#include <framework/treedocument/ISerializeable.h>
#include <framework/map/MInt2ObjMap.h>
#include <framework/listener/MListenerList.h>

#include "MBar.h"
#include "MSystemListener.h"
#include "MBarContainer.h"
#include "Synth.h"
#include "MBeatBox.h"
#include "MSongListener.h"
#include "audiochannel/MAudioChannel.h"

typedef std::vector<MBarContainer*> BarContainerList;
typedef BarContainerList::iterator BarContainerIter;

class MSong :
	public ISerializeable
{
protected:

	unsigned int		ivMaxBarCount;
	BarContainerList	ivArBarContainers;

	MListenerList		ivSongListeners;

	IBpmControl*		ivPtBpmControl;

public:

	MCriticalSection	ivCriticalSection;

	MSong( IBpmControl* ptControl );
	~MSong();

	// actions
	void			newSong();
	MBar*			createBar( unsigned int row, unsigned int col, unsigned int resolution );
	bool			moveBar( unsigned int fromRow, unsigned int fromCol, int rowDiv, int colDiv );
	bool			glow( unsigned int row, MBar* ptBar );
	MBar*			cutBar( unsigned int row, unsigned int index, MBar* ptBar );
	void			deleteBarContainer( int index, bool sendNotify = true );
	void			swapBarContainer( unsigned int index1, unsigned int index2 );
	void			resetAllInstruments();

	void			createDefaultSynth( bool shouldNotify = true );
	void			createDefaultBeatBox( bool shouldNotify = true );
	void			createDefaultAudioChannel( bool shouldNotify = true );

	// infos
	void			setMaxBarCount( unsigned int maxBarCount );
	unsigned int	getMaxBarCount();
	unsigned int	getMinBarCount();

	unsigned int	getBarContainerCount();
	MBarContainer*	getBarContainer( unsigned int index );

	// song listener
	void			addSongListener( ISongListener* ptSongListener );
	void			removeSongListener( ISongListener* ptSongListener );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

protected:

	void			addBarContainer( MBarContainer *ptBarContainer, bool sendNotify );
	MBarContainer*	removeBarContainer( unsigned int index, bool sendNotify = true );

	void			deleteContent( bool sendNotify );

	void			fireInstrumentAdded( unsigned int index );
	void			fireInstrumentRemoved( unsigned index );
	void			fireInstrumentsSwapped( unsigned index1, unsigned index2 );
	void			fireModelChanged();
	void			fireSongDestroy();
};


#endif