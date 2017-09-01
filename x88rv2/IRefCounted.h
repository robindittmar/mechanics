#ifndef __IREFCOUNTED_H__
#define __IREFCOUNTED_H__

//-----------------------------------------------------------------------------
// Purpose:	Implement a standard reference counted interface. Use of this
//			is optional insofar as all the concrete tools only require
//			at compile time that the function signatures match.
//-----------------------------------------------------------------------------
class IRefCounted
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

#endif // __IREFCOUNTED_H__