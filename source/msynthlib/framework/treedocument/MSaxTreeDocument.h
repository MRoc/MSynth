#ifndef __MSaxTreeDocument
#define __MSaxTreeDocument

#include "MTreeDocument.h"
#include <expat.h>
#include "../MException.h"
#include "../io/ISeekableInputStream.h"

/**
 * implementation of a tree document
 * able to parse a xml file with the
 * apache sax parser.
 */
class MSaxTreeDocument :
	public MTreeDocument
{
public:

	/**
	 * the runtime type info
	 */
	static MRtti gvRtti;

protected:

	/**
	 * current node, used for building the
	 * tree with the sax parser.
	 */
	MTreeNode* ivPtCurrentNode;

public:

	/**
	 * constructor
	 */
	MSaxTreeDocument();

	/**
	 * destructor
	 */
	virtual ~MSaxTreeDocument();

	/**
	 * parses the specified xml file
	 */
	virtual void parse( String fileName );

	/**
	 * parse a resource xml file
	 */
	virtual void parseResource( String resourceName );

	/**
	 * parses the xml file given in the input stream
	 */
	virtual void parse( ISeekableInputStream* pIn );

	/**
	 * creates an instance of this class
	 */
	static MObject* createInstance();

	/**
	 * returns the runtime type info
	 */
	virtual IRtti* getRtti() const;

	/**
	 * queries for a specified interface
	 */
	virtual void* getInterface( const String &className ) const;

private:

	void startElementHandler(
		void *userData,
		const XML_Char *name,
		const XML_Char **atts );

	void endElementHandler(
		void *userData,
		const XML_Char *name );

	void characterHandler(
		const XML_Char *s,
		int len );

	static void _startElementHandler(
		void *userData,
		const XML_Char *name,
		const XML_Char **atts );

	static void _endElementHandler(
		void *userData,
		const XML_Char *name );

	static void _characterHandler(
		void *userData,
		const XML_Char *s,
		int len );
};

#endif