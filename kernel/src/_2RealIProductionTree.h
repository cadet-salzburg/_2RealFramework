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

#include <string>

namespace _2Real
{

	/**
	*	blah blah class description placeholder blah
	*/

	class OutputContainer;
	class OutputListener;
	class ListenerRef;
	class Exception;

	class IProductionTree
	{

	public:

		/**
		*	to be called when production tree has completed an update
		*/
		typedef void (*ProductionTreeCallback)(IProductionTree const *const _ProductionTree);

		/**
		*	to be called when output listener has received new data
		*/
		typedef void (*OutputListenerCallback)(IProductionTree const *const _ProductionTree, ListenerRef const *const _ref);

		/**
		*	callback for exceptions during production tree update
		*/
		typedef void (*ExceptionOccuredCallback)(IProductionTree const *const _ProductionTree);

		/**
		*	begins ProductionTree of output container with _type as top level container
		*	does nothing if ProductionTree is already in process
		*/
		virtual void beginProductionTree(eContainerType const& _type) = 0;

		/**
		*	begins ProductionTree of production tree
		*	does nothing if ProductionTree is already in process
		*/
		virtual void beginProductionTree() = 0;

		/**
		*	ends ProductionTree of production tree
		*	throws exception if ProductionTree is invalid
		*/
		virtual void endProductionTree() throw (...) = 0;

		/**
		*	adds new group container of type _type to output containers' production tree
		*	does nothing if ProductionTree is already complete
		*/
		virtual void beginGroup(eContainerType const& _type) = 0;

		/**
		*	ends ProductionTree of current group container
		*	throws exception if ProductionTree is invalid
		*/
		virtual void endGroup() throw (...) = 0;

		/**
		*	adds service container with user service defined by _name & _plugin
		*	attempts to load the plugin if not loaded currently
		*	throws exception if plugin installation fails or plugin is in invalid state
		*/
		virtual const bool beginServiceProductionTree(std::string const& _name, std::string const& _plugin) throw (...) = 0;

		/**
		*	ends ProductionTree of user service
		*/
		virtual const bool endServiceProductionTree() throw (...) = 0;

		/**
		*	configure setup parameter of current service
		*/
		template < typename T >
		virtual void configureSetupParameter(std::string const& _name, T &_value)
		{
		}

		/**
		*	configure output variable of current service
		*	throws exception if parameter with same name was already configured
		*/
		virtual const Variable configureOutputParameter(std::string const& _name) throw (...) = 0;

		/**
		*	configure input variable of current service
		*	overwrites
		*/
		virtual const bool configureInputParameter(std::string const& _name, Variable const& _var) = 0;

		/**
		*	add an output listener for a specific output variable
		*	fwName - framework defined name of the output variable
		*/
		virtual const ListenerRef *const addOutputListener(Variable const& _fwName, OutputListenerCallback _callback) = 0;

		/**
		*	remove an output listener
		*/
		virtual const bool removeOutputListener(const ListenerRef *const _ref) = 0;

		/**
		*	get data from output listener
		*/
		template< typename T >
		const bool getData(std::string const& _name, T &_value)
		{
		}

		/**
		*	checks if output listener has new data
		*/
		virtual const bool hasNewData(ListenerRef const& _ref, unsigned long const& _last) = 0;

		/**
		*	updates all user defined services in the production tree exactly once
		*/
		virtual const bool updateOnce(bool const& _blocking, ProductionTreeCallback _callback) = 0;

		/**
		*	causes all user defined services in the production tree to run asynchronously in their own threads
		*	output data will be buffered by output listeners until getData() is called
		*/
		virtual const bool runAsynchronously(ProductionTreeCallback _callback) = 0;

		/**
		*	causes the porduction tree to be updated inifitely
		*	output data will be buffered by output listeners until getData() is called
		*/
		virtual const bool runSynchronously(ProductionTreeCallback _callback) = 0;

		/**
		*	stops the production tree if its currently running asynchronously or synchronously
		*/
		virtual const bool stop() = 0;

		/**
		*	set plugin path
		*/
		virtual const bool setPluginPath(std::string const& _path) = 0;

		/**
		*	install all plugins from given directory - causes all services to be installed
		*/
		virtual const bool installPlugins(std::string const& _directory) = 0;

		/**
		*	install plugin from given directory
		*/
		virtual const bool installPlugin(std::string const& _name) = 0;

	};

}