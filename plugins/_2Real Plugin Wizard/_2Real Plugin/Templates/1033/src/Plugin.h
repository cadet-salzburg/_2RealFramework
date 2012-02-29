#pragma once

#include "_2RealIPluginActivator.h"

class [!output SAFE_PROJECT_NAME] : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::Metadata &metadata);
	void setup(_2Real::PluginContext &context);
	void shutdown();
	~ [!output SAFE_PROJECT_NAME]();

};