#pragma once

#include "_2RealIPluginActivator.h"

class TutorialPlugin : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::Metadata &metadata);
	void setup(_2Real::PluginContext &context);
	void shutdown();
	~TutorialPlugin();

};