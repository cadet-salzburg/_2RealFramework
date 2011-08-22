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

#include "_2RealAbstractRef.h"
#include "_2RealAbstractContainer.h"

#include <list>
#include <map>

namespace _2Real
{

	/**
	*	service container
	*	makes sure the user service is updated and stuff
	*/

	class IService;
	class ServiceParam;
	class Identifier;

	class ServiceImpl : public AbstractContainer
	{

	public:

		/**
		*	returns value of setup param
		*/
		void getParameterValue(std::string const& _name, AbstractRef *const _param) throw(...);

		/**
		*	registers ref of input variable
		*/
		void registerInputSlot(std::string const& _name, AbstractRef *const _var) throw(...);

		/**
		*	registers ref of output variable
		*/
		void registerOutputSlot(std::string const& _name, AbstractRef *const _var) throw(...);

		/**
		*	
		*/
		typedef std::list< Identifier > IdentifierList;

		/**
		*	throws if user defined service is null
		*/
		ServiceImpl(IService *const _service, IdentifierImpl *const _id) throw(...);

		/**
		*	this one should not be called, ever
		*/
		ServiceImpl(ServiceImpl const& _src) throw(...);

		/**
		*	this one should not be called, ever
		*/
		ServiceImpl& operator=(ServiceImpl const& _src) throw(...);

		/**
		*	destructor also performs shutdown & destroys user defined service
		*/
		~ServiceImpl();

		/**
		*	called by service factory after service creation
		*/
		void addParam(ServiceParam *const _param) throw(...);

		/**
		*	calls setup function of service when called for the first time
		*/
		void checkConfiguration() throw(...);

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

	private:

		/**
		*	
		*/
		typedef std::pair< const unsigned int, ServiceParam * >	NamedInput;

		/**
		*	
		*/
		typedef std::map< const unsigned int, ServiceParam * >	InputMap;

		/**
		*	
		*/
		typedef std::pair< const std::string, ServiceParam * >	NamedParam;

		/**
		*	
		*/
		typedef std::map< const std::string, ServiceParam * >	ParamMap;

		/**
		*	input slots - are found by sender id
		*/
		InputMap												m_InputParams;

		/**
		*	output slots
		*/
		ParamMap												m_OutputParams;

		/**
		*	setup params
		*/
		ParamMap												m_SetupParams;

		/**
		*	the user defined service
		*/
		IService												*m_Service;

	};

}