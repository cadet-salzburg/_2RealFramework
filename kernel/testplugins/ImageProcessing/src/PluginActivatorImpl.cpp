#include "PluginActivatorImpl.h"

#include "ImageAddition.h"
#include "RandomImage.h"

using namespace _2Real;

void ImageProcessing::start(PluginContextPtr _context)
{
	/*
		listen to framework related events, such as unexpected shutdown etc.
	*/
	_context->addFrameworkListerner(*this);

	/*
		export service factory methods
	*/
	_context->registerService("ImageAddition_ushort", &::createImageAddition< unsigned short >);
	_context->registerService("ImageAddition_uint", &::createImageAddition< unsigned int >);
	_context->registerService("ImageAddition_float", &::createImageAddition< float >);

	_context->registerService("RandomImage_ushort", &::createRandomImage< unsigned short >);
	_context->registerService("RandomImage_uint", &::createRandomImage< unsigned int >);
	_context->registerService("RandomImage_float", &::createRandomImage< float >);
}

void ImageProcessing::stop(PluginContextPtr _context)
{
	/*
		stop listening to framework events
	*/
	_context->removeFrameworkListerner(*this);

	/*
		unregister previously exported services
		this will cause all existing service objects to be shut down,
		and prevent further service objects from being created
	*/	
	_context->unregisterService("ImageAddition_ushort");
	_context->unregisterService("ImageAddition_uint");
	_context->unregisterService("ImageAddition_float");

	_context->unregisterService("RandomImage_ushort");
	_context->unregisterService("RandomImage_uint");
	_context->unregisterService("RandomImage_float");
}

_2REAL_EXPORT_PLUGIN(ImageProcessing)