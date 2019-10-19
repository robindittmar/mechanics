#ifndef __DEPENDENCY_H__
#define __DEPENDENCY_H__

// Std Lib

// Source SDK

// Custom

class IDependable;

class CDependency
{
public:
	CDependency(IDependable* dependable, void* param = nullptr, bool reverse = false);
	~CDependency();

	IDependable* GetDependable() { return m_pDependable; }
	void* GetParam() { return m_pParam; }
	bool GetReverse() { return m_bReverse; }
private:
	IDependable* m_pDependable;
	void* m_pParam;
	bool m_bReverse;
};

#endif // __DEPENDENCY_H__