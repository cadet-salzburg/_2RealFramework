#include "PluginActivatorImpl.h"
#include "ServiceImpl.h"

using namespace _2Real;

void KinectWinSDK::start(PluginContext *const _context)
{
	/*
		service name: must be the same as the one used in plugin's metadata
	*/
	_context->registerService("Depthmap", &::createKinectService);
}

_2REAL_EXPORT_PLUGIN(KinectWinSDK)