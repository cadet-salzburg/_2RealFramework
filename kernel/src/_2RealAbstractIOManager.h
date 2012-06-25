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

#include "_2RealCallbacks.h"
#include "_2RealCallbacksInternal.h"

#include <map>
#include <string>

namespace _2Real
{

	class EngineData;
	class InletHandle;
	class OutletHandle;
	class AbstractUberBlock;
	class Inlet;
	class Outlet;
	class SetupParameter;
	class TimestampedData;
	class ParameterData;

	// declared here, as all io managers need those functions
	Inlet &				createInletFromParameterData( ParameterData const& data );
	Outlet &			createOutletFromParameterData( ParameterData const& data );
	SetupParameter &	createSetupParameterFromParameterData( ParameterData const& data );

	class AbstractIOManager
	{

	public:

		typedef std::map< std::string, Inlet * >			InletMap;
		typedef std::map< std::string, Outlet * >			OutletMap;
		typedef std::map< std::string, SetupParameter * >	ParamMap;

		AbstractIOManager( AbstractUberBlock &owner );
		virtual ~AbstractIOManager();

		std::string const& getName() const;
		const unsigned int getId() const;

		virtual void					clear() = 0;

		virtual void					registerToNewData( std::string const& outName, OutletCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData ) = 0;
		virtual void					registerToNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler ) = 0;
		virtual void					unregisterFromNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler ) = 0;
		virtual void					registerToNewData( OutputCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData( OutputCallback callback, void *userData ) = 0;
		virtual void					registerToNewData( AbstractOutputCallbackHandler &handler ) = 0;
		virtual void					unregisterFromNewData( AbstractOutputCallbackHandler &handler ) = 0;

		virtual EngineData const&		getValue( std::string const& paramName ) const = 0;
		virtual void					setValue( std::string const& paramName, TimestampedData const& value ) = 0;
		virtual std::string const&		getTypename( std::string const& paramName ) const = 0;
		virtual std::string const&		getLongTypename( std::string const& paramName ) const = 0;

		virtual Inlet const&			getInlet( std::string const& name ) const = 0;
		virtual Outlet const&			getOutlet( std::string const& name ) const = 0;
		virtual SetupParameter const&	getSetupParameter( std::string const& name ) const = 0;

	protected:

		AbstractUberBlock					&m_Owner;

	};

}