#include "IDependable.h"

IDependable::IDependable(EnableDependency_t evaluationHandler)
	: m_pEvaluationHandler(evaluationHandler)
{
}

IDependable::~IDependable()
{
	m_vDependentOnes.clear();
	m_vDependencies.clear();
}

void IDependable::AddDependency(IDependable* pDependency, void* pParam, bool bReverse)
{
	assert(pDependency != nullptr);

	m_vDependencies.push_back({ pDependency, pParam, bReverse });
	pDependency->AddDependentOne(this);
}

void IDependable::AddDependentOne(IDependable* pDependentOne)
{
	assert(pDependentOne != nullptr);

	m_vDependentOnes.push_back(pDependentOne);
}

void IDependable::EvaluateDependencies()
{
	std::unordered_set<IDependable*> m_setControls;

	bool bEnable = true;
	for (std::vector<CDependency>::iterator it = m_vDependencies.begin(); it != m_vDependencies.end(); it++)
	{
		if (m_setControls.find(it->GetDependable()) != m_setControls.end())
			continue;

		// If its == reverse, we don't want it to be enabled
		if (it->GetDependable()->ShouldDependentOnesBeEnabled(it->GetParam()) == it->GetReverse())
		{
			bEnable = false;

			// Iterate over remaining dependencies; if there are multiple conditions for the same
			// control it is treated as an 'OR' condition, instead of an 'AND'
			// (They will be skipped on the outer iteration anyways, since the unordered_set
			// keeps that from happening)
			std::vector<CDependency>::iterator itCopy = it + 1;
			for (; itCopy != m_vDependencies.end(); itCopy++)
			{
				// Only same control we already checked
				if (it->GetDependable() != itCopy->GetDependable())
					continue;

				// If any of the other conditions tell us, that we should enable the control
				if (itCopy->GetDependable()->ShouldDependentOnesBeEnabled(itCopy->GetParam()) != itCopy->GetReverse())
				{
					bEnable = true;
					break;
				}

			}

			if (!bEnable)
				break;
		}

		m_setControls.emplace(it->GetDependable());
	}

	//this->SetEnabled(bEnable);
	(this->*m_pEvaluationHandler)(bEnable);
}

void IDependable::NotifyDependentOnes()
{
	for (std::vector<IDependable*>::iterator it = m_vDependentOnes.begin(); it != m_vDependentOnes.end(); it++)
	{
		(*it)->EvaluateDependencies();
	}
}