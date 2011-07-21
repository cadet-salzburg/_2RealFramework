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
#include "_2RealPlugin.h"
#include "_2RealIService.h"
#include "_2RealContextPrivate.h"
#include "_2RealMetaData.h"

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
		_2RealServicePtr depth, rnd, add;

		_2RealPluginPtr imgproc = m_PrivateContext->installPlugin("ImageProcessing", Poco::Path::current() + "..\\..\\testplugins\\");
		imgproc->load();
		imgproc->start();

		std::cout << "started image processing plugin" << std::endl;

		_2RealData addInit;
		addInit.insert<std::string>("T", "unsigned short");

		_2RealData addSetup;
		addSetup.insert<unsigned short>("scaleFactor1", 1);
		addSetup.insert<unsigned short>("scaleFactor2", 1);
		addSetup.insert<unsigned short>("backgroundColor", 0);
		addSetup.insert<std::string>("serviceName", "ImageProcessing.ImageAddition");
		_2RealMetaData addMetaData;
		addMetaData.set("inputImage1", "KinectWinSDK.DepthMap.outputImage");
		addMetaData.set("inputImage2", "ImageProcessing.RandomImage.outputImage");
		addSetup.insert<_2RealMetaData>("inputMetaData", addMetaData);

		_2RealData rndInit;
		rndInit.insert<std::string>("T", "unsigned short");

		_2RealData rndSetup;
		rndSetup.insert<unsigned int>("imageWidth", 320);
		rndSetup.insert<unsigned int>("imageHeight", 240);
		rndSetup.insert<std::string>("serviceName", "ImageProcessing.RandomImage");

		add = imgproc->createService("ImageAddition", addInit);
		if (add.isNull())
		{
			std::cout << "could not create image addition service" << std::endl;
		}

		add->setup(addSetup);

		std::cout << "created image addition service" << std::endl;

		rnd = imgproc->createService("RandomImage", rndInit);
		if (rnd.isNull())
		{
			std::cout << "could not create random image service" << std::endl;
		}
		rnd->setup(rndSetup);
		rnd->addListener(add);

		std::cout << "created random image service" << std::endl;

		_2RealPluginPtr kinect = m_PrivateContext->installPlugin("KinectWinSDK", Poco::Path::current() + "..\\..\\testplugins\\");
		kinect->load();
		kinect->start();

		std::cout << "started kinect plugin" << std::endl;

		_2RealData depthInit;

		_2RealData depthSetup;
		depthSetup.insert<unsigned int>("imageWidth", 320);
		depthSetup.insert<unsigned int>("imageHeight", 240);
		depthSetup.insert<std::string>("serviceName", "KinectWinSDK.DepthMap");

		depth = kinect->createService("DepthMap", depthInit);
		depth->setup(depthSetup);
		depth->addListener(add);

		std::cout << "created depthmap service" << std::endl;
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