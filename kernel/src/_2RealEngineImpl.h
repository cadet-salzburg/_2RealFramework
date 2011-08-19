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
#include "_2RealServiceTypedefs.h"

#include <vector>

#include "Poco/Mutex.h"

namespace _2Real
{

	/**
	*
	*/

	class PluginPool;
	class ServiceFactory;
	class Entities;
	class Identifier;
	
	class EngineImpl
	{

		friend class Engine;

	public:

		/**
		*	
		*/
		const Identifier installPlugin(std::string const& _name, std::string const& _path, std::string const& _class, Identifiers &_serviceIDs) throw(...);

		/**
		*	
		*/
		const Identifier createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB) throw(...);

		/**
		*	
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id) throw (...);

		/**
		*	
		*/
		const Identifier createMutex(std::string const& _name, Identifier const& _id) throw (...);

		/**
		*
		*/
		void setParameterValue(Identifier const& _id) throw(...);

		/**
		*
		*/
		void configureIO(std::string const& _out, Identifier const& _outID, std::string const& _in, Identifier const& _inID) throw(...);

		/**
		*	
		*/
		void dumpPluginInfo(Identifier const& _id);

		/**
		*	
		*/
		void dumpServiceInfo(Identifier const& _id);

		/**
		*	
		*/
		void registerToExceptionChange(Identifier const& _id, ExceptionCallback _callback) throw(...);

		/**
		*	
		*/
		void registerToNewDataAvailable(Identifier const& _id, NewDataCallback _callback) throw(...);

		/**
		*	
		*/
		void insertInto(Identifier const& _dst, Identifier const& _src) throw(...);

		/**
		*	
		*/
		void startAll() throw(...);

		/**
		*	
		*/
		void stopAll() throw(...);

		/**
		*
		*/
		void start(Identifier const& _id) throw(...);

		/**
		*
		*/
		void stop(Identifier const& _id) throw(...);

	private:

		/**
		*
		*/
		static EngineImpl *const instance();

		/**
		*	
		*/
		const bool retain();

		/**
		*	
		*/
		const bool release();

		/**
		*
		*/
		static EngineImpl				*s_Instance;

		/**
		*
		*/
		static Poco::Mutex				s_Mutex;

		/**
		*
		*/
		static unsigned int				s_iRefCount;

		/**
		*	
		*/
		EngineImpl();

		/**
		*	
		*/
		~EngineImpl();

		/**
		*	
		*/
		EngineImpl(const EngineImpl &_src);

		/**
		*	
		*/
		EngineImpl& operator=(const EngineImpl &_src);

		/**
		*	takes care of installed plugins
		*/
		PluginPool					*m_PluginPool;

		/**
		*	takes care of service creation
		*/
		ServiceFactory				*m_ServiceFactory;

		/**
		*	takes care of identifier creation
		*/
		Entities					*m_Entities;

		/**
		*	
		*/
		Poco::Mutex					m_Mutex;

	};

}