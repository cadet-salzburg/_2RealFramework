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

		Plugin(Identifier const& id, IPluginActivator &activator, PluginMetadata &metadata);
		~Plugin();

		/**
		*	returns classname
		*/
		std::string const&				getClassName() const;

		/**
		*	returns lib path (as string)
		*/
		std::string const&				getLibraryPath() const;

		/**
		*	returns metadata info string
		*/
		const std::string				getInfoString() const;

		/**
		*	sets up the plugin
		*/
		void							setup();

		/**
		*	true if setup has already been called
		*/
		bool 							isSetUp() const;

		/**
		*	called by PluginContext
		*/
		void							registerService(std::string const& serviceName, ServiceCreator creator);

		/**
		*	called by PluginContext (true if service is contained in the metadata)
		*/
		const bool						definesService(std::string const& serviceName) const;

		/**
		*	true if a service was exported during setup
		*/
		const bool						exportsService(std::string const& serviceName) const;

		/**
		*	param stuff
		*/
		EngineData const&				getParameterValue(std::string const& setupName) const;
		std::string const&				getParameterKey(std::string const& setupName) const;
		void							setParameterValue(std::string const& setupName, EngineData const& data);

		/**
		*	creates a service instance
		*/
		Runnable &						createService(std::string const& serviceName, SystemGraph &graph);

		/**
		*	creates a service instance
		*/
		Runnable &						createService(std::string const& idName, std::string const& serviceName, SystemGraph &graph);

		/**
		*	returns the activator (called on pluginpool::clear)
		*/
		IPluginActivator &				getActivator();

	private:

		SetupParameter &				getSetupParameter(std::string const& setupName);
		SetupParameter const&			getSetupParameter(std::string const& setupName) const;

		IService &						createService(std::string const& serviceName) const;

		ParameterMap					m_SetupParameters;
		TemplateMap						m_ServiceTemplates;
		ServiceMap						m_Services;

		PluginMetadata					&m_Metadata;
		IPluginActivator				&m_Activator;
		bool							m_IsSetUp;

	};

}