#ifndef __MToggleButton
#define __MToggleButton

#include "MButton.h"

/* MToggleButton (C)2001-2003 MRoc hifiShock */
class MToggleButton : public MButton
{
public:

	/** runtime type info */
	static MRtti gvRtti;

protected:

	/** the internal toggle state */
	bool ivState;

public:

	/** constructor */
	MToggleButton();

	/** constructor */
	MToggleButton( IWndRenderer* pRenderer );

	/** constructor */
	MToggleButton( unsigned int bmIdUnpressed, unsigned int bmIdPressed, MColor tColor1, MColor tColor2, MColor bkColor );

	/** constructor */
	MToggleButton( String text, MColor textColor, MColor tColorInvalid, MColor bkColor );

	/** constructor */
	MToggleButton( MArrowRenderer::Direction direction, MColor bkColor );

	/** destructor */
	virtual ~MToggleButton();

	/** returns the toggle value */
	virtual bool getValue();

	/** sets the toggle value */
	virtual void setValue( bool value );

	/** returns true if button should be painted sunken */
	virtual bool isSunken();

	/** query superclass */
	virtual void* getInterface( const String& className ) const;

	/** returns the runtime type info */
	virtual IRtti* getRtti() const;

	/** creates an instance of this class */
	static MObject* createInstance();

protected:

	/** invoked from MButton when clicked */
	virtual void clicked();
};

#endif