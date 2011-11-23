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

#include "_2RealEngineTypedefs.h"
#include "_2RealEntity.h"
#include "_2RealIPluginActivator.h"
#include "_2RealPluginMetadata.h"

#include <list>

#include "Poco/ClassLoader.h"

namespace _2Real
{
	class IService;
	typedef IService *const (*ServiceCreator)(void);

	class SetupParameter;

	typedef std::pair< std::string, SetupParameter * >	NamedParameter;
	typedef std::map< std::string, SetupParameter * >	ParameterMap;
	typedef std::pair< std::string, ServiceCreator >	NamedTemplate;
	typedef std::map< std::string, ServiceCreator >		TemplateMap;

	class Plugin : public Entity
	{

	public:

		Plugin(Identifier const& id, PluginMetadata *const info);
		virtual ~Plugin();

		void														install();
		void														uninstall();
		void														setup();

		void														registerService(std::string const& name, ServiceCreator service);
		void														addSetupParameter(SetupParameter *const param);

		PluginMetadata const&										pluginMetadata() const;
		ServiceMetadata const&										serviceMetadata(std::string const& name) const;

		TemplateMap const& services() const
		{
			return m_Services;
		}

		TemplateMap& services()
		{
			return m_Services;
		}

		ParameterMap const& setupParameters() const
		{
			return m_SetupParameters;
		}

		ParameterMap& setupParameters()
		{
			return m_SetupParameters;
		}

		//std::list< unsigned int > serviceIDs() const;
		std::list< unsigned int > setupParameterIDs() const;

		//ServiceTemplate *const getServiceTemplate(std::string const& name);
		SetupParameter *const getSetupParameter(std::string const& name);

		bool const& isInitialized() const;
		SharedAny getParameterValue(std::string const& name);

		IService *const createService(std::string const& name);

	private:

		typedef Poco::ClassLoader< IPluginActivator >				PluginLoader;

		ParameterMap												m_SetupParameters;
		TemplateMap													m_Services;

		/**
		*	dll filepath
		*/
		std::string													m_File;

		/**
		*	plugin activator implemented by user plugin
		*/
		IPluginActivator											*m_Activator;

		/**
		*	metadata
		*/
		PluginMetadata												*const m_Metadata;

		/**
		*	poco classloader
		*/
		PluginLoader												m_PluginLoader;

		/**
		*	true if plugin's setup has been called at least once
		*/
		bool														m_IsInitialized;

	};

}