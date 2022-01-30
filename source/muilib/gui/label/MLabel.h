/**
 * MLabel (C)2000-2003 MRoc
 */

#ifndef __MLABEL
#define __MLABEL

#include "MObjectLabel.h"

/**
 * a label displaying a string.
 * inherits from MObjectLabel to obtain default label behaviour.
 * because MObjectLabel only displays a MObject by invoking its
 * toString method, it creates a MObjString as MObject.
 */
class MLabel :
	public MObjectLabel
{
public:

	/** runtime type info */
	static MRtti gvRtti;

public:

	/** constructor */
	MLabel();

	/** constructor */
	MLabel( String text, MColor textColor, MColor bkColor );

	/** destructor */
	virtual ~MLabel();

	/** sets the text to be rendered */
	virtual void setText( String text );

	/** returns the displayed text */
	virtual String getText();

	/** loads from the given tree node */
	virtual void load( MTreeNode* ptNode );

	/** stores as tree node */
	virtual MTreeNode* save();

	/** creates an instance of this class */
	static MObject* createInstance();

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** query interface */
	virtual void* getInterface( const String& className ) const;
};

#endif
