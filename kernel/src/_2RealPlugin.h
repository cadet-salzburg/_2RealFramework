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

#include "_2RealEntity.h"
#include "_2RealIPluginActivator.h"
#include "_2RealPluginMetadata.h"

#include "Poco/ClassLoader.h"

namespace _2Real
{
	class IService;
	typedef IService *const (*ServiceCreator)(void);

	class SetupParameter;
	class EngineData;
	class SystemGraph;

	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;
	typedef std::pair< std::string, ServiceCreator >	NamedTemplate;
	typedef std::map< std::string, ServiceCreator >		TemplateMap;
	typedef Poco::ClassLoader< IPluginActivator >		PluginLoader;

	class Plugin : public Entity
	{

	public:

		Plugin(Identifier const& id, std::string const& directory, std::string const& file, std::string const& classname, SystemGraph const& system);
		~Plugin();

		void							install();
		void							uninstall();
		void							setup();

		void							registerService(std::string const& name, ServiceCreator service);
		void							addSetupParameter(SetupParameter *const param);

		bool							canExportService(std::string const& name) const;
		bool							exportsService(std::string const& name) const;
		bool							hasSetupParameter(std::string const& name) const;
		bool 							isInitialized() const;

		PluginMetadata const&			getMetadata() const;
		ServiceMetadata const&			getMetadata(std::string const& name) const;

		SetupParameter &				getSetupParameter(std::string const& name);
		SetupParameter const&			getSetupParameter(std::string const& name) const;

		IService						*const createService(std::string const& name) const;

		TemplateMap const&				services() const;
		TemplateMap &					services();
		ParameterMap const&				setupParameters() const;
		ParameterMap &					setupParameters();

	private:

		ParameterMap			m_SetupParameters;
		TemplateMap				m_Services;
		
		IPluginActivator		*m_Activator;
		PluginLoader			m_PluginLoader;

		std::string				m_File;
		bool					m_IsInitialized;

		PluginMetadata			m_Metadata;

		SystemGraph				const& m_System;

	};

	inline TemplateMap const& Plugin::services() const
	{
		return m_Services;
	}

	inline TemplateMap & Plugin::services()
	{
		return m_Services;
	}

	inline ParameterMap const& Plugin::setupParameters() const
	{
		return m_SetupParameters;
	}

	inline ParameterMap & Plugin::setupParameters()
	{
		return m_SetupParameters;
	}

}