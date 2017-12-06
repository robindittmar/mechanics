#ifndef __CUTLVECTOR_H__
#define __CUTLVECTOR_H__

// We actually don't care about the implementation
//
// Afaik these are only used in EmitSound1 and EmitSound2 and are also just passed as pointers in the
// parameter list which are not even used by us

template<class T, class I = int>
class CUtlMemory {};

template<class T, class A = CUtlMemory<T>>
class CUtlVector {};

#endif // __CUTLVECTOR_H__