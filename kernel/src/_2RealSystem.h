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

#include "Poco/Any.h"

namespace _2Real
{
	class Engine;
	class System
	{

	public:

		/**
		*	constructor
		*
		*	creates the nirvana, a production graph where all created entities run threaded and
		*	can be freely connected as the application programmer pleases, although cycles are
		*	not allowed - or are they? every newly created entity (meaning service, sequence or
		*	synchronization) is moved there, where it will live for all eternity in permanent
		*	bliss - or until it is inserted somewhere else.
		*	it is possible to create multiple systems. services, sequences ans synchronizations
		*	are created in relation a a particular system, and can never be moved elsewhere -
		*	or linked with an entity of another system
		*
		*	@param name:			name of system = name of nirvana
		*
		*	open questions:			buddhists might disapprove of calling a production graph 'nirvana'
		*							also, this class has now been named: engine / context / framework,
		*							and now finally system. where will it end?
		*/
		System(std::string const& name);

		/**
		*	returns the system's own identifier
		*
		*	@return:				system identifier
		*/
		const Identifier getID();

		/**
		*	installs a plugin, causing all of its services to be exported
		*
		*	possible exceptions:	library not found
		*							wrong class name
		*							error during plugin's init() or getMetadata()
		*
		*	@param name:			name chosen by user, will be used to create identifier
		*	@param directory:		absolute path to installation directory
		*	@param file:			filename
		*	@param class:			class name, must be identical to the name defined in the
		*							plugin's metadata
		*	@return:				the plugin's unique identifier
		*/
		const Identifier loadPlugin(std::string const& name, std::string const& directory, std::string const& file, std::string const& classname) throw(...);

		/**
		*	printf plugin metadata
		*
		*	possible exceptions:	invalid id
		*
		*	@param pluginID:		identifier of an installed plugin
		*/
		void dumpPluginInfo(Identifier const& pluginID) throw(...);

		/**
		*	printf service metadata
		*
		*	possible exceptions:	invalid id
		*							non-existant service
		*
		*	@param pluginID:		identifier of an installed plugin
		*	@param serviceName:		name of a service exported by the plugin
		*/
		void dumpServiceInfo(Identifier const& pluginID, std::string const& serviceName) throw(...);

		/**
		*	creates instance of user service
		*
		*	possible exceptions:	id does not belong to a valid plugin
		*							plugin does not export the service in question
		*							error during service creation
		*
		*	@param name:			name chosen by user, will be used to create identifier
		*	@param pluginID:		identifier of an installed plugin
		*	@param serviceName:		name of a service exported by the plugin
		*	@return:				the service's unique identifier
		*/
		const Identifier createService(std::string const& name, Identifier const& pluginID, std::string const& serviceName) throw(...);

		/**
		*	returns identifiers of all setup params of a service
		*
		*	possible exceptions:	invalid id
		*
		*	@param _id:				identifier of service
		*	@return:				ids of setup params
		*/
		Identifiers getSetupParameters(Identifier const& serviceID) throw(...);

		/**
		*	initializes a service's setup parameter
		*
		*	possible exceptions:	invalid id
		*							types do not match
		*
		*	@param paramID:			identifier of a setup parameter
		*	@param value:			the value
		*/
		template< typename T >
		void setParameterValue(Identifier const& paramID, T const& value) throw(...)
		{
			try
			{
				Poco::Any any(value);
				setParameterValue(paramID, any);
			}
			catch (Exception &e)
			{
				throw e;
			}
		}

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
		*	@param name:			name chosen by user, will be used to create identifier
		*	@param idA:				identifier of either: sequence, synchronization or service
		*	@param idB:				identifier of either: sequence, synchronization or service
		*	@return:				the sequence's unique identifier
		*
		*	open questions: is it really necessary for signatures to match perfectly here?
		*	output slots could be discarded, IO slots might match, but be ordered differently
		*/
		const Identifier createSequence(std::string const& name, Identifier const& idA, Identifier const& idB) throw(...);

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
		*	@param name:			name chosen by user, will be used to create identifier
		*	@param idA:				identifier of either: sequence, synchronization or service
		*	@param idB:				identifier of either: sequence, synchronization or service
		*	@return:				the synchronization's unique identifier
		*/
		const Identifier createSynchronization(std::string const& name, Identifier const& idA, Identifier const& idB) throw(...);

		/**
		*	links two entities
		*
		*	possible exceptions:	invalid ids
		*							IO mismatch
		*
		*	@param in:				identifier of service / sequence / synchronization
		*	@param out:				identifier of service / sequence / synchronization
		*/
		void link(Identifier const& in, Identifier const& out) throw(...);

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
		*	@param id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of output slots
		*/
		Identifiers getOutputSlots(Identifier const& id) throw(...);

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
		*	@param id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of input slots
		*/
		Identifiers getInputSlots(Identifier const& id) throw(...);

		/**
		*	links an input slot with an output slot
		*
		*	possible exceptions:	invalid ids
		*							type mismatch
		*							slots belong to the same entity
		*							linkage results in a cycle (cycle checking is TODO)
		*
		*	@param in:				identifier of input slot
		*	@param out:				identifier of output slot
		*/
		void linkSlots(Identifier const& in, Identifier const& out) throw(...);

		/**
		*	returns the ids of an entity's children
		*
		*	possible exceptions:	invalid id
		*
		*	@param id:				identifier of either: sequence, synchronization or service
		*	@return:				ids of output slots
		*/
		Identifiers getChildren(Identifier const& id) throw(...);

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
		*	@param id:				identifier of either: sequence, synchronization or service
		*
		*	open questions: if the entity listens to another entity that is currently paused,
		*	it might never receive any DataImpl, meaning it might never update at all
		*/
		void start(Identifier const& id) throw(...);

		/**
		*	starts all of nirvanas children at once
		*
		*	possible exceptions:	IO misconfiguration if a child
		*/
		void startAll() throw(...);

		/**
		*	stops an entity
		*
		*	stops an entity as well as all of its children. if the entity in question does
		*	have a superior that one will be stopped as well, unless the superior is nirvana,
		*	where stopping should not matter as every entity has their own thread
		*
		*	possible exceptions:	invalid id
		*
		*	@param id:				identifier of either: sequence, synchronization or service
		*/
		void stop(Identifier const& id) throw(...);

		/**
		*	starts all of nirvanas children at once
		*
		*	possible exceptions:	IO misconfiguration if a child
		*/
		void stopAll() throw(...);

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
		void destroy(Identifier const& id) throw(...);

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
		*	link insert, with index being the last place in the children
		*/
		void append(Identifier const& dst, Identifier const& id) throw(...);

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
		*	you know what this is :)
		*/
		~System();

		/**
		*	and that, too
		*/
		System(System const& src);

		/**
		*	and then there's that cute little critter as well
		*/
		System& operator=(System const& src);

	private:

		/**
		*	internally used function for setting param values
		*/
		void setParameterValue(Identifier const& id, Poco::Any any) throw(...);

		/**
		*	the 2 real engine
		*/
		Engine					*m_Engine;

		/**
		*	identifier of the system
		*/
		Identifier				*m_ID;
	};
}