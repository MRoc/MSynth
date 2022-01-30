#ifndef __MDiscreteControl
#define __MDiscreteControl

#include "MDefaultControl.h"

/**
 * a control embedding a list of possible values
 */
class MDiscreteControl : public MDefaultControl
{
protected:

	/**
	 * the list of possible values
	 */
	IntList ivValues;

	/**
	 * the default value
	 */
	int ivDefaultValue;

	/**
	 * the index of value in IntList
	 */
	int ivSelectedIndex;

public:

	/**
	 * constructor 1
	 */
	MDiscreteControl( String name, IntList values, int defaultValue );

	/**
	 * constructor 2
	 */
	MDiscreteControl( String name, const int values[], unsigned int valueLength, int defaultValue );
	
	/**
	 * destructor
	 */
	virtual ~MDiscreteControl();

	/**
	 * resets the control to default value
	 */
	virtual void resetToDefault();

	/**
	 * sets the value
	 */
	virtual void setValue( int value );

	/**
	 * returns the selected value
	 */
	virtual int getValue();

	/**
	 * returns the number of possible values
	 */
	virtual unsigned int getValueCount();

	/**
	 * returns the possible value at index
	 */
	virtual int getValueAt( unsigned int index );

	/**
	 * sets the selected index
	 */
	virtual void setSelectedIndex( unsigned int index );

	/**
	 * returns the defaulr value
	 */
	virtual int getDefaultValue();

	/**
	 * returns the selected index
	 */
	virtual unsigned int getSelectedIndex();

	/**
	 * saves the control
	 */
	virtual MTreeNode* save();

	/**
	 * loads the control
	 */
	virtual void load( MTreeNode* node );

	/**
	 * returns a string representation
	 */
	virtual String toString();

protected:

	/**
	 * returns the index of the given value in intlist
	 */
	int getIndex( int value );

	/**
	 * returns true if a value is possible
	 */
	bool isPossibleValue( int value );
};

#endif