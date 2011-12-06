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

#include "_2RealEngineData.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"

#include <iostream>
#include <string>

namespace _2Real
{

	class Engine;
	class Data;
	class Identifier;
	class RunnableError;
	class OutputListener;
	class ExceptionListener;

	typedef void (*ExceptionCallback)(RunnableError &exception);
	typedef void (*DataCallback)(Data &data);

	class System
	{

	public:

		System(std::string const& name);
		System(System const& src);
		~System();

		void setInstallDirectory(std::string const& directory);
		void setLogfile(std::string const& file);

		//void setLoglevel();

		/**
		*	...
		*/
		void destroy();

		/**
		*	loads a dll - assumption: classname-> filename, e.g: MyPlugin -> MyPlugin.dll (release mode), MyPlugin_d.dll (debug mode)
		*	the install directory will be taken from the config.txt
		*	plugins belong to systems that loaded them: this means different systems can have the same plugin loaded, with different setup params
		*	multiple loading of a plugin from within the same system accomplishes nothing
		*/
		const Identifier load(std::string const& name, std::string const& classname);

		/**
		*	calls setup of either a plugin or a service = initialization
		*	if the service is currently running, it will be stopped
		*	warning: for the time being, do not attempt this with plugins more than once
		*/
		void setup(Identifier const& id);

		/**
		*	checks whether or not a plugin / service, in its current state, could be set up, i.e.
		*	if all the existing setup params were set & their datatypes correspond to those defined in the metadata
		*/
		//bool checkParameters(Identifier const& id);

		/**
		*	checks if a service, in it's current state, could be run, i.e.
		*	if all input slots were either set directly or linked.
		*	will also fail if the service was not set up correctly
		*/
		//void checkConfiguration(Identifier const& id);

		/**
		*	printf plugin metadata
		*/
		const std::string getInfo(Identifier const& plugin);

		/**
		*	creates instance of service
		*/
		const Identifier createService(std::string const& name, Identifier const& plugin, std::string const& service);

		/**
		*	sets a runnable's update rate
		*/
		void setUpdateRate(Identifier const& id, float updatesPerSecond);

		/**
		*	initializes a service's or plugin's setup parameter, or directly sets the value of an input slot
		*	if the input slot has been linked to an output slot previously, this linkage will be reset
		*/
		template< typename T >
		void setValue(Identifier const& id, std::string const& name, T const& value)
		{
			EngineData any(value);
			setValueInternal(id, name, any);
		}

		/**
		*	links output slot to input slot
		*/
		void linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);

		/**
		*	starts a runnable (service, synchronization, sequence)
		*/
		void start(Identifier const& id);

		/**
		*	stops a runnable (service, synchronization, sequence)
		*/
		void stop(Identifier const& id);

		/**
		*	registers exception callback for a system
		*/
		void registerToException(ExceptionCallback callback);

		/**
		*	unregisters exception callback for a system
		*/
		void unregisterFromException(ExceptionCallback callback);

		/**
		*	registers exception callback for a system
		*/
		void registerToException(ExceptionListener &listener);

		/**
		*	unregisters exception callback for a system
		*/
		void unregisterFromException(ExceptionListener &listener);

		/**
		*	registers callback for a service's output slot
		*/
		void registerToNewData(Identifier const& service, std::string const& name, DataCallback callback);

		/**
		*	unregisters callback for a service's output slot
		*/
		void unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback);

		/**
		*	registers callback for a service's output slot
		*/
		void registerToNewData(Identifier const& service, std::string const& name, OutputListener &receiver);

		/**
		*	unregisters callback for a service's output slot
		*/
		void unregisterFromNewData(Identifier const& service, std::string const& name, OutputListener &receiver);

		//functions below are currently being refactored

		/**
		*	stops all of nirvanas children at once
		*/
		void stopAll();

		/**
		*	starts all of nirvanas children at once
		*/
		void startAll();

		/**
		*	creates a sequence of entities
		*/
		//const Identifier createSequence(std::string const& name, Identifier const& idA, Identifier const& idB);

		/**
		*	creates a synchronization of entities
		*/
		//const Identifier createSynchronization(std::string const& name, Identifier const& idA, Identifier const& idB);

		/**
		*	links two entities
		*/
		//void link(Identifier const& in, Identifier const& out);

		/**
		*	returns the ids of an entity's children
		*/
		//Identifiers getChildren(Identifier const& id);

		/**
		*	destroys an entity
		*/
		//void destroy(Identifier const& id);

		/**
		*	inserts an entity into another
		*/
		//void insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src);

		/**
		*	like insert, with index being the last place in the children
		*/
		//void append(Identifier const& dst, Identifier const& id);

	private:

		/**
		*	internally used function for setting param values
		*/
		void setValueInternal(Identifier const& id, std::string const& name, EngineData const& value);

		/**
		*	system's identifier
		*/
		Identifier		m_Id;

		/**
		*	the 2real engine
		*/
		Engine			&m_Engine;
	};
}