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

		ServiceContainer(ServicePtr _service);
		
		void start();
		void stop();
		void run();

		bool setup(ConfigMetadataPtr const& _config);
		void shutdown();
		void update();

		void addListener(ServicePtr _listener);
		void removeListener(ServicePtr _listener);
		//this one is called by the contained service obj
		void serviceListener(DataPtr &_input);

		void addListener(ContainerPtr _listener);
		void removeListener(ContainerPtr _listener);
		//this one is called by other containers
		void containerListener(DataPtr &_input);

		void outputData(bool _blocking);
		
	private:

		typedef std::list< VariableName > NameList;

		std::list< DataPtr >			m_Data;
		Poco::Mutex						m_Lock;

		bool							m_bRunThreaded;
		bool							m_bIsConfigured;

		Poco::BasicEvent< DataPtr >		m_OutputEvent;
	
		NameList						m_InputVariables;
		
		ServicePtr						m_ServicePtr;
	};
}