#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "ImageAddition.h"
#include "RandomImage.h"
#include "ImageScaling.h"

#include <iostream>

using namespace _2Real;

void ImageProcessing::getMetadata(PluginMetadata &_info)
{
	//uses the metadata reader helper class
	//(one could build the metadata manually, too)
	try
	{
		MetadataReader reader;
		reader.readMetadata(_info);
	}
	catch (Exception &e)
	{
		std::cout << "dll: e caught" << std::endl;
		throw e;
	}
}

void ImageProcessing::setup(PluginContext &_context)
{
	//export service factory methods
	try
	{

		//query test parameters - those are not actually needed here ;)
		std::vector< std::string > vec;
		_context.getSetupParameter("input vector", vec);
		for (unsigned int i=0; i<vec.size(); i++)
		{
			std::cout << vec[i] << std::endl;
		}

		_context.registerService("ImageAddition2D_ushort", &::createImageAddition< unsigned short >);
		_context.registerService("ImageAddition2D_float", &::createImageAddition< float >);
		_context.registerService("RandomImage2D_ushort", &::createRandomImage< unsigned short >);
		_context.registerService("RandomImage2D_float", &::createRandomImage< float >);
		_context.registerService("ImageScaling2D_ushort", &::createImageScaling< unsigned short >);
		_context.registerService("ImageScaling2D_float", &::createImageScaling< float >);

	}
	catch (Exception &e)
	{
		throw e;
	}
}

//this macro is needed for the poco classloader
//it's mandatory that this is the same name as the one defined as pluginname in metadata
_2REAL_EXPORT_PLUGIN(ImageProcessing)