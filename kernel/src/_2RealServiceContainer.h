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
	class InputSlot;
	class OutputSlot;
	class ServiceParameter;

	class ServiceContainer : public AbstractContainer
	{

	public:

		/**
		*	returns value of setup param
		*/
		void getParameterValue(std::string const& _name, AbstractRef *const _param);

		/**
		*	registers ref of input variable
		*/
		void registerInputSlot(std::string const& _name, AbstractRef *const _var);

		/**
		*	registers ref of output variable
		*/
		void registerOutputSlot(std::string const& _name, AbstractRef *const _var);

		/**
		*	throws if user defined service is null
		*/
		ServiceContainer(IService *const _service, Id *const _id, ApplicationCallback *const _output);

		/**
		*	this one should not be called, ever
		*/
		ServiceContainer(ServiceContainer const& _src);

		/**
		*	this one should not be called, ever
		*/
		ServiceContainer& operator=(ServiceContainer const& _src);

		/**
		*	destructor also performs shutdown & destroys user defined service
		*/
		~ServiceContainer();

		/**
		*	called by service factory after service creation
		*/
		void addInputSlot(unsigned int const& id, InputSlot *const _slot);

		/**
		*	called by service factory after service creation
		*/
		void addOutputSlot(unsigned int const& id, OutputSlot *const _slot);

		/**
		*	called by service factory after service creation
		*/
		void addSetupValue(unsigned int const& id, ServiceParameter *const _value);

		/**
		*	calls setup function of service when called for the first time
		*/
		void checkConfiguration();

		/**
		*	
		*/
		void run();

		/**
		*	
		*/
		void update();

		/**
		*	
		*/
		void shutdown();

		/**
		*	
		*/
		IDs setupParamIDs() const;

		/**
		*	
		*/
		IDs inputSlotIDs() const;

		/**
		*	
		*/
		IDs outputSlotIDs() const;

		/**
		*	
		*/
		std::list< InputSlot * > inputSlots();

		/**
		*	
		*/
		std::list< OutputSlot * > outputSlots();

		/**
		*	
		*/
		std::list< ServiceParameter * > setupParams();

	private:

		typedef std::pair< std::string, InputSlot * >				NamedInput;
		typedef std::map< std::string, InputSlot * >				InputMap;
		typedef std::pair< std::string, OutputSlot * >				NamedOutput;
		typedef std::map< std::string, OutputSlot * >				OutputMap;
		typedef std::pair< std::string, ServiceParameter * >		NamedParam;
		typedef std::map< std::string, ServiceParameter * >			ParamMap;

		/**
		*	input slots as map
		*/
		InputMap													m_InputParams;

		/**
		*	
		*/
		IDs															m_InputIds;

		/**
		*	
		*/
		std::list< InputSlot * >									m_InputSlots;

		/**
		*	output slots
		*/
		OutputMap													m_OutputParams;

		/**
		*	
		*/
		IDs															m_OutputIds;

		/**
		*	
		*/
		std::list< OutputSlot * >									m_OutputSlots;

		/**
		*	setup value
		*/
		ParamMap													m_SetupParams;

		/**
		*	
		*/
		IDs															m_SetupIds;

		/**
		*	
		*/
		std::list< ServiceParameter * >								m_SetupValues;

		/**
		*	the user defined service
		*/
		IService													*m_Service;

	};

}