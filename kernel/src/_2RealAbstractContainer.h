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

namespace _2Real
{
	class AbstractContainer : public IContainer
	{

	public:

		AbstractContainer(ServiceName _name) : m_Name(_name), m_bIsConfigured(false), m_bRunThreaded(false), m_bRunOnceOnly(false) {}

		virtual void start(bool _loop) = 0;
		virtual void stop() = 0;
		virtual void run() = 0;
		virtual void update() = 0;
		virtual bool setup(ConfigMetadataPtr const& _config) = 0;
		virtual void shutdown() = 0;
		
		virtual void addListener(ServicePtr _listener) = 0;
		virtual void removeListener(ServicePtr _listener) = 0;
		virtual void serviceListener(DataPtr &_input) = 0;
		
		/**
		*	functions for communication between containers -
		*	might be useless; placeholder for now
		*/
		void addListener(ContainerPtr _listener) {}
		void removeListener(ContainerPtr _listener) {}
		void containerListener(DataPtr &_input) {}
		
		/**
		*	function to output data - unused
		*/
		void outputData(bool _blocking) {}
		
		ServiceName const& name() const { return m_Name; }

	protected:

		bool								m_bRunThreaded;
		bool								m_bRunOnceOnly;
		bool								m_bIsConfigured;

		/**
		*	every container is a service; thus it receives a name when created
		*/
		ServiceName							m_Name;

	};
}