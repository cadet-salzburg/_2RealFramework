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

#include "_2RealBlockIdentifier.h"
#include "_2RealBundleIdentifier.h"
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

		System( std::string const& name );
		~System();

		void clear();

		const BlockIdentifier
		createBlock( BundleIdentifier const& bundle, std::string const& blockName, UpdatePolicy const& triggers = UpdatePolicy() );

		void setup( BlockIdentifier const& block );
		void link( BlockIdentifier const& out, std::string const& outlet, BlockIdentifier const& in, std::string const& inlet );

		template< typename Datatype >
		void setValue( BlockIdentifier const& block, std::string const& paramName, Datatype const& value )
		{
			EngineData data( value );
			setValueInternal( block, paramName, data );
		}

		template< typename Datatype >
		void insertValue( BlockIdentifier const& block, std::string const& paramName, Datatype const& value )
		{
			EngineData data( value );
			insertValueInternal( block, paramName, data );
		}

		template< typename Datatype >
		Datatype const& getValue( BlockIdentifier const& block, std::string const& paramName ) const
		{
			std::shared_ptr< Datatype > ptr = extractFrom< Datatype >( getValueInternal( block, paramName ) );
			return *ptr.get();
		}

		void registerToException( ExceptionCallback callback, void *userData = nullptr );
		void unregisterFromException( ExceptionCallback callback, void *userData = nullptr );

		template< typename Callable >
		void registerToException( Callable &callable, void ( Callable::*callback )( BlockError& ) )
		{
			AbstractExceptionCallbackHandler *handler = new ExceptionCallbackHandler< Callable >( callable, callback );
			registerToExceptionInternal( *handler );
		}

		template< typename Callable >
		void unregisterFromException( Callable &callable, void ( Callable::*callback )( BlockError& ) )
		{
			AbstractExceptionCallbackHandler *handler = new ExceptionCallbackHandler< Callable >( callable, callback );
			unregisterFromExceptionInternal( *handler );
		}

		void registerToNewData( BlockIdentifier const& block, std::string const& outlet, OutletCallback callback, void *userData = nullptr );
		void unregisterFromNewData( BlockIdentifier const& block, std::string const& outlet, OutletCallback callback, void *userData = nullptr );

		template< typename Callable >
		void registerToNewData( BlockIdentifier const& block, std::string const& outlet, Callable &callable, void ( Callable::*callback )( OutputData& ) )
		{
			AbstractOutletCallbackHandler *handler = new OutletCallbackHandler< Callable >( callable, callback );
			registerToNewDataInternal( block, outlet, *handler );
		}

		template< typename Callable >
		void unregisterFromNewData( BlockIdentifier const& block, std::string const& outlet, Callable &callable, void ( Callable::*callback )( OutputData& ) )
		{
			AbstractOutletCallbackHandler *handler = new OutletCallbackHandler< Callable >( callable, callback );
			unregisterFromNewDataInternal( block, outlet, *handler );
		}

		void registerToNewData( BlockIdentifier const& block, OutputCallback callback, void *userData = nullptr );
		void unregisterFromNewData( BlockIdentifier const& block, OutputCallback callback, void *userData = nullptr );

		template< typename Callable >
		void registerToNewData( BlockIdentifier const& block, Callable &callable, void ( Callable::*callback )( std::list< OutputData > ) )
		{
			AbstractOutputCallbackHandler *handler = new OutputCallbackHandler< Callable >( callable, callback );
			registerToNewDataInternal( block, *handler );
		}

		template< typename Callable >
		void unregisterFromNewData( BlockIdentifier const& block, Callable &callable, void ( Callable::*callback )( std::list< OutputData > ) )
		{
			AbstractOutputCallbackHandler *handler = new OutputCallbackHandler< Callable >( callable, callback );
			unregisterFromNewDataInternal( block, *handler );
		}

	private:

		System(System const& src);
		System& operator=(System const& src);

		void				setValueInternal( BlockIdentifier const& id, std::string const& name, EngineData const& value );
		void				insertValueInternal( BlockIdentifier const& id, std::string const& name, EngineData const& value );
		const EngineData	getValueInternal( BlockIdentifier const& id, std::string const& name ) const;
		void				registerToNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler );
		void				unregisterFromNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler );
		void				registerToNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler );
		void				unregisterFromNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler );
		void				registerToExceptionInternal( AbstractExceptionCallbackHandler &handler );
		void				unregisterFromExceptionInternal( AbstractExceptionCallbackHandler &handler );

		SystemImpl			*m_Impl;

	};

}