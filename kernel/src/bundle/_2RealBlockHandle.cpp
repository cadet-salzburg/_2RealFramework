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

#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "bundle/_2RealParameterHandle.h"
#include "bundle/_2RealBlockHandle.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	template< typename TObj >
	std::shared_ptr< TObj > checkValidity( std::weak_ptr< TObj > handle, std::string const& what )
	{
		std::shared_ptr< TObj > locked = handle.lock();
		if ( locked.get() == nullptr )
		{
			std::stringstream msg;
			msg << "nullptr access: " << what << " handle does not point to an object" << std::endl;
		}

		return locked;
	}

	namespace bundle
	{
		BlockHandle::BlockHandle() :
			mImpl()
		{
		}

		BlockHandle::BlockHandle( std::shared_ptr< FunctionBlockIOManager > io ) :
			mImpl( io )
		{
		}

		bool BlockHandle::isValid() const
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = mImpl.lock();
			return ( mgr.get() != nullptr );
		}

		InletHandle BlockHandle::getInletHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			return InletHandle( mgr->getInlet( trim ( name ) )->getInlet() );
		}

		OutletHandle BlockHandle::getOutletHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			return OutletHandle( mgr->getOutlet( trim( name ) )->getOutlet() );
		}

		ParameterHandle BlockHandle::getParameterHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			return ParameterHandle( mgr->getParameter( trim ( name ) )->getInlet() );
		}

		void BlockHandle::getAllInletHandles( InletHandles &handles )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			AbstractIOManager::InletVector &inlets = mgr->getAllInlets();
			handles.clear();
			for ( AbstractIOManager::InletVector::iterator it = inlets.begin(); it != inlets.end(); ++it )
				handles.push_back( InletHandle( ( *it )->getInlet() ) );
		}

		void BlockHandle::getAllOutletHandles( OutletHandles &handles )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			AbstractIOManager::OutletVector &outlets = mgr->getAllOutlets();
			handles.clear();
			for ( AbstractIOManager::OutletVector::iterator it = outlets.begin(); it != outlets.end(); ++it )
				handles.push_back( OutletHandle( ( *it )->getOutlet() ) );
		}

		void BlockHandle::getAllParameterHandles( ParameterHandles &handles )
		{
			std::shared_ptr< FunctionBlockIOManager > mgr = checkValidity< FunctionBlockIOManager >( mImpl, "block" );
			AbstractIOManager::ParameterVector &parameters = mgr->getAllParameters();
			handles.clear();
			for ( AbstractIOManager::ParameterVector::iterator it = parameters.begin(); it != parameters.end(); ++it )
				handles.push_back( ParameterHandle( ( *it )->getInlet() ) );
		}
	}
}