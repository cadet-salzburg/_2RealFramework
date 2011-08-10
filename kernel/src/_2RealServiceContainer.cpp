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

#include "_2RealServiceContainer.h"
#include "_2RealData.h"
#include "_2RealConfigMetadata.h"

#include "Poco/Delegate.h"

namespace _2Real
{
	void ServiceContainer::start(bool const& _loop)
	{
		if (!m_bIsConfigured)
		{
			return;
		}

		if (!_loop)
		{
			m_bRunOnceOnly = true;
		}
		else
		{
			m_bRunThreaded = true;
		}
	}

	void ServiceContainer::stop()
	{
		m_bRunThreaded = false;
	}

	void ServiceContainer::update()
	{
		m_Lock.lock();
		
		if (m_bIsConfigured)
		{
			VariableList tmpList = m_InputVariables;
			
			DataPtr data(new Data());
			for (std::list< DataPtr >::reverse_iterator it = m_Data.rbegin(); it != m_Data.rend(); it++)
			{
				Data tmp = *it->get();
				for (VariableList::iterator it = tmpList.begin(); it != tmpList.end(); it++)
				{
					Poco::Any result;
					if (tmp.getAny(*it, result))
					{
						data->insertAny(*it, result);
					}
					else
					{
					}
				}
			}

			m_ServicePtr->serviceListener(data);
			
			m_ServicePtr->update();
			
			m_ServicePtr->outputData(true);

			m_Data.clear();
		}

		m_Lock.unlock();
	}

	void ServiceContainer::run()
	{
		while (m_bRunThreaded || m_bRunOnceOnly)
		{
			update();
			m_bRunOnceOnly = false;
		}
	}

	const bool ServiceContainer::setup(ConfigMetadataPtr const& _config)
	{
		if (m_bIsConfigured)
		{
			return false;
		}

		//m_Name = _config->name();

		const Data variables = _config->child("input params")->attributes();
		for (Data::DataItems::const_iterator it = variables.begin(); it != variables.end(); it++)
		{
			const Variable name = Poco::AnyCast< Variable >(it->second);
			m_InputVariables.push_back(name);
		}

		m_bIsConfigured = m_ServicePtr->setup(_config);
		return m_bIsConfigured;
	}

	void ServiceContainer::shutdown()
	{
		m_ServicePtr->shutdown();
	}

	void ServiceContainer::addListener(ServicePtr &_listener)
	{
		m_ServicePtr->addListener(_listener);
	}

	void ServiceContainer::removeListener(ServicePtr &_listener)
	{
		m_ServicePtr->removeListener(_listener);
	}

	void ServiceContainer::serviceListener(DataPtr &_input)
	{
		m_Lock.lock();
		m_Data.push_back(_input);
		m_Lock.unlock();
	}
}