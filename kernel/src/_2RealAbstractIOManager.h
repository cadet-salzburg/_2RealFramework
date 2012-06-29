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
	class InletBuffer;
	class Outlet;
	class Parameter;
	class TimestampedData;
	class ParameterData;

	// declared here, as all io managers need those functions
	Inlet*					createInletFromParameterData( AbstractUberBlock &owner, ParameterData const& data );
	Outlet *				createOutletFromParameterData(AbstractUberBlock &owner, ParameterData const& data );
	Parameter *				createParameterFromParameterData( AbstractUberBlock &owner, ParameterData const& data );

	class AbstractIOManager
	{

	public:

		AbstractIOManager( AbstractUberBlock &owner );
		virtual ~AbstractIOManager();

		std::string const& getName() const;
		unsigned int getId() const;

		virtual void					registerToNewData( std::string const& outName, app::OutletDataCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData( std::string const& outName, app::OutletDataCallback callback, void *userData ) = 0;
		virtual void					registerToNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler ) = 0;
		virtual void					unregisterFromNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler ) = 0;
		virtual void					registerToNewData( app::BlockDataCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData( app::BlockDataCallback callback, void *userData ) = 0;
		virtual void					registerToNewData( app::AbstractBlockDataCallbackHandler &handler ) = 0;
		virtual void					unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler ) = 0;

		// not sure if those are really needed
		virtual Inlet &					getInlet( std::string const& name ) = 0;
		virtual Outlet &				getOutlet( std::string const& name ) = 0;
		virtual Parameter &				getParameter( std::string const& name ) = 0;

	protected:

		AbstractUberBlock					&m_Owner;

	};

}