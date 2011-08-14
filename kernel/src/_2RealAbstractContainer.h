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

#include "_2RealIContainer.h"
#include "_2RealVariableName.h"
#include "_2RealContainerName.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	/**
	*
	*/

	class AbstractContainer : public IContainer
	{

	public:

		AbstractContainer(ContainerName const& _name);
		AbstractContainer(AbstractContainer const& _src);
		AbstractContainer& operator=(AbstractContainer const& _src);
		~AbstractContainer();

		/**
		*	returns name
		*/
		ContainerName const& name() const;

		/**
		*	functions inherited from IContainer
		*/
		
		/**
		*
		*/
		void start(bool const& _runOnce);

		/**
		*
		*/
		void stop();

		/**
		*
		*/
		bool const& canBeReconfigured() const;

		/**
		*	functions inherited from IService
		*/

		/**
		*
		*/
		void setup(ServiceContext *const _contextPtr) throw(...);

		/**
		*	functions inherited from IDataQueue
		*/

		/**
		*	
		*/
		void addListener(IDataQueue *const _queue);
		
		/**
		*
		*/
		void removeListener(IDataQueue *const _queue);
		
		/**
		*
		*/
		void receiveData(NamedData &_data);

		/**
		*
		*/
		void sendData(bool const& _blocking);

	protected:

		bool								m_bRunOnce;
		bool								m_bRun;
		bool								m_bIsConfigured;
		bool								m_bCanReconfigure;

		ConfigurationData					*m_ConfigurationPtr;

		std::list< NamedData >				m_DataList;

		Poco::BasicEvent< NamedData >		m_NewData;

	private:

		ContainerName						m_Name;

	};

}