#include "IFeature.h"
#include "Application.h"

IFeature::IFeature()
	: m_bIsEnabled(false)
{
}

IFeature::~IFeature()
{
}

void IFeature::Setup()
{
	m_pApp = CApplication::Instance();
}
