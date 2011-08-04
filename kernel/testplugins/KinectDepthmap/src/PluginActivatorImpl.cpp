#include "PluginActivatorImpl.h"
#include "ServiceImpl.h"

using namespace _2Real;

void KinectWinSDK::start(PluginContextPtr _context)
{
	_context->addFrameworkListerner(*this);

	/*
		service name: must be the same as the one used in plugin's metadata
	*/
	_context->registerService("Depthmap", &::createKinectService);
}

void KinectWinSDK::stop(PluginContextPtr _context)
{
	_context->removeFrameworkListerner(*this);

	_context->unregisterService("Depthmap");
}

_2REAL_EXPORT_PLUGIN(KinectWinSDK)