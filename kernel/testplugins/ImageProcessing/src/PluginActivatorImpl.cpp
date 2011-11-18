#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "ImageAccumulation.h"
#include "ImageConversion.h"

#include <iostream>

using namespace _2Real;
using namespace std;

void ImageProcessing::getMetadata(PluginMetadata &_info)
{
	MetadataReader reader(_info);
	reader.readMetadata();
}

void ImageProcessing::setup(PluginContext &_context)
{
	_context.registerService("ImageAccumulation_uchar", &::createImageAccumulation< unsigned char >);
	//_context.registerService("ImageAccumulation", &::createImageAccumulation< float >);
	_context.registerService("ToFloat_uchar", &::createFloatConversion< unsigned char >);
}

_2REAL_EXPORT_PLUGIN(ImageProcessing)