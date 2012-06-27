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
#include "_2RealCallbacks.h"
#include "_2RealUpdatePolicyHandle.h"

#include <string>

namespace _2Real
{

	class Inlet;
	class Outlet;
	class SetupParameter;
	class TimestampedData;
	class UpdatePolicy;
	class EngineData;
	class AbstractUberBlockBasedTrigger;

	class AbstractUberBlock
	{

	public:

		AbstractUberBlock( BlockIdentifier const& id );
		virtual ~AbstractUberBlock();

		BlockIdentifier	const&					getIdentifier() const;
		std::string const&						getName() const;
		unsigned int							getId() const;

		virtual void							addSubBlock( AbstractUberBlock &subBlock, AbstractUberBlockBasedTrigger *trigger ) = 0;
		virtual void							removeSubBlock( AbstractUberBlock &subBlock ) = 0;
		virtual void							addSuperBlock( AbstractUberBlock &superBlock, AbstractUberBlockBasedTrigger *trigger ) = 0;
		virtual void							removeSuperBlock( AbstractUberBlock &superBlock ) = 0;

		virtual void							registerToNewData( std::string const& outlet, OutletCallback callback, void *userData ) = 0;
		virtual void							unregisterFromNewData( std::string const& outlet, OutletCallback callback, void *userData ) = 0;
		virtual void							registerToNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler ) = 0;
		virtual void							unregisterFromNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler ) = 0;
		virtual void							registerToNewData( OutputCallback callback, void *userData ) = 0;
		virtual void							unregisterFromNewData( OutputCallback callback, void *userData ) = 0;
		virtual void							registerToNewData( AbstractOutputCallbackHandler &handler ) = 0;
		virtual void							unregisterFromNewData( AbstractOutputCallbackHandler &handler ) = 0;

		virtual EngineData const&				getValue( std::string const& paramName ) const = 0;
		virtual std::string const&				getTypename( std::string const& paramName ) const = 0;
		virtual std::string const&				getLongTypename( std::string const& paramName ) const = 0;
		virtual void							setValue( std::string const& paramName, TimestampedData const& value ) = 0;

		virtual void							setUp() = 0;
		virtual void							start() = 0;
		virtual void							stop( const bool blocking, const long timeout ) = 0;
		virtual void							prepareForShutDown() = 0;
		virtual bool							shutDown( const long timeout ) = 0;

		virtual Inlet &							getInlet( std::string const& name ) = 0;
		virtual Outlet &						getOutlet( std::string const& name ) = 0;
		virtual SetupParameter &				getSetupParameter( std::string const& name ) = 0;

		virtual UpdatePolicyHandle				getUpdatePolicyHandle() const = 0;

	protected:

		BlockIdentifier							const m_Identifier;

	};

}