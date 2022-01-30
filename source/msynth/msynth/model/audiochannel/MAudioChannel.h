#ifndef __MAudioChannel
#define __MAudioChannel

#include <framework/list/MSelectableObjectList.h>
#include "MAudiofile.h"
#include "MAudioTriggerList.h"
#include "../interfaces/MInstrument.h"
#include "../interfaces/IBpmListener.h"
#include "../../../resource.h"

class MAudioChannel :
	public MInstrument
{
public:

	static const int TYPE_ID;

protected:

	static int AUDIOCHANNEL_COUNT;

	MSelectableObjectList* ivPtAudioFiles;

	MAudioFile* ivPtCurrentFile;

	bool ivFadeOut;

public:

	MAudioChannel();
	virtual ~MAudioChannel();

	virtual void	importFile( String fileName );

	virtual void	processEvent( MEvent* ptEvent );
	virtual void	goNext( MSoundBuffer *buffer, unsigned int startFrom, unsigned int stopAt);
	virtual unsigned int getChannelCount();

	virtual int		getNoteListId();
	virtual int		getEditorId();

	virtual bool	noteListNeedsListener();
	virtual int		getBitmapId( int state );

	/**
	 * loads from the given tree node
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * stores as tree node
	 */
	virtual MTreeNode* save();

	virtual String getName();
	virtual String toString();

	virtual MSelectableObjectList* getFileList();

	virtual IControlCollection* getChild( unsigned int index );
	virtual unsigned int getChildCount();

};

#endif