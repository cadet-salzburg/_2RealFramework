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
	class Service;

	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;
	typedef std::pair< std::string, ServiceCreator >	NamedTemplate;
	typedef std::map< std::string, ServiceCreator >		TemplateMap;
	typedef std::pair< Identifier, Service * >			NamedService;
	typedef std::map< Identifier, Service * >			ServiceMap;
	typedef Poco::ClassLoader< IPluginActivator >		PluginLoader;

	class Plugin : public Entity
	{

	public:

		Plugin(Identifier const& id, std::string const& directory, std::string const& file, std::string const& classname, SystemGraph &system);
		~Plugin();

		void							install();
		void							uninstall();
		void							setup();
		bool 							isInitialized() const;
		const std::string				getInfoString() const;
		void							registerService(std::string const& serviceName, ServiceCreator creator);
		bool							canExportService(std::string const& serviceName) const;
		EngineData						getParameterValue(std::string const& setupName) const;
		void							setParameterValue(std::string const& setupName, EngineData const& value);
		const Identifier				createService(std::string const& idName, std::string const& serviceName);

	private:

		SetupParameter &				getSetupParameter(std::string const& setupName);
		SetupParameter const&			getSetupParameter(std::string const& setupName) const;

		IService &						createService(std::string const& serviceName) const;

		ParameterMap					m_SetupParameters;
		TemplateMap						m_ServiceTemplates;
		ServiceMap						m_Services;

		IPluginActivator				*m_Activator;
		PluginLoader					m_PluginLoader;
		std::string						m_File;
		bool							m_IsInitialized;
		SystemGraph						&m_System;
		PluginMetadata					m_Metadata;

	};

	inline const std::string Plugin::getInfoString() const
	{
		return m_Metadata.getInfoString();
	}

	inline bool Plugin::isInitialized() const
	{
		return m_IsInitialized;
	}

}