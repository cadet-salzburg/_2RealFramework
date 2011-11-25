#include "OpenCVvideoplayer.h"
#include "OpenCVvideoplayerActivator.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include <iostream>

using namespace _2Real;

void OpenCVvideoplayer::getMetadata(PluginMetadata &_info)
{
	MetadataReader reader(_info);
	reader.readMetadata();
}

void OpenCVvideoplayer::setup(PluginContext &context)
{
	// has to match the name of the service in the xml file
	context.registerService("playvideo", ::createservice_playvideo);
}

OpenCVvideoplayer::~OpenCVvideoplayer()
{
}

//exportiert die klasse
_2REAL_EXPORT_PLUGIN(OpenCVvideoplayer)