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

#include "Poco/Path.h"

namespace _2Real
{

	class IService;
	typedef IService *const (*ServiceCreator)(void);

	class SetupParameter;
	class EngineData;
	class SystemGraph;
	class Runnable;
	class Service;

	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;
	typedef std::pair< std::string, ServiceCreator >	NamedTemplate;
	typedef std::map< std::string, ServiceCreator >		TemplateMap;
	typedef std::pair< Identifier, Service * >			NamedService;
	typedef std::map< Identifier, Service * >			ServiceMap;

	class Plugin : public Entity
	{

	public:

		Plugin(Identifier const& id, std::string const& classname, Poco::Path const& dir, SystemGraph &system, IPluginActivator *activator);
		~Plugin();

		void							install();
		void							uninstall();
		void							setup();

		/**
		*	true if setup has already been called
		*/
		bool 							isSetUp() const;

		/**
		*	returns a string contains all the metadata information
		*/
		const std::string				getInfoString() const;

		/**
		*	returns the plugin activator's classname
		*/
		const std::string				getClassname() const;

		/**
		*	called by PluginContext
		*/
		void							registerService(std::string const& serviceName, ServiceCreator creator);

		/**
		*	called by PluginContext (true if service is contained in the metadata)
		*/
		bool							canExportService(std::string const& serviceName) const;

		EngineData const&				getParameterValue(std::string const& setupName) const;
		std::string const&				getParameterKey(std::string const& setupName) const;
		void							setParameterValue(std::string const& setupName, EngineData const& data);
		Runnable &						createService(std::string const& idName, std::string const& serviceName);

	private:

		SetupParameter &				getSetupParameter(std::string const& setupName);
		SetupParameter const&			getSetupParameter(std::string const& setupName) const;

		IService &						createService(std::string const& serviceName) const;

		ParameterMap					m_SetupParameters;
		TemplateMap						m_ServiceTemplates;
		ServiceMap						m_Services;

		SystemGraph						&m_System;
		PluginMetadata					m_Metadata;
		std::string						m_Classname;
		IPluginActivator				*m_Activator;
		bool							m_IsSetUp;

	};

}