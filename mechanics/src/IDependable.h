#ifndef __ICONTROLDEPENDABLE_H__
#define __ICONTROLDEPENDABLE_H__

// Std Lib
#include <assert.h>
#include <vector>
#include <unordered_set>

// Source SDK

// Custom
#include "Dependency.h"

typedef void(IDependable::*EnableDependency_t)(bool);

class IDependable
{
public:
	IDependable(EnableDependency_t evaluationHandler = nullptr);
	~IDependable();

	/// <summary>
	/// Adds dependency of another IDependable
	/// (For example: ChildCheckbox->AddDependency(ParentCheckbox) will result
	/// in ChildCheckbox being disabled, when ParentCheckbox is disabled aswell [and vice versa ofc])
	/// 
	/// The full dependency list is checked, using && on multiple dependencies,
	/// but || when having multiple conditions for a single dependency
	/// </summary>
	/// <param name="pDependency">Dependency</param>
	/// <param name="pParam">Optional parameter to use on evaluation</param>
	/// <param name="bReverse">Reverse the logic if this control will be enabled/disabled for this one dependency</param>
	void AddDependency(IDependable* pDependency, void* pParam = nullptr, bool bReverse = false);

	/// <summary>
	/// Honestly you shouldn't invoke this, unless you explicitly want to change the Relationship between controls
	/// 
	/// this will be invoked by AddDependency, so I highly recommend you to check out AddDependency instead of this
	/// </summary>
	/// <param name="pDependentOne"></param>
	void AddDependentOne(IDependable* pDependentOne);

	/// <summary>
	/// Evaluates if we want to be enabled or disabled (usually gets called from depenencies itself)
	/// </summary>
	void EvaluateDependencies();

	/// <summary>
	/// Will notify dependent ones about any changes so they'll evaluate if
	/// they need to be enabled or disabled
	/// </summary>
	void NotifyDependentOnes();

	/// <summary>
	/// Evaluates if we want our dependent ones to be enabled
	/// </summary>
	/// <returns>Wether or not the dependent ones should be enabled</returns>
	virtual bool ShouldDependentOnesBeEnabled(void* pParam) = 0;
protected:
	EnableDependency_t m_pEvaluationHandler;

	std::vector<CDependency> m_vDependencies;
	std::vector<IDependable*> m_vDependentOnes;
};

#endif // __ICONTROLDEPENDABLE_H__