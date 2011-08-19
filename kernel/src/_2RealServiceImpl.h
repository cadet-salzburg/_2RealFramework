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

#include "_2RealAbstractValue.h"
#include "_2RealAbstractContainer.h"

#include <list>

namespace _2Real
{

	/**
	*	service container
	*/

	class IService;
	class Identifier;

	class ServiceImpl : public AbstractContainer
	{

		friend class ServiceFactory;

	public:

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

		/**
		*	throws if user defined service is null
		*/
		ServiceImpl(IdentifierImpl const& _id, IService *const _service) throw(...);

		/**
		*	this one should not be called, ever
		*/
		ServiceImpl(ServiceImpl const& _src) throw(...);

		/**
		*	this one should never be called either
		*/
		ServiceImpl& operator=(ServiceImpl const& _src) throw(...);

		/**
		*	destructor also performs shutdown & destroys user defined service
		*/
		~ServiceImpl();

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
		typedef std::list< AbstractValue * >	VariableList;

		/**
		*	input variables registred by user defined service
		*/
		VariableList							m_InputVariables;
		
		/**
		*	output variables registred by user defined service
		*/
		VariableList							m_OutputVariables;

		/**
		*	the user defined service
		*/
		IService								*m_Service;

	};

}