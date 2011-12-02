#pragma once

#include "_2RealIPluginActivator.h"

#include <string>
#include <fstream>

class ImageProcessing : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::PluginMetadata &info);
	void setup(_2Real::PluginContext &context);
	virtual ~ImageProcessing();

private:

	//std::ofstream		m_Logfile;

};