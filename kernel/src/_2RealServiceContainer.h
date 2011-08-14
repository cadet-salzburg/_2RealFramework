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

#include "_2RealAbstractContainer.h"
#include "_2RealAbstractValue.h"

#include <list>

namespace _2Real
{

	/**
	*	container class
	*	each service container takes care of exactly one user defined service
	*/

	class ServiceContainer : public AbstractContainer
	{

	public:

		ServiceContainer(ContainerName const& _name);
		ServiceContainer(ServiceContainer const& _src);
		ServiceContainer& operator=(ServiceContainer const& _src);
		~ServiceContainer();

		/**
		*	configures container - calls setup function of service
		*/
		void configure(ConfigurationData *const _dataPtr) throw(...);

		/**
		*
		*/
		void run() throw(...);

		/**
		*
		*/
		void update() throw(...);

		/**
		*
		*/
		void shutdown() throw(...);

		/**
		*
		*/
		void getParameterValue(AbstractValue *const _param);

		/**
		*
		*/
		void registerInputVariable(AbstractValue *const _var);

		/**
		*
		*/
		void registerOutputVariable(AbstractValue *const _var);

	private:

		typedef std::list< AbstractValue * >	VariableList;

		VariableList							m_InputVariables;
		VariableList							m_OutputVariables;

		/**
		*	the user defined service
		*/
		IService								*m_ServicePtr;

	};

}