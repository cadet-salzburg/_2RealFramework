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

#pragma once

#include "_2RealGroupContainer.h"

namespace _2Real
{
	bool GroupContainer::setup(ConfigMetadataPtr const& _config)
	{
		//cannot be reconfigured; meaning all of its child containers cannot be reconfigured either. this might change at a later time.
		if (m_bIsConfigured)
		{
			std::cout << "group container " << m_Name << " setup: is already configured" << std::endl;
			return false;
		}

		//every group container must contain at least one child container
		if (m_Containers.empty())
		{
			std::cout << "group container " << m_Name << " setup: container list is empty" << std::endl;
			return false;
		}

		bool success = true;

		//is a service, thus receives a name in the framework
		if (m_Name != _config->name())
		{
			std::cout << "group container " << m_Name << " setup: configuration metadata's name does not match" << std::endl;
			success &= false;
		}

		//group containers offer no service themselves; thus _config's setup/input/output params are ignored
		//there must be a child config metadata in _config for each child container, with a matching name
		for (ContainerList::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
		{
			ServiceName name = (*it)->name();
			MetadataPtr meta = _config->child(name);

			if (meta.isNull())
			{
				//this should not happen; todo: exception handling
				std::cout << "group container " << m_Name << " setup: configuration metadata for " << name << " is null." << std::endl;
				success &= false;
			}

			ConfigMetadataPtr config = meta.unsafeCast< ConfigMetadata >();

			if (config.isNull())
			{
				//this should not happen either
				std::cout << "group container " << m_Name << " setup: metadata for " << name << " is no configuration." << std::endl;
				success &= false;
			}

			success &= (*it)->setup(config);
		}

		//change capacity of threadpool to match nr of children
		unsigned int children = m_Containers.size();
		unsigned int capacity = m_ThreadPool.capacity();
		m_ThreadPool.addCapacity(children - capacity);

		if (success)
		{
			m_bIsConfigured = true;
			return true;
		}
		
		return false;
	}

	void GroupContainer::shutdown()
	{
		for (ContainerList::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
		{
			(*it)->shutdown();
		}
	}

	void GroupContainer::start(bool _loop)
	{
		if (!m_bIsConfigured)
		{
			return;
		}

		if (_loop)
		{
			m_bRunThreaded = true;
		}
		else
		{
			m_bRunOnceOnly = true;
		}
	}

	void GroupContainer::stop()
	{
		m_bRunThreaded = false;
	}

	void GroupContainer::run()
	{
		while (m_bRunThreaded || m_bRunOnceOnly)
		{
			update();
			m_bRunOnceOnly = false;
		}
	}

	bool GroupContainer::addElement(ContainerPtr _container)
	{
		if (m_bIsConfigured)
		{
			return false;
		}

		for (ContainerList::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
		{
			if ((*it)->name() == _container->name())
			{
				std::cout << "group container " << m_Name << " add element: container " << _container->name() << " already exists" << std::endl;
				return false;
			}
		}

		m_Containers.push_back(_container);
		return true;
	}
}