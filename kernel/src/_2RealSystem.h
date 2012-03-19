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

#include "_2RealIdentifier.h"
#include "_2RealUpdateTriggers.h"
#include "_2RealEngineData.h"
#include "_2RealCallbacks.h"

#include <string>

namespace _2Real
{

	class SystemImpl;

	class System
	{

	public:

		~System();
		System(std::string const& name);

		/**
		*	kills all runnables in the system
		*/
		void clear();

		const Identifier createService(Identifier const& pluginId, std::string const& serviceName, UpdateTriggers const& updateTriggers = UpdateTriggers::ServiceDefault());
		const Identifier createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName, UpdateTriggers const& updateTriggers = UpdateTriggers::ServiceDefault());

		void setup(Identifier const& runnableId);

		template< typename DataType >
		void setValue(Identifier const& id, std::string const& name, DataType const& value)
		{
			EngineData data(value);
			setValueInternal(id, name, data);
		}

		template< typename DataType >
		DataType const& getValue(Identifier const& id, std::string const& name) const
		{
			Poco::SharedPtr< DataType > ptr = Extract< DataType >(getValueInternal(id, name));
			return *ptr.get();
		}

		void linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);
		void unlinkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);
		void clearOutputListeners(Identifier const& outService, std::string const& outName, const bool clearCallbacks = false);
		void clearInputProviders(Identifier const& inService, std::string const& inName);

		///**
		//*	exception listener callbacks
		//*/
		//void registerToException(ExceptionCallback callback);
		//void unregisterFromException(ExceptionCallback callback);
		//void registerToException(IExceptionListener &listener);
		//void unregisterFromException(IExceptionListener &listener);

		///**
		//*	output slot callbacks
		//*/
		//void registerToNewData(Identifier const& service, std::string const& name, DataCallback callback);
		//void unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback);
		//void registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener);
		//void unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener);

		///**
		//*	runnable state changes
		//*/
		//void registerToStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener);
		//void unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener);

	private:

		System(System const& src);

		void setValueInternal(Identifier const& id, std::string const& name, EngineData const& value);
		const EngineData getValueInternal(Identifier const& id, std::string const& name) const;

		SystemImpl				*m_Impl;

		/**
		*	helper lookup table
		*/
		std::map< std::string, Identifier >		m_Lookup;
		void unique(std::string const& s) const;

	};

}