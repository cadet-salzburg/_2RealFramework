#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "ImageAddition.h"
#include "RandomImage.h"

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

	_context.registerService("ImageAddition_ushort", &::createImageAddition< unsigned short >);
	_context.registerService("ImageAddition_uint", &::createImageAddition< unsigned int >);
	_context.registerService("ImageAddition_float", &::createImageAddition< float >);
	_context.registerService("RandomImage_ushort", &::createRandomImage< unsigned short >);
	_context.registerService("RandomImage_uint", &::createRandomImage< unsigned int >);
	_context.registerService("RandomImage_float", &::createRandomImage< float >);
}

/**
*	this macro is needed for the poco classloader
*/
_2REAL_EXPORT_PLUGIN(ImageProcessing)