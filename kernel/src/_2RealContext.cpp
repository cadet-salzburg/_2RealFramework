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

	void _2RealContext::update()
	{
		for (std::map<std::string, _2RealServicePtr>::iterator it = m_PrivateContext->m_Services.begin(); it != m_PrivateContext->m_Services.end(); it++)
		{
			std::cout << "updating service: " << it->first << std::endl;

			//todo: check metadata to find out if services should actually be threaded
			_2RealIService* service = it->second.get();
			Poco::ThreadPool::defaultPool().start(*service);
		}

		Poco::ThreadPool::defaultPool().joinAll();
	}

	void _2RealContext::start(const std::vector<std::string>& _plugins)
	{
		//load all plugins & create service chain for testing

		std::string thingie = std::string();
		_2RealServicePtr previous;
		previous.assign(NULL);
		
		for (std::vector<std::string>::const_iterator it = _plugins.begin(); it != _plugins.end(); it++)
		{
			_2RealPluginPtr p = m_PrivateContext->installPlugin(*it, Poco::Path::current() + "..\\..\\testplugins\\");
			p->load();
			p->start();
			
			_2RealServicePtr s = p->createService(thingie);
			if (!previous.isNull())
			{
				s->addListener(previous);
			}

			previous = s;
		}
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