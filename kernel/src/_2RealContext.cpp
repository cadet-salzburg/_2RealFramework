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

#include "Poco/ThreadPool.h"
#include "Poco/Path.h"

namespace _2Real
{
	bool _2RealContext::s_bIsInstanced = false;
	_2RealContextPtr _2RealContext::s_ContextPtr;
	Poco::Mutex _2RealContext::s_Mutex;

	_2RealContextPtr _2RealContext::instance()
	{
		Poco::ScopedLock<Poco::Mutex> lock(s_Mutex);
		if(!s_bIsInstanced)
		{
			s_ContextPtr = _2RealContextPtr(new _2RealContext());
			s_bIsInstanced = true;
			return s_ContextPtr;
		}
		else
		{
			return s_ContextPtr;
		}
	}

	_2RealContext::_2RealContext()
	{
		m_PrivateContext = new _2RealContextPrivate();
	}

	_2RealContext::~_2RealContext()
	{
		delete m_PrivateContext;
	}

	void _2RealContext::config()
	{
	}

	bool _2RealContext::update()
	{
		for (std::map<std::string, _2RealServicePtr>::iterator it = m_PrivateContext->m_Services.begin(); it != m_PrivateContext->m_Services.end(); it++)
		{
			//todo: check metadata to find out if services should actually be threaded
			//also, use different thread pool
			_2RealIService* service = it->second.get();
			Poco::ThreadPool::defaultPool().start(*service);
		}
		
		Poco::ThreadPool::defaultPool().joinAll();
		return true;
	}

	void _2RealContext::start()
	{
		_2RealPluginPtr pImg = m_PrivateContext->installPlugin("ImageProcessing", Poco::Path::current() + "..\\..\\testplugins\\");
		pImg->load();
		pImg->start();
	
		//is null
		_2RealMetadataPtr mImg = pImg->metadata();
		
		_2RealConfigMetadataPtr cRandom(new _2RealConfigMetadata("RandomImage"));

		cRandom->setSetupParameter<std::string>("T", "unsigned short");
		cRandom->setSetupParameter<std::string>("service name", "ImageProcessing.RandomImage");
		cRandom->setSetupParameter<unsigned int>("image width", 320);
		cRandom->setSetupParameter<unsigned int>("image height", 240);
		
		cRandom->setOutputParameter<std::string>("output image", "ImageProcessing.RandomImage.image");

		_2RealServicePtr sRandom = pImg->createService(cRandom);

		_2RealConfigMetadataPtr cAddition(new _2RealConfigMetadata("ImageAddition"));

		cAddition->setSetupParameter<std::string>("T", "unsigned short");
		cAddition->setSetupParameter<std::string>("service name", "ImageProcessing.ImageAddition");
		cAddition->setSetupParameter<unsigned short>("scale factor 1", 1);
		cAddition->setSetupParameter<unsigned short>("scale factor 2", 1);

		cAddition->setInputParameter<std::string>("input image 1", "ImageProcessing.RandomImage.image");
		cAddition->setInputParameter<std::string>("input image 2", "KinectWinSDK.Depthmap.image");

		cAddition->setOutputParameter<std::string>("output image", "ImageProcessing.RandomImage.image");

		_2RealServicePtr sAddition = pImg->createService(cAddition);

		sRandom->addListener(sAddition);

		_2RealPluginPtr pKinect = m_PrivateContext->installPlugin("KinectWinSDK", Poco::Path::current() + "..\\..\\testplugins\\");
		pKinect->load();
		pKinect->start();

		//is null
		_2RealMetadataPtr mKinect = pKinect->metadata();

		_2RealConfigMetadataPtr cDepthmap(new _2RealConfigMetadata("Depthmap"));

		cDepthmap->setSetupParameter<std::string>("service name", "KinectWinSDK.Depthmap");
		cDepthmap->setSetupParameter<unsigned int>("image width", 320);
		cDepthmap->setSetupParameter<unsigned int>("image height", 240);

		cDepthmap->setOutputParameter<std::string>("output image", "KinectWinSDK.Depthmap.image");

		_2RealServicePtr sDepthmap = pKinect->createService(cDepthmap);

		sDepthmap->addListener(sAddition);
	}

	void _2RealContext::stop()
	{
		for (std::map<std::string, _2RealPluginPtr>::iterator it = m_PrivateContext->m_Plugins.begin(); it != m_PrivateContext->m_Plugins.end(); it++)
		{
			(it->second)->stop();
			(it->second)->unload();
			(it->second)->uninstall();
		}
	}
}