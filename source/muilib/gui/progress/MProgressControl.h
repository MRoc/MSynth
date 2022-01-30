#ifndef __MProgressControl
#define __MProgressControl

#include <framework/thread/IProgress.h>
#include "../MWnd.h"

class MProgressControl :
	public MWnd,
	public IProgress
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the progress value 0.0 >= value <= 1.0 */
	float ivValue;

	/** the foreground color */
	MColor ivFgColor;

public:

	GET_RTTI( MProgressControl );
	GET_INTERFACE_SUPER( MProgressControl, "MProgressControl", MWnd );

	/** constructor */
	MProgressControl();

	/** destructor */
	virtual ~MProgressControl();
	
	/** paints the progress bar */
	virtual void paint( IGraphics* pG, const MRect &rect );

	/** invoked to show the progress of a job 0.0 <= value <= 1.0 */
	virtual void onProgress( float value );

	/** invoked when the job was finished */
	virtual void onFinished();

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	static MObject* createInstance()
	{
		return new MProgressControl();
	}
};

#endif