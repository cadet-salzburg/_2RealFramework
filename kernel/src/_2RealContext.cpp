/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH

		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "_2RealContext.h"
#include "_2RealContextPrivate.h"
#include "_2RealIService.h"
#include "_2RealPlugin.h"
#include "_2RealConfigMetaData.h"

#include "Poco/Path.h"

namespace _2Real
{
	bool Context::s_bIsInstanced = false;
	ContextPtr Context::s_ContextPtr;
	Poco::Mutex Context::s_Mutex;

	ContextPtr Context::instance()
	{
		Poco::ScopedLock<Poco::Mutex> lock(s_Mutex);
		if(!s_bIsInstanced)
		{
			s_ContextPtr = ContextPtr(new Context());
			s_bIsInstanced = true;
			return s_ContextPtr;
		}
		else
		{
			return s_ContextPtr;
		}
	}

	Context::Context()
	{
		m_PrivateContext = new ContextPrivate();
	}

	Context::~Context()
	{
		delete m_PrivateContext;
	}

	void Context::config()
	{
	}

	bool Context::update()
	{
		return m_PrivateContext->updateServiceRegistry();
	}

	void Context::start()
	{
		PluginPtr pImg = m_PrivateContext->installPlugin("ImageProcessing", Poco::Path::current() + "..\\..\\testplugins\\");
		pImg->load();
		pImg->start();

		//MetadataPtr mImg = pImg->metadata();
		
		ConfigMetadataPtr cRandom(new ConfigMetadata("RandomImage_ushort"));

		cRandom->setSetupParameter<std::string>("service name", "ImageProcessing.RandomImage_ushort");
		cRandom->setSetupParameter<unsigned int>("image width", 320);
		cRandom->setSetupParameter<unsigned int>("image height", 240);
		
		cRandom->setOutputParameter<std::string>("output image", "ImageProcessing.RandomImage_ushort.image");

		ServicePtr sRandom = m_PrivateContext->createService("RandomImage_ushort");

		if (sRandom.isNull())
		{
			std::cout << "random service is NULL" << std::endl;
			return;
		}

		if (!sRandom->setup(cRandom))
		{
			std::cout << "failed to setup random img service" << std::endl;
			return;
		}

		ConfigMetadataPtr cAddition(new ConfigMetadata("ImageAddition_ushort"));

		cAddition->setSetupParameter<std::string>("service name", "ImageProcessing.ImageAddition_ushort");
		cAddition->setSetupParameter<unsigned short>("scale factor 1", 1);
		cAddition->setSetupParameter<unsigned short>("scale factor 2", 1);

		cAddition->setInputParameter<std::string>("input image 1", "ImageProcessing.RandomImage_ushort.image");
		cAddition->setInputParameter<std::string>("input image 2", "KinectWinSDK.Depthmap.image");

		cAddition->setOutputParameter<std::string>("output image", "ImageProcessing.ImageAddition_ushort.image");

		ServicePtr sAddition = m_PrivateContext->createService("ImageAddition_ushort");

		if (sAddition.isNull())
		{
			std::cout << "img addition service is NULL" << std::endl;
			return;
		}
		
		if (!sAddition->setup(cAddition))
		{
			std::cout << "failed to setup img addition service" << std::endl;
			return;
		}

		sRandom->addListener(sAddition);

		PluginPtr pKinect = m_PrivateContext->installPlugin("KinectWinSDK", Poco::Path::current() + "..\\..\\testplugins\\");
		pKinect->load();
		pKinect->start();

		//MetadataPtr mKinect = pKinect->metadata();

		ConfigMetadataPtr cDepthmap(new ConfigMetadata("Depthmap"));

		cDepthmap->setSetupParameter<std::string>("service name", "KinectWinSDK.Depthmap");
		cDepthmap->setSetupParameter<unsigned int>("image width", 320);
		cDepthmap->setSetupParameter<unsigned int>("image height", 240);

		cDepthmap->setOutputParameter<std::string>("output image", "KinectWinSDK.Depthmap.image");

		ServicePtr sDepthmap = m_PrivateContext->createService("Depthmap");

		if (sDepthmap.isNull())
		{
			std::cout << "depthmap service is NULL" << std::endl;
			return;
		}

		if (!sDepthmap->setup(cDepthmap))
		{
			std::cout << "failed to setup depthmap service" << std::endl;
			return;
		}

		sDepthmap->addListener(sAddition);
	}

	void Context::stop()
	{
		for (std::map<std::string, PluginPtr>::iterator it = m_PrivateContext->m_Plugins.begin(); it != m_PrivateContext->m_Plugins.end(); it++)
		{
			(it->second)->stop();
			(it->second)->unload();
			(it->second)->uninstall();
		}
	}
}