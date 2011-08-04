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

#include "_2RealFramework.h"
#include "_2RealIContainer.h"

#include "Poco/BasicEvent.h"

namespace _2Real
{
	class ServiceContainer : public IContainer
	{
	public:

		ServiceContainer();
		
		void start();
		void stop();
		void run();

		bool setup(ConfigMetadataPtr const& _config);
		void shutdown();
		void update();

		//these are not needed
		void addListener(ServicePtr _listener) {}
		void removeListener(ServicePtr _listener) {}
		void serviceListener(DataPtr &_input) {}

		void addListener(ContainerPtr _listener);
		void removeListener(ContainerPtr _listener);
		void containerListener(DataPtr &_input);

		void outputData(bool _blocking);

		//functions to build sequence
		void beginSequence(ContainerPtr _first)
		{
			if (m_bHasFirstElement)
			{
				return;
			}

			m_Containers.push_back(_first);
			//add container to listeners
			m_bHasFirstElement = true;
		}

		//functions to build sequence
		void endSequence(ContainerPtr _last)
		{
			if (m_bHasLastElement)
			{
				return;
			}

			m_Containers.push_back(_last);
			//add container to listeners
			m_bHasLastElement = true;
		}

		void addElement(ContainerPtr _element)
		{
			if (m_bHasFirstElement && !m_bHasLastElement)
			{
				m_Containers.push_back(_element);
			}

			//hmmm should this thing also create the connections between 
		}

		ContainerPtr first() { return *m_Containers.begin(); }
		ContainerPtr last() { return *m_Containers.end(); }

	private:

		typedef std::list< ContainerPtr > ContainerSequence;

		//list of input containers
		ContainerSequence		m_Containers;

		//std::list< DataPtr >			m_Data;
		Poco::Mutex						m_Lock;

		bool							m_bRunThreaded;
		bool							m_bIsConfigured;

		Poco::BasicEvent< DataPtr >		m_OutputEvent;
		
		ServicePtr						m_ServicePtr;

		bool m_bHasFirstElement;
		bool m_bHasLastElement;

	};
}