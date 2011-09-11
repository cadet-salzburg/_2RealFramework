#pragma once

#include "_2RealIPluginActivator.h"

class ImageProcessing : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::PluginMetadata &_info) throw(...);

	void setup(_2Real::PluginContext &_context) throw (...);

};