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

	class SystemBlock;

	class System
	{

	public:

		System( std::string const& name );
		~System();

		void clear();

		// creates block & adds it to system
		// default policy: all data must be new & 30 updates per second
		const BlockIdentifier createBlock( BundleIdentifier const& bundle, std::string const& blockName );

		// calls setup once the next update cycle is finished
		// or immediately if block was never started
		void setup( BlockIdentifier const& block );

		// starts the block -> causes it to be updated
		// throws a InvalidStateChangeException if block was not set succesfully
		void start( BlockIdentifier const& block );

		// stops the block
		// will block for up to the specified amount of time, and throw a TimeOutException if unsuccsessful
		void stop( BlockIdentifier const& block, const long timeout = LONG_MAX );

		// throws a InvalidStateChangeException if block was not set succesfully
		// stops the block, if you don't do it yourself before
		// then, carries out a single update
		void singleStep( BlockIdentifier const& block );

		// calls shut down, and destroys the block
		// will stop the block if you don't do it before
		void destroy( BlockIdentifier const& block );

		// update cycle needs to be finished for this
		void setPolicy( BlockIdentifier const& block, UpdatePolicy const& policy );

		void link( BlockIdentifier const& out, std::string const& outlet, BlockIdentifier const& in, std::string const& inlet );

		// for setup param: sets value, can be read during next setup
		// for inlet: resets all links, sets value to value
		// otherwise: not found exception
		// inlet & update policy -> 'data new condition' will be met only when the value is first set
		template< typename Datatype >
		void setValue( BlockIdentifier const& block, std::string const& paramName, Datatype const& value )
		{
			setValueInternal( block, paramName, EngineData( value ) );
		}

		// for setup: returns the value ( if it was never set -> default value from metadata )
		// for inlet: returns what exactly?
		// for outlet: returns the value of the most recent update cycle
		template< typename Datatype >
		Datatype const& getValue( BlockIdentifier const& block, std::string const& paramName ) const
		{
			Datatype const& val = extractFrom< Datatype >( getValueInternal( block, paramName ) );
			return val;
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

		void registerToOutletData( BlockIdentifier const& block, std::string const& outlet, OutletCallback callback, void *userData = nullptr );
		void unregisterFromOutletData( BlockIdentifier const& block, std::string const& outlet, OutletCallback callback, void *userData = nullptr );

		template< typename Callable >
		void registerToOutletData( BlockIdentifier const& block, std::string const& outlet, Callable &callable, void ( Callable::*callback )( OutputData& ) )
		{
			AbstractOutletCallbackHandler *handler = new OutletCallbackHandler< Callable >( callable, callback );
			registerToNewDataInternal( block, outlet, *handler );
		}

		template< typename Callable >
		void unregisterFromOutletData( BlockIdentifier const& block, std::string const& outlet, Callable &callable, void ( Callable::*callback )( OutputData& ) )
		{
			AbstractOutletCallbackHandler *handler = new OutletCallbackHandler< Callable >( callable, callback );
			unregisterFromNewDataInternal( block, outlet, *handler );
		}

		void registerToAllOutletData( BlockIdentifier const& block, OutputCallback callback, void *userData = nullptr );
		void unregisterFromAllOutletData( BlockIdentifier const& block, OutputCallback callback, void *userData = nullptr );

		template< typename Callable >
		void registerToAllOutletData( BlockIdentifier const& block, Callable &callable, void ( Callable::*callback )( std::list< OutputData > ) )
		{
			AbstractOutputCallbackHandler *handler = new OutputCallbackHandler< Callable >( callable, callback );
			registerToNewDataInternal( block, *handler );
		}

		template< typename Callable >
		void unregisterFromAllOutletData( BlockIdentifier const& block, Callable &callable, void ( Callable::*callback )( std::list< OutputData > ) )
		{
			AbstractOutputCallbackHandler *handler = new OutputCallbackHandler< Callable >( callable, callback );
			unregisterFromNewDataInternal( block, *handler );
		}

	private:

		System(System const& src);
		System& operator=(System const& src);

		void				setValueInternal( BlockIdentifier const& id, std::string const& name, EngineData const& value );
		EngineData const&	getValueInternal( BlockIdentifier const& id, std::string const& name ) const;

		void				registerToNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler );
		void				unregisterFromNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler );
		void				registerToNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler );
		void				unregisterFromNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler );
		void				registerToExceptionInternal( AbstractExceptionCallbackHandler &handler );
		void				unregisterFromExceptionInternal( AbstractExceptionCallbackHandler &handler );

		SystemBlock			*m_Impl;

	};

}