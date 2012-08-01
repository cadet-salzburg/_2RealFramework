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

		void							addInlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue, AnyOptionSet const& options );
		void							addOutlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue );

		app::InletHandle &				getAppInletHandle( std::string const& name ) const;
		app::OutletHandle &				getAppOutletHandle( std::string const& name ) const;
		bundle::InletHandle &			getBundleInletHandle( std::string const& name ) const;
		bundle::OutletHandle &			getBundleOutletHandle( std::string const& name ) const;

		AppInletHandles const&			getAppInletHandles() const;
		AppOutletHandles const&			getAppOutletHandles() const;
		BundleInletHandles const&		getBundleInletHandles() const;
		BundleOutletHandles const&		getBundleOutletHandles() const;

		void							updateInletData();
		void							updateOutletData();
		void							updateInletBuffers();

		unsigned int					getInletBufferSize( std::string const& inlet ) const;

	private:

		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockUpdatePolicy		*m_UpdatePolicy;

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		InletVector						m_Inlets;
		OutletVector					m_Outlets;

		InletIO &						getInletIO( std::string const& name );
		OutletIO &						getOutletIO( std::string const& name );
		InletIO const&					getInletIO( std::string const& name ) const;
		OutletIO const&					getOutletIO( std::string const& name ) const;

		AppInletHandles					m_AppInletHandles;
		AppOutletHandles				m_AppOutletHandles;
		BundleInletHandles				m_BundleInletHandles;
		BundleOutletHandles				m_BundleOutletHandles;
	};

}