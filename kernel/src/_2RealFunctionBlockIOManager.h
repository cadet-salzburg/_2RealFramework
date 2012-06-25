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

#include "_2RealAbstractIOManager.h"

#include "Poco/Mutex.h"

#include <map>
#include <string>

namespace _2Real
{

	class BlockData;
	class ParameterData;
	class EngineData;
	class TimestampedData;
	class InletHandle;
	class OutletHandle;
	class AbstractUberBlock;
	class BufferPolicy;
	class Inlet;
	class Outlet;
	class SetupParameter;
	class UpdatePolicyImpl;

	class AbstractInletBasedTrigger;

	class FunctionBlockIOManager : public AbstractIOManager
	{

	public:

		FunctionBlockIOManager(AbstractUberBlock &owner);
		~FunctionBlockIOManager();

		// stuff that is inherited

		void						clear();

		void						registerToNewData( std::string const& outName, OutletCallback callback, void *userData );
		void						unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData );
		void						registerToNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler );
		void						unregisterFromNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler );
		void						registerToNewData( OutputCallback callback, void *userData );
		void						unregisterFromNewData( OutputCallback callback, void *userData );
		void						registerToNewData( AbstractOutputCallbackHandler &handler );
		void						unregisterFromNewData( AbstractOutputCallbackHandler &handler );

		EngineData const&			getValue( std::string const& paramName ) const;
		void						setValue( std::string const& paramName, TimestampedData const& value );
		std::string const&			getTypename( std::string const& paramName ) const;
		std::string const&			getLongTypename( std::string const& paramName ) const;

		Inlet const&				getInlet( std::string const& name ) const;
		Outlet const&				getOutlet( std::string const& name ) const;
		SetupParameter const&		getSetupParameter( std::string const& name ) const;

		// stuff that is exclusive to this class:

		InletHandle					createInletHandle( std::string const& name );
		OutletHandle				createOutletHandle( std::string const& name );

		void						updateInletValues();
		void						updateOutletValues();
		void						updateInletBuffers();

		void						initFrom( BlockData const& meta );

	private:

		// i could write lots of stupid get functions... but frankly, i don't see
		// the point : function block io mgr & state mgr are totally tied to each other anyway,
		// so let the state mgr do whatever it likes
		friend class FunctionBlockStateManager;

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		mutable Poco::FastMutex			m_ParamAccess;
		AbstractIOManager::InletMap		m_Inlets;
		AbstractIOManager::OutletMap	m_Outlets;
		AbstractIOManager::ParamMap		m_Params;

		OutletFunctionCallbacks			m_OutletFunctionCallbacks;
		OutletCallbackHandlers			m_OutletCallbackHandlers;
		OutputFunctionCallbacks			m_OutputFunctionCallbacks;
		OutputCallbackHandlers			m_OutputCallbackHandlers;

		std::list< OutputData >			m_OutputData;

	};

}