#pragma once

#include "_2RealIPluginActivator.h"

#include <fstream>

class SimpleStuff : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::Metadata &metadata);
	void setup(_2Real::PluginContext &context);
	void shutdown();
	~SimpleStuff();

};