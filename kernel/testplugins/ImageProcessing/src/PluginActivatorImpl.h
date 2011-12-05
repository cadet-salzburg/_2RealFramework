#pragma once

#include "_2RealIPluginActivator.h"

#include <fstream>

class ImageProcessing : public _2Real::IPluginActivator
{

public:

	void getMetadata(_2Real::Metadata &metadata);
	void setup(_2Real::PluginContext &context);
	~ImageProcessing();

private:

	std::ofstream		m_Logfile;

};