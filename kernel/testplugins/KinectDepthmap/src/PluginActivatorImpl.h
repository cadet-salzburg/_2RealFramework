#pragma once

#include "_2RealAbstractPluginActivator.h"

class KinectWinSDK : public _2Real::AbstractPluginActivator
{

public:

	void start(_2Real::PluginContextPtr _context);
	void stop(_2Real::PluginContextPtr _context);

};