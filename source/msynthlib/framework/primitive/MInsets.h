/**
 * (C) 2000-2003 MRoc
 */

#ifndef __MINSETS
#define __MINSETS

#include "MInt.h"
#include "../treedocument/ISerializeable.h"

class MInsets :
	public MObject,
	public ISerializeable
{
public:

	static MRtti gvRtti;

protected:

	int		ivLeft,
			ivRight,
			ivTop,
			ivBottom;

public:

	MInsets();
	MInsets( int left, int right, int top, int bottom );

	virtual ~MInsets();

	virtual IRtti* getRtti() const;
	virtual void* getInterface( const String &className ) const;

	virtual int left() const;
	virtual int right() const;
	virtual int top() const;
	virtual int bottom() const;
	virtual int width() const;
	virtual int height() const;

	virtual String toString() const;

	virtual void load( MTreeNode* ptNode );
	virtual MTreeNode* save();

	static MObject* createInstance();
};

#endif