#include "PluginActivatorImpl.h"

#include "ImageAddition.h"
#include "RandomImage.h"

using namespace _2Real;

void ImageProcessing::start(PluginContext *const _context)
{
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

_2REAL_EXPORT_PLUGIN(ImageProcessing)