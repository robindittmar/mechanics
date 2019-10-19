#ifndef __CONFIGHELPER_H__
#define __CONFIGHELPER_H__

#include "Config.h"

namespace ConfigHelper
{
	void ConfigToFeatures(CConfig* pConfig);
	void FeaturesToConfig(CConfig* pConfig);
}

#endif // __CONFIGHELPER_H__