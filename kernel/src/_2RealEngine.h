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
#include <list>

#include "Poco/Any.h"

namespace _2Real
{

	/**
	*	in the following comments, whenever i write 'nirvana', i'm referring to the top-
	*	-level asynchronous production graph that is created when the constructor is called;
	*	where all other entities (such as they are: service containers = services,
	*	sequence graphs = sequences & synchronization graphs = synchronizations) run in 
	*	their own threads and the programmer can connect input and output slots to their
	*	hearts content. 'a container is in nirvana' is therefore synonymous for:
	*	their father is the top level container.
	*/

	class Exception;
	class Data;
	class Identifier;
	class EngineImpl;

	class Engine
	{

	public:

		/**
		*	constructor
		*
		*	creates the nirvana, a production graph where all created entities run threaded and
		*	can be freely connected as the application programmer pleases, although cycles are
		*	not allowed - or are they? every newly created entity (meaning service, sequence or
		*	synchronization) is moved there, where it will live for all eternity in permanent
		*	bliss - or until it is inserted somewhere else. removing an entity from its superior
		*	will move it back to nirvana.
		*
		*	@param _name:			name of engine = name of nirvana
		*
		*	open questions:			buddhists might not approve of the naming.
		*/
		Engine(std::string const& name);

		/**
		*	installs a plugin, causing all of its services to be exported
		*
		*	possible exceptions:	library not found
		*							wrong class name
		*
		*	@param _name:			name chosen by user, will be used to create identifier
		*	@param _dir:			absolute path to installation directory
		*	@param _file:			filename
		*	@param _class:			class name, must be identical to the name defined in the
		*							plugin's metadata
		*	@param _serviceIDs:		will be overwritten & return identifiers for the services
		*							exported by the plugin library
		*							service identifiers will have the same names and be in the
		*							same order as defined in the metadata
		*	@return:				the plugin's unique identifier
		*/
		const Identifier loadPlugin(std::string const& name, std::string const& dir, std::string const& file, std::string const& classname, Identifiers &serviceIDs) throw(...);

		/**
		*	printf plugin metadata
		*
		*	possible exceptions:	invalid id
		*/
		void dumpPluginInfo(Identifier const& pluginID) throw(...);

		/**
		*	printf service metadata
		*
		*	possible exceptions:	invalid id
		*/
		void dumpServiceInfo(Identifier const& serviceID) throw(...);

		/**
		*	printf service metadata
		*
		*	possible exceptions:	invalid id
		*/
		void dumpServiceInfo(Identifier const& pluginID, std::string const& serviceName) throw(...);

		/**
		*	creates new user service object; if service is a singleton, returns instance
		*
		*	actually, the identifier returned does not belong to the service itself, but to its
		*	container. not that this really matters.
		*
		*	possible exceptions:	id does not belong to a valid service factory function
		*
		*	@param _name:			name chosen by user, will be used to create identifier
		*	@param _id:				identifier of the factory function
		*	@param _setupIDs:		will be overwritten: setup params
		*	@return:				the service's unique identifier
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs) throw (...);

		/**
		*	creates new user service object; if service is a singleton, returns instance
		*
		*	possible exceptions:	id does not belong to a valid plugin
		*							plugin does not export the service in question
		*
		*	@param _name:			name chosen by user, will be used to create identifier
		*	@param _id:				identifier of the plugin exporting the service
		*	@param _service:		name of the service, must be identical to a name defined
		*							in the plugin's metadata
		*	@param _setupIDs:		will be overwritten: setup params
		*	@return:				the service's unique identifier
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifiers &_setupIDs) throw(...);

		/**
		*	returns identifiers of all output slots of an entity
		*
		*	returns a vector of all output slots of a sequence, synchronization or service
		*	in case of a sequence, will be identical to the output of the last child. in case
		*	of a service, will be identical to the output variables specified in the metadata.
		*	in case of a synchronization, will be the combined output of all its children.
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of output slots
		*/
		Identifiers getOutputSlots(Identifier const& _id) throw(...);

		/**
		*	returns identifiers of all input slots of an entity
		*
		*	returns a vector of all input slots of a sequence, synchronization or simply a
		*	service. in case of a sequence, will be identical to the input of the first
		*	child. in case of a service, will be identical to the input variables specified in
		*	the metadata (might be none). in case of a synchronization, will be the
		*	combined input of all its children.
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of input slots
		*/
		Identifiers getInputSlots(Identifier const& _id) throw(...);

		/**
		*	creates a sequence of entities
		*
		*	two entities being in a sequence guarantees that the first one will be updated
		*	exactly once and that the second will have received the output DataImpl before the
		*	second entity is being updated. sequences can be built out of services, sequence
		*	graphs or synchronization graphs; example usage:
		*	createSequence("S0", createSequence("S1", _idA, _idB), createSequence("S2", _idC, _idD));
		*	creates sequence of: _idA -> _idB -> _idC -> _idD
		*	entities passed as arguments must be in nirvana, afterwards the sequence becomes
		*	their superior; newly created sequences will be placed in nirvana as well
		*	output slot configuration: building a sequence will delete all previously built IO
		*	connections of the entities in question (while keeping their internal IO connections
		*	intact), and connect the IO slots of the 2 entities instead
		*
		*	possible exceptions:	invalid identifiers
		*							entities are not in nirvana
		*							IO slots mismatch
		*
		*	@param _name:			name chosen by user, will be used to create identifier
		*	@param _idA:			identifier of either: sequence, synchronization or service
		*	@param _idB:			identifier of either: sequence, synchronization or service
		*	@return:				the sequence's unique identifier
		*
		*	open questions: is it really necessary for signatures to match perfectly here?
		*	output slots could be discarded, IO slots might match, but be ordered differently
		*/
		const Identifier createSequenceContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB) throw(...);

