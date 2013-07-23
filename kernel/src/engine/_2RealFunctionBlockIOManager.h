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

#include "engine/_2RealAbstractIOManager.h"
#include "helpers/_2RealHandleable.h"
#include "app/_2RealCallbacks.h"
#include "bundle/_2RealBlockHandle.h"

namespace _2Real
{
	namespace app
	{
		class InletHandle;
		class OutletHandle;
	}

	namespace bundle
	{
		class InletHandle;
		class OutletHandle;
		class BlockHandle;
	}
	
	class FunctionBlockStateManager;
	class FunctionBlockUpdatePolicy;
	class TypeDescriptor;

	class FunctionBlockIOManager : private AbstractIOManager, private Handleable< FunctionBlockIOManager, bundle::BlockHandle >
	{

		template< typename T >
		friend class FunctionBlock;

	public:

		FunctionBlockIOManager( AbstractUberBlock &owner );
		~FunctionBlockIOManager();

		using Handleable< FunctionBlockIOManager, bundle::BlockHandle >::getHandle;
		using Handleable< FunctionBlockIOManager, bundle::BlockHandle >::registerHandle;
		using Handleable< FunctionBlockIOManager, bundle::BlockHandle >::unregisterHandle;

		void							clear();

		void							registerToNewData( app::BlockCallback &cb );
		void							unregisterFromNewData( app::BlockCallback &cb );

		void							addBasicInlet( InletInfo const& info );
		void							addMultiInlet( InletInfo const& info );
		void							addOutlet( OutletInfo const& info );
		void							addParameter( ParameterInfo const& info );

		app::InletHandle &				getAppInletHandle( std::string const& name );
		app::OutletHandle &				getAppOutletHandle( std::string const& name );
		app::ParameterHandle &				getAppParameterHandle( std::string const& name );
		bundle::InletHandle &			getBundleInletHandle( std::string const& name ) const;
		bundle::OutletHandle &			getBundleOutletHandle( std::string const& name ) const;
		bundle::ParameterHandle &			getBundleParameterHandle( std::string const& name ) const;

		AppInletHandles const&			getAppInletHandles() const;
		AppOutletHandles const&			getAppOutletHandles() const;
		AppParameterHandles const&			getAppParameterHandles() const;
		BundleInletHandles const&		getBundleInletHandles() const;
		BundleOutletHandles const&		getBundleOutletHandles() const;
		BundleParameterHandles const&		getBundleParameterHandles() const;

		void							updateInletData();
		void							updateOutletData();
		//void							clearInletBuffers();
		void							updateInletBuffers( const bool enableTriggering );
		void							updateSetupParameters();

		/* moved to public 13/05/2013 - using this function might cause sync issues?? */

		AbstractInletIO &				getInletIO( std::string const& name );
		OutletIO &						getOutletIO( std::string const& name );
		ParameterIO &					getParameterIO( std::string const& name );
		AbstractInletIO const&			getInletIO( std::string const& name ) const;
		OutletIO const&					getOutletIO( std::string const& name ) const;
		ParameterIO const&				getParameterIO( std::string const& name ) const;

	private:

		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockUpdatePolicy		*m_UpdatePolicy;

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		mutable Poco::FastMutex			m_ParameterAccess;
		InletVector						m_Inlets;
		OutletVector					m_Outlets;
		ParameterVector					m_Parameters;

		//AbstractInletIO &				getInletIO( std::string const& name );
		//OutletIO &						getOutletIO( std::string const& name );
		//AbstractInletIO const&			getInletIO( std::string const& name ) const;
		//OutletIO const&					getOutletIO( std::string const& name ) const;

		AppInletHandles					m_AppInletHandles;
		AppOutletHandles				m_AppOutletHandles;
		AppParameterHandles				m_AppParameterHandles;
		BundleInletHandles				m_BundleInletHandles;
		BundleOutletHandles				m_BundleOutletHandles;
		BundleParameterHandles			m_BundleParameterHandles;
	};

}