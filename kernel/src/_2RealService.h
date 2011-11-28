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

#include "Poco/Timestamp.h"

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

		SharedAny getParameterValue(std::string const& name);
		InputHandle createInputHandle(std::string const& name);
		OutputHandle createOutputHandle(std::string const& name);

		void addInputSlot(InputSlot *const slot);
		void addOutputSlot(OutputSlot *const slot);
		void addSetupParameter(SetupParameter *const parameter);

		void checkConfiguration();
		void run();
		void update();
		void shutdown();
		
		std::list< Identifier > setupParameterIDs() const;
		std::list< Identifier > inputSlotIDs() const;
		std::list< Identifier > outputSlotIDs() const;

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

		const bool hasInputSlot(std::string const& name) const;
		const bool hasOutputSlot(std::string const& name) const;
		const bool hasSetupParameter(std::string const& name) const;

		OutputSlot *const getOutputSlot(std::string const& name);
		InputSlot *const getInputSlot(std::string const& name);
		SetupParameter *const getSetupParameter(std::string const& name);

		void setUpdateRate(float const& updatesPerSecond);

	private:

		InputMap											m_InputSlots;
		OutputMap											m_OutputSlots;
		ParameterMap										m_SetupParameters;

		IService											*m_Service;

		Poco::Timestamp										m_Timer;
		long												m_MaxDelay;
		float												m_UpdatesPerSecond;

	};

}