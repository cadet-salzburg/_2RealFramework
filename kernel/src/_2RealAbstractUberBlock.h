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
#include "app/_2RealCallbacks.h"

namespace _2Real
{

	class Inlet;
	class Outlet;
	class Exception;

	class AbstractUberBlock
	{

	public:

		AbstractUberBlock( Identifier const& id );
		virtual ~AbstractUberBlock();

		Identifier	const&						getIdentifier() const;
		std::string const&						getName() const;
		unsigned int							getId() const;

		virtual void							registerToNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData ) = 0;
		virtual void							unregisterFromNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData ) = 0;
		virtual void							registerToNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler ) = 0;
		virtual void							unregisterFromNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler ) = 0;
		virtual void							registerToNewData( app::BlockDataCallback callback, void *userData ) = 0;
		virtual void							unregisterFromNewData( app::BlockDataCallback callback, void *userData ) = 0;
		virtual void							registerToNewData( app::AbstractBlockDataCallbackHandler &handler ) = 0;
		virtual void							unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler ) = 0;

		virtual void							setUp() = 0;
		virtual void							start() = 0;
		virtual void							stop( const bool blocking, const long timeout ) = 0;
		virtual void							prepareForShutDown() = 0;
		virtual bool							shutDown( const long timeout ) = 0;

		virtual void							createLink( Inlet &inlet, Outlet &outlet ) = 0;
		virtual void							destroyLink( Inlet &inlet, Outlet &outlet ) = 0;

		virtual void							updateWhenInletDataNew( Inlet &inlet ) = 0;
		virtual void							updateWhenInletDataValid( Inlet &inlet ) = 0;
		virtual void							updateWhenAllInletDataNew() = 0;
		virtual void							updateWhenAllInletDataValid() = 0;
		virtual void							updateWithFixedRate( const double updatesPerSecond ) = 0;

		virtual void							handleException( Exception &e ) = 0;

	protected:

		Identifier								const m_Identifier;

	};

}