#pragma once

#include "_2RealIPluginActivator.h"

class MidiPlugin : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::Metadata &metadata);
	void setup(_2Real::PluginContext &context);
	void shutdown();
	~ MidiPlugin();

};