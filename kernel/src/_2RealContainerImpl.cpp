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

#include "_2RealContainerImpl.h"

namespace _2Real
{

	ContainerImpl::ContainerImpl(ContainerName const& _name) : AbstractContainer(_name)
	{
	}

	ContainerImpl::ContainerImpl(ContainerImpl const& _src) : AbstractContainer(_src)
	{
#ifdef _DEBUG
		std::cout << "production container container copy constructor called" << std::endl;
#endif
	}

	ContainerImpl& ContainerImpl::operator=(ContainerImpl const& _src)
	{
		AbstractContainer::operator=(_src);

#ifdef _DEBUG
		std::cout << "production container assignment operator called" << std::endl;
#endif

		return *this;
	}

	ContainerImpl::~ContainerImpl()
	{
		//NOTE-TO-SELF:
		//child ptrs are not owning
		//services must be deleted by factory
	}

	void ContainerImpl::configure(ConfigurationData *const _dataPtr) throw(...)
	{
		if (!m_bIsConfigured || m_bCanReconfigure)
		{
			//TODO: throw
			return;
		}

		for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			try
			{
				//TODO: pass on configuration data;
			}
			catch(...)
			{
				//TODO::error handling
			}
		}

		for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			try
			{
				(*it)->update();
			}
			catch(...)
			{
				//TODO::error handling
			}
		}

		//save configuration
		m_ConfigurationPtr = _dataPtr;
	}

	void ContainerImpl::run() throw(...)
	{
		while (m_bRun || m_bRunOnce)
		{

			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				try
				{
					(*it)->update();
				}
				catch(...)
				{
					//TODO::error handling
				}
			}

			m_bRunOnce = false;
			sendData(false);
		}
	}

	void ContainerImpl::update() throw(...)
	{
		if (!m_bIsConfigured)
		{
			return;
		}

		for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			try
			{
				(*it)->update();
			}
			catch(...)
			{
				//TODO::error handling
			}
		}

		//output vars should have updated data now
		//blocking send -> function returns when listeners have been notified
		sendData(true);
	}

	void ContainerImpl::shutdown() throw(...)
	{
		for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			try
			{
				(*it)->shutdown();
			}
			catch(...)
			{
				//TODO::error handling
			}
		}
	}

}