		/**
		*	creates a synchronization of entities
		*
		*	two entities being in synchronization guarantees causes both update functions to
		*	be carried out parallel, combining the output DataImpl of both into one combined DataImpl
		*	when both are finished. synchronizations can be built out of services, sequence
		*	graphs or synchronization graphs; example usage:
		*	createSynchronization("S0", createSequence("S1", _idA, _idB), createSynchronization("S2", _idC, _idD));
		*	causes sequence _idA -> _idB to run in parallel with both _idC and _idB
		*	entities passed as arguments must be in nirvana, afterwards the synchronization
		*	becomes their superior; newly created synchronizations will be placed in nirvana
		*	output slot configuration: building a synchronization will delete all previously
		*	built IO-configurations of the entities in question, while keeping the internal
		*	ones intact. 
		*
		*	possible exceptions:	invalid identifiers
		*							entities are not in nirvana
		*							IO slots mismatch
		*
		*	@param _name:			name chosen by user, will be used to create identifier
		*	@param _idA:			identifier of either: sequence, synchronization or service
		*	@param _idB:			identifier of either: sequence, synchronization or service
		*	@return:				the synchronization's unique identifier
		*/
		const Identifier createSynchronizationContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB) throw(...);

		/**
		*	links an input slot with an output slot
		*
		*	@param _in:				identifier of input variable
		*	@param _out:			identifier of output variable
		*/
		void link(Identifier const& _in, Identifier const& _out) throw(...);

		/**
		*	initializes a service's setup parameter
		*
		*	possible exceptions:	invalid id
		*							types do not match
		*							service is already configured, reconfiguration not allowed
		*
		*	@param _id:				identifier of a setup parameter
		*	@param _value:			the value
		*/
		template< typename T >
		void setParameterValue(Identifier const& _id, T const& _value) throw(...)
		{
			try
			{
				Poco::Any any(_value);
				setParameterValue(_id, any);
			}
			catch (...)
			{
				throw;
			}
		}

		/**
		*	returns the ids of an entity's children
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of output slots
		*/
		Identifiers getChildren(Identifier const& _id) throw(...);

		/**
		*	stops an entity
		*
		*	stops an entity as well as all of its children. if the entity in question does
		*	have a superior that one will be stopped as well, unless the superior is nirvana,
		*	where stopping should not matter as every entity has their own thread
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*/
		void stop(Identifier const& _id) throw(...);

		/**
		*	starts an entity
		*
		*	does only work if the entity in question belongs to nirvana. causes a thread to be
		*	started, where the entity running until stop is called. calling start will perform
		*	a check on the entity & all its children, making sure all IO slots are connected
		*
		*	possible exceptions:	invalid id
		*							entity not in nirvana
		*							IO misconfiguration
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*
		*	open questions: if the entity listens to another entity that is currently paused,
		*	it might never receive any DataImpl, meaning it might never update at all
		*/
		void start(Identifier const& _id) throw(...);

		/**
		*	starts all of nirvanas children at once
		*
		*	possible exceptions:	IO misconfiguration if a child
		*/
		void startAll() throw(...);

		/**
		*	function to remove an entity from its superior, back to nirvana
		*
		*	moves a service, sequence or synchronization from its superior entity back to
		*	nirvana. as a consequence, all existing IO connections are removed. the superior
		*	will be stopped, causing its IO connections to be checked on restart. entities
		*	already in nirvana are unaffected by this operation.
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id				identifier of either: sequence, synchronization or service
		*/
		void remove(Identifier const& _id);

		/**
		*	destroys an entity
		*
		*	destroys entity, as well as all of it's children. will stop the superior, if the
		*	entity is not in nirvana.
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of either: sequence, synchronization or service
		*/
		void destroy(Identifier const& _id) throw(...);

		/**
		*	inserts an entity into another
		*
		*	inserts a service, sequence or synchronization that is currently in nirvana into
		*	other sequence or synchronization, at the specified index. causes all existing IO
		*	connections to break.
		*
		*	possible exceptions:	invalid ids
		*							_dst and _src are the same
		*
		*	@param _src:			production graph to be inserted
		*	@param _dst:			the other one
		*	@param _index:			0, 1, 2 - first, middle, last
		*
		*	open questions:			spaghetti monster, help me. rAmen.
		*/
		void insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src) throw(...);

		/**
		*	registers callback for an exception
		*
		*	possible exceptions:	invalid id
		*	
		*	@param _id:				identifier of either: sequence, synchronization or service
		*	@param _callback		function pointer
		*/
		void registerToException(Identifier const& _id, ExceptionCallback _callback) throw(...);

		/**
		*	registers callback for new DataImpl
		*
		*	possible exceptions:	invalid id
		*	
		*	@param _id:				identifier of either: sequence, synchronization or service
		*	@param _callback		function pointer
		*/
		void registerToNewData(Identifier const& _id, NewDataCallback _callback) throw(...);

		/**
		*	
		*/
		~Engine();

		/**
		*	
		*/
		Engine(Engine const& _src);

		/**
		*	
		*/
		Engine& operator=(Engine const& _src);

	private:

		/**
		*	internally used function for setting param values
		*/
		void setParameterValue(Identifier const& _id, Poco::Any _any) throw(...);

		/**
		*	
		*/
		EngineImpl				*m_Impl;

		/**
		*
		*/
		Identifier				*m_ID;

	};

}