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
#include "_2RealIService.h"
#include "_2RealData.h"
#include "_2RealServiceVariable.h"
#include "_2RealServiceParameter.h"

#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/Mutex.h"

namespace _2Real
{
	class AbstractService : public IService
	{

	public:

		AbstractService() : m_bIsConfigured(false) {}

		virtual void run() = 0;											//needs to be implemented by actual service
		virtual void shutdown() = 0;									//this one as well
		virtual bool setup(ConfigMetadataPtr const& _config) = 0;		//and this one, too 

		std::string const& name() const			{ return m_ServiceName; }
		bool const& isConfigured() const		{ return m_bIsConfigured; }

		const bool configure(ConfigMetadataPtr const& _config);
		
		void addListener(ServicePtr _listener);
		void inputListener(Data &_data);
		void outputData();

		template< typename T >
		void addInputVariable(std::string _name, T &_var)
		{
			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_InputVariables.push_back(var);
		}

		template< typename T >
		void addOutputVariable(std::string _name, T &_var)
		{
			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_OutputVariables.push_back(var);
		}

		template< typename T >
		void addSetupParameter(std::string _name, T &_param)
		{
			ServiceParameter< T > *param = new ServiceParameter< T >(_name, _param);
			m_SetupParameters.push_back(param);
		}
		
		void lock() { m_Lock.lock(); }
		void unlock() { m_Lock.unlock(); }

	private:

		friend class ContextPrivate;

		typedef std::list<AbstractServiceVariable *> VariableList;
		typedef std::list<AbstractServiceVariable *> ParameterList;

		ConfigMetadataPtr const& config() const	{ return m_Configuration; }
		const bool hasInputVariables() const	{ return !m_InputVariables.empty(); }
		const bool hasOutputVariables() const	{ return !m_OutputVariables.empty(); }
		const bool hasSetupParameters() const	{ return !m_SetupParameters.empty(); }

		Poco::Mutex					m_Lock;

		Poco::BasicEvent< Data >	m_NewDataEvent;
		bool						m_bIsConfigured;

		std::string					m_ServiceName;
		
		VariableList				m_InputVariables;
		VariableList				m_OutputVariables;
		ParameterList				m_SetupParameters;

		ConfigMetadataPtr			m_Configuration;

	};
}