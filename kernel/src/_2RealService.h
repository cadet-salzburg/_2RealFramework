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

#include <map>

namespace _2Real
{

	class IService;
	class InputSlot;
	class OutputSlot;
	class SetupParameter;
	class InputHandle;
	class OutputHandle;
	class EngineData;

	class IOutputListener;
	class Data;
	typedef void (*DataCallback)(Data &data);

	typedef std::pair< std::string, InputSlot * >		NamedInput;
	typedef std::map< std::string, InputSlot * >		InputMap;
	typedef std::pair< std::string, OutputSlot * >		NamedOutput;
	typedef std::map< std::string, OutputSlot * >		OutputMap;
	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;
	typedef std::map< std::string, std::string >		StringMap;

	class Service : public Runnable
	{

	public:

		Service(Identifier const& id, IService &service, SystemGraph &system, StringMap const& setup, StringMap const& input, StringMap const& output);
		~Service();

		void					setParameterValue(std::string const& name, EngineData const& data);
		EngineData				getParameterValue(std::string const& name) const;
		InputHandle				createInputHandle(std::string const& name) const;
		OutputHandle			createOutputHandle(std::string const& name);

		bool					checkForSetup();
		bool					checkForUpdate();

		void					setup();
		void					run();
		void					update();
		void					shutdown();

		void					setUpdateRate(float updatesPerSecond);

		void					registerToNewData(std::string const& outName, DataCallback callback);
		void					unregisterFromNewData(std::string const& outName, DataCallback callback);
		void					registerToNewData(std::string const& outName, IOutputListener &listener);
		void					unregisterFromNewData(std::string const& outName, IOutputListener &listener);

		void					linkWith(std::string const& inName, Service &serviceOut, std::string const& outName);
		//void					linkWith(Service &serviceOut);

		bool					isSetUp() const;

	private:

		OutputSlot const&		getOutputSlot(std::string const& name) const;
		InputSlot const&		getInputSlot(std::string const& name) const;
		SetupParameter const&	getSetupParameter(std::string const& name) const;
		OutputSlot &			getOutputSlot(std::string const& name);
		InputSlot &				getInputSlot(std::string const& name);
		SetupParameter &		getSetupParameter(std::string const& name);

		InputMap				m_InputSlots;
		OutputMap				m_OutputSlots;
		ParameterMap			m_SetupParameters;

		IService				*m_Service;

		long					m_MaxDelay;
		float					m_UpdatesPerSecond;
		bool					m_IsSetUp;

	};

	inline bool Service::isSetUp() const
	{
		return m_IsSetUp;
	}

}