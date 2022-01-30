#ifndef __MAttributeMatch
#define __MAttributeMatch

#include "MXmlMatchingNode.h"

/**
 * the attribute matcher is a xml matching returning true
 * if a attribute with a value is found in a node
 */
class MAttributeMatch :
	public MXmlMatchingNode
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

	enum MatchingType
	{
		EXACT,
		BEGIN,
		END
	};

protected:

	MatchingType ivMatchingType;
	String ivAttributeName;
	StrList ivPossibleContent;

public:

	/**
	 * constructor
	 */
	MAttributeMatch();

	/**
	 * destructor
	 */
	virtual ~MAttributeMatch();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * sets the name of the attribute to find
	 */
	virtual void setAttributeName( String name );

	/**
	 * returns the name of the attribute to find
	 */
	virtual String getAttributeName();

	/**
	 * returns true if the attribute is found and has the
	 * specified value.
	 */
	virtual bool match( MTreeNode* ptNode );

	/**
	 * save
	 */
	virtual MTreeNode* save();

	/**
	 * load
	 */
	virtual void load( MTreeNode* ptNode );

	/**
	 * returns an instance of a MAttributeMatch
	 */
	static MObject* createInstance();

	/**
	 * returns a string like the enum matchingtype
	 */
	static String getMatchingTypeAsString( MatchingType t );

	/**
	 * returns the matchingtype extracted from the string
	 */
	static MatchingType getMatchingType( String t );
};

#endif