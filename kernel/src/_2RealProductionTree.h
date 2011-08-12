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

#include "_2RealTypedefs.h"
#include "_2RealErrorState.h"
#include "_2RealServiceVariable.h"

#include <map>
#include <string>

namespace _2Real
{

	/**
	*	blah blah class description placeholder blah
	*/

	class OutputContainer;
	class OutputListener;

	class ProductionTree
	{

		friend class Framework;

	public:

		///**
		//*	
		//*/
		//typedef std::string ListenerName;

		///**
		//*
		//*/
		//typedef std::string ServiceName;

		/**
		*	callback for production tree update completion
		*/
		typedef void (*ProductionTreeCallback)(std::string const& _treeName, ErrorState const& _error);

		/**
		*	callback for output listener data
		*/
		typedef void (*OutputListenerCallback)(std::string const& _listenerName, ErrorState const& _error);

		///**
		//*	callback for error state change
		//*/
		//typedef void (*ErrorStateCallback)(std::string const& _treeName, ErrorState const& _error);

		/**
		*	begins ProductionTree of output container with _type as top level container
		*/
		const bool beginProductionTree(eContainerType const& _type, ErrorState &_error = ErrorState());

		/**
		*	begins ProductionTree of output container
		*/
		const bool beginProductionTree(ErrorState &_error = ErrorState());

		/**
		*	ends ProductionTree of output container & checks if ProductionTree is valid
		*/
		const bool endProductionTree(ErrorState &_error = ErrorState());

		/**
		*	adds new group container of type _type to output containers' production tree
		*/
		const bool beginGroup(eContainerType const& _type, ErrorState &_error = ErrorState());

		/**
		*	ends ProductionTree of current group container & checks if ProductionTree is valid
		*/
		const bool endGroup(ErrorState &_error = ErrorState());

		/**
		*	adds service container with user service defined by _name & _plugin
		*	will load the plugin if its not loaded currently
		*/
		const bool beginServiceProductionTree(std::string const& _name, std::string const& _plugin, ErrorState &_error = ErrorState());

		/**
		*	ends ProductionTree of user service
		*/
		const bool endServiceProductionTree(ErrorState &_error = ErrorState());

		/**
		*	configure setup parameter of current service
		*/
		template< typename T >
		const bool configureSetupParameter(std::string const& _name, T &_value, ErrorState &_error = ErrorState())
		{
			AbstractServiceVariable *param = new ServiceVariable< T >(_name, _value);

			if (param == NULL)
			{
				return false;
			}

			return configureSetupParameter(param, _error);
		}

		/**
		*	configure output variable of current service
		*/
		const Variable configureOutputParameter(std::string const& _name, ErrorState &_error = ErrorState());

		/**
		*	configure input variable of current service
		*/
		const bool configureInputParameter(std::string const& _name, Variable const& _var, ErrorState &_error = ErrorState());

		/**
		*	add an output listener for a specific output variable
		*	name - selected by user, used to address listener
		*	fwName - framework defined name of the output variable
		*/
		const bool addOutputListener(std::string const& _name, Variable const& _fwName, OutputListenerCallback _callback, ErrorState &_error = ErrorState());

		/**
		*	remove an output listener
		*/
		const bool removeOutputListener(std::string const& _name, ErrorState &_error = ErrorState());

		/**
		*	get data from output listener
		*/
		template< typename T >
		const bool getData(std::string const& _name, T &_value, ErrorState &_error = ErrorState())
		{
			AnyPtr anyPtr = getData(_name, _error);

			if (anyPtr.isNull())
			{
				return false;
			}

			try
			{
				_value = Poco::AnyCast< T >(*anyPtr.get());
			}
			catch (Poco::BadCastException e)
			{
				return false;
			}

			return true;
		}

		/**
		*	checks if output listener has new data
		*/
		const bool hasNewData(std::string const& _name, unsigned long const& _last, ErrorState &_error = ErrorState());

		/**
		*	updates all user defined services in the production tree exactly once
		*/
		const bool updateOnce(bool const& _blocking, ProductionTreeCallback _callback, ErrorState &_error = ErrorState());

		/**
		*	causes all user defined services in the production tree to run asynchronously in their own threads
		*	output data will be buffered by output listeners until getData() is called
		*/
		const bool runAsynchronously(ProductionTreeCallback _callback, ErrorState &_error = ErrorState());

		/**
		*	causes the porduction tree to be updated inifitely
		*	output data will be buffered by output listeners until getData() is called
		*/
		const bool runSynchronously(ProductionTreeCallback _callback, ErrorState &_error = ErrorState());

		/**
		*	stops the production tree if its currently running asynchronously or synchronously
		*/
		const bool stop(ErrorState &_error = ErrorState());

		/**
		*	set plugin path
		*/
		const bool setPluginPath(std::string const& _path, ErrorState &_error = ErrorState());

		/**
		*	install all plugins from given directory - causes all services to be installed
		*/
		const bool installPlugins(std::string const& _directory, ErrorState &_error = ErrorState());

		/**
		*	install plugin from given directory
		*/
		const bool installPlugin(std::string const& _name, ErrorState &_error = ErrorState());

	private:

		/**
		*
		*/
		typedef std::pair< std::string, OutputListener * >	NamedListener;

		/**
		*	note to self: those pointers are owning
		*/
		typedef std::map< std::string, OutputListener * >	NamedListenerMap;

		/**
		*	creation only by framework
		*/
		ProductionTree(OutputContainer *const _container);

		/**
		*	creation only by framework
		*/
		ProductionTree(ProductionTree const& _src);

		/**
		*	creation only by framework
		*/
		ProductionTree& operator=(ProductionTree const& _src);

		/**
		*	destruction only by framework
		*/
		~ProductionTree();

		/*
		*	internal function for setup parameter creation
		*/
		const bool configureSetupParameter(AbstractServiceVariable *const _param, ErrorState &_error = ErrorState());

		/*
		*	internal function for data retrieval
		*/
		AnyPtr getData(std::string const& _name, ErrorState &_error = ErrorState());

		/**
		*	ptr to output container which stores the production tree
		*	note to self: this is an owning pointer // or maybe not?
		*/
		OutputContainer					*m_ContainerPtr;

		/**
		*	map of output listeners + name under which they were registered
		*/
		NamedListenerMap				m_Listeners;

		/**
		*	path to plugin directory
		*/
		std::string						m_PluginPath;

	};

}