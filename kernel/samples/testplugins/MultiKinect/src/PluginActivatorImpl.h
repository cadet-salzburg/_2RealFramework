#pragma once

#include "_2RealIPluginActivator.h"

#include "_2RealKinect.h"

#include "vld.h"

#include <string>
#include <fstream>

class MultiKinectOpenNI : public _2Real::IPluginActivator
{

public:

	~MultiKinectOpenNI();
	void					getMetadata(_2Real::Metadata &info);
	void					setup(_2Real::PluginContext &context);
	void					shutdown();
	
private:

	_2Real::_2RealKinect*	m_2RealKinect;
	bool					m_AlignColorDepth;
	unsigned int			m_GeneratorFlags;
	unsigned int			m_ImageFlags;
	_2Real::_2RealLogLevel	m_LogLevel;
	std::string				m_LogPath;
	std::ofstream			m_LogStream;

	void					getGeneratorFlags(_2Real::PluginContext &context);
	void					getImageFlags(_2Real::PluginContext &context);
	void					getLogSettings(_2Real::PluginContext &context);
};