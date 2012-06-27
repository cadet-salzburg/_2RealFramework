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
	class UpdatePolicy;

	class AbstractInletBasedTrigger;

	class FunctionBlockIOManager : public AbstractIOManager
	{

	public:

		typedef std::vector< Inlet* >				InletVector;
		typedef std::vector< Outlet * >				OutletVector;
		typedef std::vector< SetupParameter * >		ParamVector;

		FunctionBlockIOManager( AbstractUberBlock &owner );
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

		void						addInlet( ParameterData const& data );
		void						addOutlet( ParameterData const& data );
		void						addSetupParameter( ParameterData const& data );

		Inlet const&				getInlet( std::string const& name ) const;
		Outlet const&				getOutlet( std::string const& name ) const;
		SetupParameter const&		getSetupParameter( std::string const& name ) const;

		Inlet &						getInlet( std::string const& name );
		Outlet &					getOutlet( std::string const& name );
		SetupParameter &			getSetupParameter( std::string const& name );

		// stuff that is exclusive to this class: called by function block & function state mgr

		InletHandle					createInletHandle( std::string const& name );
		OutletHandle				createOutletHandle( std::string const& name );

		void						updateInletValues();
		void						updateOutletValues();
		void						updateInletBuffers();

	private:

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		mutable Poco::FastMutex			m_ParamAccess;
		InletVector						m_Inlets;
		OutletVector					m_Outlets;
		ParamVector						m_Params;

		Inlet const *const				findInlet( std::string const& name ) const;
		Outlet const *const				findOutlet( std::string const& name ) const;
		SetupParameter const *const		findSetupParameter( std::string const& name ) const;

		Inlet *							findInlet( std::string const& name );
		Outlet *						findOutlet( std::string const& name );
		SetupParameter *				findSetupParameter( std::string const& name );

		mutable Poco::FastMutex			m_CallbackAccess;
		OutletFunctionCallbacks			m_OutletFunctionCallbacks;
		OutletCallbackHandlers			m_OutletCallbackHandlers;
		OutputFunctionCallbacks			m_OutputFunctionCallbacks;
		OutputCallbackHandlers			m_OutputCallbackHandlers;

		std::list< OutputData >			m_OutputData;

	};

}