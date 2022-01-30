#ifndef __MInt
#define __MInt

#include "../MObject.h"
#include "../treedocument/ISerializeable.h"

/**
 * implementation of a int as object
 */
class MInt :
	public MObject,
	public ISerializeable
{
public:

	/**
	 * runtime support
	 */
	static MRtti gvRtti;

	/**
	 * the maximal number a singed int cat be
	 */
	static const int MAX_INT;

	/**
	 * the maximal number a singed int cat be
	 */
	static const int MIN_INT;

protected:

	/**
	 * the arttribute name for storing in a xml node
	 */
	static const String ATTR_VALUE;

	/**
	 * stores the int value
	 */
	signed int ivData;

public:

	/**
	 * default constructor
	 */
	MInt();

    /**
	 * constructor
	 */
	MInt( const signed int value );


	/**
	 * destructor
	 */
	virtual ~MInt();

	/**
	 * returns a string representation
	 */
	virtual String toString() const;

	/**
	 * returns the class name for runtime purpose
	 */
	virtual IRtti* getRtti() const;

	/**
	 * querries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

	/**
	 * returns the stored int value
	 */
	virtual signed int getValue();

	/**
	 * sets the stored int value
	 */
	virtual void setValue( signed int value );

	/**
	 * serialize as tree node
	 */
	virtual MTreeNode* save();

	/**
	 * load from treenode
	 */
	virtual void load( MTreeNode* node );

	/**
	 * creates an instance
	 */
	static MObject* createInstance();

	/**
	 * parses the int in the text
	 */
	static int parse( const char* txt );

	/**
	 * parses the int in the text
	 */
	static int parse( const char* txt, int defaultValue );

	/**
	 * parses the int in the text and tests it
	 */
	static int parse( const char* txt, int min, int max, int defaultValue );

	/**
	 * to strings an int
	 */
	static String toString( int number );
};

#endif