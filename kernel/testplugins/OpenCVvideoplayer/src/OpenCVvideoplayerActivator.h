#pragma once

#include "_2RealIPluginActivator.h"
//#include "OpenCVvideoplayer.h"

//name of plugin is "OpenCVvideoplayer"
class OpenCVvideoplayer : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::PluginMetadata &info);
	void setup(_2Real::PluginContext &context);
	~OpenCVvideoplayer();

};