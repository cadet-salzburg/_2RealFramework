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

		System(std::string const& name);
		System(System const& src);
		~System();

		/**
		*	sets the system's plugin directory - i.e. where plugins will be searched
		*/
		void setInstallDirectory(std::string const& directory);

		/**
		*	sets the system's logfile (logging = incomplete right now)
		*/
		void setLogfile(std::string const& file);

		/**
		*	destroys everything in the system
		*/
		void shutdown();

		/**
		*	loads a dll - assumption: classname-> filename, e.g: MyPlugin -> MyPlugin.dll (release mode), MyPlugin_d.dll (debug mode)
		*	the install directory will be taken from the config.txt
		*	plugins belong to systems that loaded them: this means different systems can have the same plugin loaded, with different setup params
		*	multiple loading of a plugin from within the same system accomplishes nothing
		*/
		const Identifier loadPlugin(std::string const& name, std::string const& className);

		/**
		*	calls setup of either a plugin or a service = initialization
		*	if the service is currently running, it will be stopped
		*	warning: plugins can be set up only once for reasons of safety
		*	(in theory, it could be that a plugin does not export a certain service any more after being set up once again etc)
		*/
		void setup(Identifier const& id);

		/**
		*	returns plugin metadata
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
		*	returns value of an input slot or setup parameter
		*/
		template< typename DataType >
		DataType const& getValue(Identifier const& id, std::string const& name) const
		{
			Poco::SharedPtr< DataType > ptr = Extract< DataType >(getValueInternal(id, name));
			return *ptr.get();
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
		*	stops all of nirvanas children at once
		*/
		void stopAll();

		/**
		*	starts all of nirvanas children at once
		*/
		void startAll();

		/*
		*	currently not functional until the whole insert / remove thing is resolved
		*/
		//const Identifier createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//void add(Identifier const& runnable, Identifier const& parent, unsigned int index);
		//void append(Identifier const& runnable, Identifier const& parent);

	private:

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