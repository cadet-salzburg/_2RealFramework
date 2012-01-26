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

#include <string>
#include <list>

namespace _2Real
{

	class Engine;
	class Data;
	class Identifier;
	class RunnableError;
	class IOutputListener;
	class IExceptionListener;

	typedef void (*ExceptionCallback)(RunnableError &exception);
	typedef void (*DataCallback)(Data &data);

	class System
	{

	public:

		/**
		*	destroys the system & everything within
		*/
		~System();

		/**
		*	creates an empty system w. the desired name
		*/
		System(std::string const& name);

		/*******************************************************************************
		*	functions related to xml - are currently being implemented
		*/

		/**
		*	creates a system from an xml file - configName must be equal to the one defined in the xml file
		*/
		//System(std::string const& name, std::string const& configFile, std::string const& configName);

		/**
		*	returns the identifier of a plugin, service, sequence or graph -> can be used to get id of an entity defined in the xml
		*	can also return the system's own identifier
		*/
		//const Identifier getId(std::string const& name);

		//some more identifier helper functions
		//std::list< Identifier > getIds();
		//std::list< Identifier > getPluginIds();
		//std::list< Identifier > getRunnableIds();
		//std::list< Identifier > getGraphIds();
		//std::list< Identifier > getServiceIds();

		//some more helpers, this time for names
		//std::list< std::string > getInputSlots(Identifier const& serviceId);
		//std::list< std::string > getOutputSlots(Identifier const& serviceId);
		//std::list< std::string > getSetupParameters(Identifier const& setupAbleId);
		//std::list< std::string > getExportedServices(Identifier const& pluginId);

		/********************************************************************************************/

		/**
		*	sets the system's plugin directory - i.e. where plugins will be searched
		*/
		void setInstallDirectory(std::string const& directory);

		/**
		*	sets the system's logfile (logging = incomplete right now)
		*/
		void setLogfile(std::string const& file);

		/**
		*	destroys everything in the system.
		*/
		void shutdown();

		/**
		*	loads a dll - assumption: classname->filename, e.g: MyPlugin -> MyPlugin.dll / MyPlugin.so / MyPlugin.dylib (release mode), MyPlugin_d.dll etc. (debug mode)
		*	plugins belong to systems that loaded them: this means different systems can have the same plugin loaded, with different setup params
		*	multiple loading of a plugin from within the same system will cause an exception
		*/
		const Identifier loadPlugin(std::string const& name, std::string const& className);

		/**
		*	calls setup of either a plugin or a service = initialization
		*	a service that is running must be stopped before it can be set up again
		*	plugins can be set up only once for reasons of safety
		*	(otherwise, services might become invalid etc.)
		*/
		void setup(Identifier const& id);

		/**
		*	returns plugin metadata as string
		*/
		const std::string getInfo(Identifier const& plugin);

		/**
		*	creates instance of service
		*/
		const Identifier createService(std::string const& name, Identifier const& plugin, std::string const& service);

		/**
		*	sets a runnable's desired update rate
		*/
		void setUpdateRate(Identifier const& id, float updatesPerSecond);

		/**
		*	initializes a service's or plugin's setup parameter, or directly sets the value of an input slot
		*	if the input slot has been linked to an output slot previously, this link will be broken
		*/
		template< typename DataType >
		void setValue(Identifier const& id, std::string const& name, DataType const& value)
		{
			EngineData data(value);
			setValueInternal(id, name, data);
		}

		/**
		*	returns value of an input slot or setup parameter. if it is an input slot, the value returned will
		*	be the one that is used for the current update cycle! (so setting a value & querying it immediately afterwards
		*	might not yield the same value)
		*/
		template< typename DataType >
		DataType const& getValue(Identifier const& id, std::string const& name) const
		{
			Poco::SharedPtr< DataType > ptr = Extract< DataType >(getValueInternal(id, name));
			return *ptr.get();
		}

		/**
		*	links output slot to input slot. the input slot's previous link will be broken.
		*/
		void linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);

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
		void registerToException(IExceptionListener &listener);

		/**
		*	unregisters exception callback for a system
		*/
		void unregisterFromException(IExceptionListener &listener);

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
		void registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener);

		/**
		*	unregisters callback for a service's output slot
		*/
		void unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener);

		/**
		*	runnable state changes
		*/
		//void registerToStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener);
		//void unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener);

		/**
		*	starts a runnable (service, synchronization, sequence)
		*/
		void start(Identifier const& runnableId);

		/**
		*	stops a runnable (service, synchronization, sequence)
		*/
		void stop(Identifier const& runnableId);

		/**
		*	stops all running (root) runnables at once
		*/
		void stopAll();

		/**
		*	starts all (root) runnables at once
		*/
		void startAll();

		/*
		*	currently not functional until the whole insert / remove thing is resolved properly
		*/
		//const Identifier createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier createSequence(std::string const& idName, std::list< Identifier > const& runnableIds);
		//const Identifier createSynchronization(std::string const& idName, std::list< Identifier > const& runnableIds);
		//void add(Identifier const& runnable, Identifier const& parent, unsigned int index);
		//void append(Identifier const& runnable, Identifier const& parent);

	private:

		//shallow copy
		System(System const& src);

		/**
		*	internally used function for setting param values
		*/
		void setValueInternal(Identifier const& id, std::string const& name, EngineData const& value);

		/**
		*	internally used function for setting param values
		*/
		const EngineData getValueInternal(Identifier const& id, std::string const& name) const;

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