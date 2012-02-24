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

	class EngineImpl;
	class Data;
	class Identifier;
	class RunnableError;

	class IOutputListener;
	class IExceptionListener;
	class IStateChangeListener;

	typedef void (*ExceptionCallback)(RunnableError &exception);
	typedef void (*DataCallback)(Data &data);
	typedef void (*StateChangeCallback)(std::string &newState);

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

		/**
		*	creates empty system w. given name & loads the xml
		*/
		System(std::string const& name, std::string const& xmlFile);

		/**
		*	sets the system's logfile
		*/
		void setLogfile(std::string const& file);

		/**
		*	destroys everything in the system.
		*/
		void clear();

		/**
		*	calls setup of a runnable
		*	if the runnbale is currently running, it needs to be stopped
		*/
		void setup(Identifier const& id);

		/**
		*	creates instance of service
		*	the name of the returned identifier will be auto-generated
		*/
		const Identifier createService(Identifier const& pluginId, std::string const& serviceName);

		/**
		*	creates instance of service
		*/
		const Identifier createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName);

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
		*	links out with in
		*/
		void linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);

		/**
		*	removes the link between out and in (does nothing if they are not actually linked)
		*/
		void unlinkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);

		/**
		*	removes all listening input slots from an output slot, and optionally all registered callbacks too
		*/
		void clearOutputListeners(Identifier const& outService, std::string const& outName, const bool clearCallbacks = false);

		/**
		*	removes all output slots from an input slot
		*/
		void clearInputProviders(Identifier const& inService, std::string const& inName);

		/**
		*	exception listener callbacks
		*/
		void registerToException(ExceptionCallback callback);
		void unregisterFromException(ExceptionCallback callback);
		void registerToException(IExceptionListener &listener);
		void unregisterFromException(IExceptionListener &listener);

		/**
		*	output slot callbacks
		*/
		void registerToNewData(Identifier const& service, std::string const& name, DataCallback callback);
		void unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback);
		void registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener);
		void unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener);

		/**
		*	runnable state changes
		*/
		void registerToStateChange(Identifier const& runnableId, StateChangeCallback callback);
		void unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback);
		void registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener);
		void unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener);

		/**
		*	starts a runnable
		*/
		void start(Identifier const& runnableId);

		/**
		*	stops a runnable
		*/
		void stop(Identifier const& runnableId);

		/**
		*	stops all running runnables at once
		*/
		void stopAll();

		/**
		*	starts all runnables at once
		*/
		void startAll();

		/*
		*	currently not functional until the whole insert / remove thing is resolved properly
		*/

		/**
		*
		*/
		//const Identifier createSequenceFromXML();
		//const Identifier createSynchronizationFromXML();

		//const Identifier createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier createSequence(std::string const& idName, std::list< Identifier > const& runnableIds);
		//const Identifier createSynchronization(std::string const& idName, std::list< Identifier > const& runnableIds);
		//void add(Identifier const& runnable, Identifier const& parent, unsigned int index);
		//void append(Identifier const& runnable, Identifier const& parent);

		/*******************************************************************************
		*	functions related to xml - are currently being implemented
		*/

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

	private:

		/**
		*	shallow copy - this does not actually create new services
		*/
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
		Identifier								m_Id;

		/**
		*	the 2real EngineImpl
		*/
		EngineImpl								&m_EngineImpl;

		/**
		*	helper lookup table
		*/
		std::map< std::string, Identifier >		m_Lookup;
		void unique(std::string const& s) const;

	};
}