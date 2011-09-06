#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "ImageAddition.h"
#include "RandomImage.h"

#include <iostream>

using namespace _2Real;

void ImageProcessing::getMetadata(PluginMetadata &_info)
{
	/**
	*	uses the metadata reader helper class
	*	(one could build the metadata manually, too)
	*/

	MetadataReader reader;
	reader.readMetadata(_info);
}

void ImageProcessing::start(PluginContext &_context)
{
	/**
		export service factory methods
	*/

	_context.registerService("ImageAddition2D_ushort", &::createImageAddition< unsigned short >);
	_context.registerService("ImageAddition2D_float", &::createImageAddition< float >);
	_context.registerService("RandomImage2D_ushort", &::createRandomImage< unsigned short >);
	_context.registerService("RandomImage2D_float", &::createRandomImage< float >);
}

/**
*	this macro is needed for the poco classloader
*/
_2REAL_EXPORT_PLUGIN(ImageProcessing)