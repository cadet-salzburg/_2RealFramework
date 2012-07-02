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
#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"

#include "_2RealPoco.h"

#include <map>
#include <string>

namespace _2Real
{
	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

	namespace bundle
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}
	
	class FunctionBlock;
	class FunctionBlockStateManager;
	class FunctionBlockUpdatePolicy;

	class FunctionBlockIOManager : public AbstractIOManager
	{

	public:

		typedef std::vector< Inlet * >					InletVector;
		typedef std::vector< Inlet * >::iterator		InletIterator;	
		typedef std::vector< Outlet * >					OutletVector;
		typedef std::vector< Outlet * >::iterator		OutletIterator;	
		typedef std::vector< Parameter * >				ParamVector;
		typedef std::vector< Parameter * >::iterator	ParamIterator;	

		FunctionBlockIOManager( FunctionBlock &owner );
		~FunctionBlockIOManager();

		// stuff that is inherited

		void						clear();

		void						registerToNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData );
		void						unregisterFromNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData );
		void						registerToNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void						unregisterFromNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void						registerToNewData( app::BlockDataCallback callback, void *userData );
		void						unregisterFromNewData( app::BlockDataCallback callback, void *userData );
		void						registerToNewData( app::AbstractBlockDataCallbackHandler &handler );
		void						unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler );

		// stuff that is exclusive to this class: called by function block & function state mgr

		void						addInlet( ParamData const& data );
		void						addOutlet( ParamData const& data );
		void						addParameter( ParamData const& data );

		app::InletHandle			createAppInletHandle( std::string const& name );
		app::OutletHandle			createAppOutletHandle( std::string const& name );
		app::ParameterHandle		createAppParameterHandle( std::string const& name );

		bundle::InletHandle			createBundleInletHandle( std::string const& name );
		bundle::OutletHandle		createBundleOutletHandle( std::string const& name );
		bundle::ParameterHandle		createBundleParameterHandle( std::string const& name );

		void						updateInletValues();
		void						updateOutletValues();
		void						updateInletBuffers();
		void						updateParameterValues();

	private:

		friend class FunctionBlock;

		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockUpdatePolicy		*m_UpdatePolicy;

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		mutable Poco::FastMutex			m_ParamAccess;
		InletVector						m_Inlets;
		OutletVector					m_Outlets;
		ParamVector						m_Params;
				
		Inlet &						getInlet( std::string const& name );
		Outlet &					getOutlet( std::string const& name );
		Parameter &					getParameter( std::string const& name );

		Inlet *							findInlet( std::string const& name );
		Outlet *						findOutlet( std::string const& name );
		Parameter *						findParameter( std::string const& name );

		// ugh
		mutable Poco::FastMutex					m_CallbackAccess;
		app::OutletDataFunctionCallbacks		m_OutletDataFunctionCallbacks;
		app::OutletDataCallbackHandlers			m_OutletDataCallbackHandlers;
		app::BlockDataFunctionCallbacks			m_BlockDataFunctionCallbacks;
		app::BlockDataCallbackHandlers			m_BlockDataCallbackHandlers;

	};

}