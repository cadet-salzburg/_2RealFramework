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

#include <string>

namespace _2Real
{

	/**
	*	i hate life
	*/

	class EngineImpl;

	class Engine
	{

	public:

		/**
		*	installs a plugin, causing all services to be exported
		*
		*	possible exceptions:	dll not found / wrong classname / error during service export / other
		*
		*	@param _id:				name chosen by user, will be used to create plugin's identifier
		*	@param _path:			dll's absolute path
		*	@param _name:			dll's classname, must be identical to pluginname defined in the plugin's metadata
		*	@param _serviceIDs:		will be overwritten & return identifiers for the service exported by the dll
		*							service identifiers will have the same names and be in the same order as defined in the plugin's metadata
		*	@return:				plugin's unique identifier
		*/
		const Identifier installPlugin(std::string const& _id, std::string const& _path, std::string const& _name, Identifiers &_serviceIDs) throw(...);

		/**
		*	creates a new production graph
		*
		*	possible exceptions:	?
		*
		*	@param _id:				name chosen by user, will be used to create graph's identifier
		*	@param _type:			type of graph, must be one of the following: PRODUCTION, SEQUENCE or SYNCHRONIZATION
		*	@return:				production graph's unique identifier
		*/
		const Identifier createProductionGraph(std::string const& _id, eContainerType const& _type) throw(...);

		/**
		*	creates instance of a user service; if service in question is a singleton, return its instance
		*
		*	possible exceptions:	plugin exporting the service has been uninstalled or is otherwise invalid / _id does not belong to a valid plugin
		*
		*	@param _name:			name chosen by user, will be used to create service's identifier
		*	@param _id:				identifier of the service's factory - as returned by installPlugin() function
		*	@param _setupIDs		will be overwritten: service's setup params
		*	@param _inputIDs		will be overwritten: service's input variables
		*	@param _outputIDs		will be overwritten: service's output variables
		*	@return:				service's unique identifier
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs, Identifiers &_inputIDs, Identifiers &_outputIDs) throw (...);

		/**
		*	adds a(nother) mutex to a production graph
		*
		*	possible exceptions:	_id does not belong to a valid production graph
		*
		*	@param _name:			name chosen by user, will be used to create mutex' identifier
		*	@param _id:				identifier of the production graph
		*	@return:				mutex' unique identifier
		*/
		const Identifier createMutex(std::string const& _name, Identifier const& _id) throw (...);

		/**
		*	initializes service's setup parameter
		*
		*	possible exceptions:	_id does not belong to a valid service parameter / type does not match
		*
		*	@param _id:				identifier of a setup parameter
		*	@param _value:			value
		*	@return:				false if the service is already fully configured(meaning it has carried out its update function at least once)
		*							and reconfiguration is not allowed as per the service's metadata.
		*/
		template< typename T >
		bool setParameterValue(Identifier const& _id, T const& _value) throw(...)
		{
		}

		/**
		*	connects a service's output var with another service's input var. yeah, this function's name sucks, i know.
		*
		*	possible exceptions:	_out / _in belong to same service (checking for cycles is going to be a pain in the ass. although it would work if
		*							_in had a default value) _out / _in are not valid ids, or types do not match
		*
		*	@param _out:			output var of a service
		*	@param _in:				input var of a different service
		*	@return:				false if _in is already configured, and cannot be reconfigured
		*/
		bool connectSlots(Identifier const& _out, Identifier const& _in) throw(...);

		/**
		*	registers callback for an exception
		*
		*	possible exceptions:	_id does not belong to a production graph
		*	
		*	@param _id:				identifier of production graph.
		*	@param _callback		function pointer
		*
		*	open questions:			what to do if this pg is later inserted into another pg?
		*/
		void registerToException(Identifier const& _id, ExceptionCallback _callback) throw(...);

		/**	registers callback for new data
		*
		*	possible exceptions:	_id does not belong to a production graph
		*	
		*	@param _id:				identifier of production graph.
		*	@param _callback		function pointer
		*
		*	open questions:			what to do if this pg is later inserted into another pg?
		*/
		void registerToNewData(Identifier const& _id, NewDataCallback _callback) throw(...);

		/**
		*	inserts production graph or service _src into production graph _dst
		*
		*	possible exceptions:	invalid ids, or self-insertion. or creating a cycle. fuck you, cycles.
		*							or inserting a container which cannot be reconfigured somewhere else, so that it needs to be reconfigured.
		*							(and can this even happen, i think not)
		*
		*	@param _src:			production graph to be inserted
		*	@param _dst:			the other one
		*
		*	open questions:			spaghetti monster, help me. rAmen.
		*/
		void insertInto(Identifier const& _dst, Identifier const& _src) throw(...);

		/**
		*	starts all top-level pg's
		*
		*	possible exceptions:	if a graph is incompletely / incorrectly configured.
		*/
		void startAll() throw(...);

		/**
		*	stops all top-level pg's
		*
		*	possible exceptions:	?
		*/
		void stopAll() throw(...);

		/**
		*	starts container
		*
		*	possible exceptions:	id not fully configured, or incorrectly configured
		*/
		void start(Identifier const& _id) throw(...);

		/**
		*	stops container
		*
		*	possible exceptions:	?
		*/
		void stop(Identifier const& _id) throw(...);

		/**
		*	printf plugin's metadata
		*
		*	possible exceptions:	not a plugin's id
		*/
		void dumpPluginInfo(Identifier const& _id) throw(...);

		/**
		*	printf service's metadata
		*
		*	possible exceptions:	not a service's id
		*/
		void dumpServiceInfo(Identifier const& _id) throw(...);

		/**
		*	you know what this is
		*/
		Engine();

		/**
		*	and this too
		*/
		~Engine();

		/**
		*	and that cute little critter, too
		*/
		Engine(Engine const& _src);

		/**
		*	hi there
		*/
		Engine& operator=(Engine const& _src);

	private:

		/**
		*	you made it to the end of the file! congratulations.
		*/
		EngineImpl				*m_Impl;

	};

}