/**
 * (C)2001-2003 MRoc
 */

#ifndef __MCOUNTCONTROL
#define __MCOUNTCONTROL

#include "../MWndCollection.h"
#include "../label/MLabel.h"
#include "../spinbutton\MSpinButton.h"
#include "MCounter.h"

class MCountControl :
	public MWndCollection,
	public ISpinButtonModelListener
{
protected:

	MLabel*			ivPtLabel;
	MSpinButton*	ivPtSpinbutton;
	MCounter*		ivPtCounter;

	unsigned int	ivRange;	// decimal positions
	int				ivTextLabelWidth;

public:

	FULL_RTTI_SUPER( MCountControl, "MCountControl", MWndCollection );

	MCountControl();

	MCountControl(
		unsigned int range,
		int min,
		int max,
		MColor bgColor,
		MColor fgColor,
		String text,
		int textLabelwidth );

	virtual ~MCountControl();

	virtual void	doLayout();
	int				getPos();
	void			setPos( int pos );
	MSpinButtonModel* getModel();

	virtual void paint( IGraphics* pG, const MRect& rect );

	/** store ui state into treenode */
	virtual MTreeNode* save();

	/** load ui state from treenode */
	virtual void load( MTreeNode* pNode );

	static MObject* createInstance()
	{
		return new MCountControl();
	}

protected:

	virtual void valueChanged( MSpinButtonModel* pModel );
};

#endif