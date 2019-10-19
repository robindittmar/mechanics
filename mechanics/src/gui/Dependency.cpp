#include "Dependency.h"

CDependency::CDependency(IDependable* dependable, void* param, bool reverse)
	: m_pDependable(dependable), m_pParam(param), m_bReverse(reverse)
{
}

CDependency::~CDependency()
{
}
