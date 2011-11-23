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

#include "_2RealRunnable.h"
//#include "_2RealInputHandle.h"
//#include "_2RealOutputHandle.h"
//#include "_2RealSharedAny.h"

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
	class SetupParameter;
	class InputHandle;
	class OutputHandle;
	class SharedAny;

	typedef std::pair< std::string, InputSlot * >		NamedInput;
	typedef std::map< std::string, InputSlot * >		InputMap;
	typedef std::pair< std::string, OutputSlot * >		NamedOutput;
	typedef std::map< std::string, OutputSlot * >		OutputMap;
	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;

	class Service : public Runnable
	{

	public:

		Service(IService *const service, Identifier const& id, SystemGraph *const system);
		virtual ~Service();

		/**
		*	functions for service context
		*/
		SharedAny getParameterValue(std::string const& name);
		InputHandle createInputHandle(std::string const& name);
		OutputHandle createOutputHandle(std::string const& name);

		/**
		*	functions for service factory
		*/
		void addInputSlot(InputSlot *const slot);
		void addOutputSlot(OutputSlot *const slot);
		void addSetupParameter(SetupParameter *const parameter);

		void checkConfiguration();
		void run();
		void update();
		void shutdown();
		
		std::list< unsigned int > setupParameterIDs() const;
		std::list< unsigned int > inputSlotIDs() const;
		std::list< unsigned int > outputSlotIDs() const;

		InputMap & inputSlots()
		{
			return m_InputSlots;
		}

		OutputMap & outputSlots()
		{
			return m_OutputSlots;
		}

		ParameterMap & setupParameters()
		{
			return m_SetupParameters;
		}

		InputMap const& inputSlots() const
		{
			return m_InputSlots;
		}

		OutputMap const& outputSlots() const
		{
			return m_OutputSlots;
		}

		ParameterMap const& setupParameters() const
		{
			return m_SetupParameters;
		}

		const bool hasParameters() const
		{
			return true;
		}

		OutputSlot *const getOutputSlot(std::string const& name);
		InputSlot *const getInputSlot(std::string const& name);
		SetupParameter *const getSetupParameter(std::string const& name);

	private:

		InputMap											m_InputSlots;
		OutputMap											m_OutputSlots;
		ParameterMap										m_SetupParameters;

		IService											*m_Service;

	};

}