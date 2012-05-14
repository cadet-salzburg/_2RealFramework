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
#include "_2RealUpdatePolicy.h"
#include "_2RealEngineData.h"
#include "_2RealCallbacks.h"

#include <string>

namespace _2Real
{

	class SystemImpl;

	class System
	{

	public:

		System(std::string const& name);
		~System();

		void clear();

		const Identifier
		createService(Identifier const& pluginId, std::string const& serviceName, UpdatePolicy const& triggers = UpdatePolicy());

		const Identifier
		createSynchronization(std::list< Identifier > const& blockIds);

		void setup(Identifier const& serviceId);

		void link(Identifier const& outService, std::string const& outlet, Identifier const& inService, std::string const& inlet);

		template< typename DataType >
		void setValue(Identifier const& id, std::string const& name, DataType const& value)
		{
			EngineData data(value);
			setValueInternal(id, name, data);
		}

		template< typename DataType >
		void insertValue(Identifier const& id, std::string const& name, DataType const& value)
		{
			EngineData data(value);
			insertValueInternal(id, name, data);
		}

		template< typename DataType >
		DataType const& getValue(Identifier const& id, std::string const& name) const
		{
			Poco::SharedPtr< DataType > ptr = Extract< DataType >( getValueInternal(id, name) );
			return *ptr.get();
		}

		//void unlinkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName);

		//void registerToException(ExceptionCallback callback);
		//void unregisterFromException(ExceptionCallback callback);
		//void registerToException(IExceptionListener &listener);
		//void unregisterFromException(IExceptionListener &listener);

		void registerToNewData( Identifier const& service, std::string const& outletName, DataCallback callback, void *userData = NULL );
		void unregisterFromNewData( Identifier const& service, std::string const& outletName, DataCallback callback, void *userData = NULL );

		template< typename Callable >
		void registerToNewData( Identifier const& service, std::string const& outletName, Callable &callable, void ( Callable::*callback )( Data& ) )
		{
			AbstractDataCallbackHandler *handler = new DataCallbackHandler< Callable >( callable, callback );
			registerToNewDataInternal( service, outletName, *handler );
		}

		template< typename Callable >
		void unregisterFromNewData( Identifier const& service, std::string const& outletName, Callable &callable, void ( Callable::*callback )( Data& ) )
		{
			AbstractDataCallbackHandler *handler = new DataCallbackHandler< Callable >( callable, callback );
			unregisterFromNewDataInternal( service, outletName, *handler );
		}

	private:

		System(System const& src);
		System& operator=(System const& src);

		void				setValueInternal( Identifier const& id, std::string const& name, EngineData const& value );
		void				insertValueInternal( Identifier const& id, std::string const& name, EngineData const& value );
		const EngineData	getValueInternal( Identifier const& id, std::string const& name ) const;
		void				registerToNewDataInternal( Identifier const& service, std::string const& outletName, AbstractDataCallbackHandler &handler );
		void				unregisterFromNewDataInternal( Identifier const& service, std::string const& outletName, AbstractDataCallbackHandler &handler );

		SystemImpl			*m_Impl;

	};

}