/*
 * 
 * m.
 * 
 */

#include "ImageViewer.h"
#include "ImageViewerActivator.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include <iostream>

using namespace _2Real;

void ImageViewer::getMetadata(PluginMetadata &_info)
{
	MetadataReader reader(_info);
	reader.readMetadata();
}

void ImageViewer::setup(PluginContext &context)
{
	// has to match the name of the service in the xml file
	context.registerService("DisplayWindow", ::createservice_displaywindow);
}

ImageViewer::~ImageViewer()
{
}

//exportiert die klasse
_2REAL_EXPORT_PLUGIN(ImageViewer)