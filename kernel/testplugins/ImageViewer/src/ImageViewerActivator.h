#pragma once

#include "_2RealIPluginActivator.h"

// give class the name of the plugin
class ImageViewer : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::PluginMetadata &info);
	void setup(_2Real::PluginContext &context);
	~ImageViewer();

};