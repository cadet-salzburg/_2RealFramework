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

#include "bundle/_2RealFunctionBlockMetainfo.h"
#include "bundle/_2RealInletMetainfo.h"
#include "bundle/_2RealOutletMetainfo.h"
#include "bundle/_2RealParameterMetainfo.h"
#include "bundle/_2RealInSlotPolicy.h"
#include "engine/_2RealSharedServiceMetainfo.h"

namespace _2Real
{
	namespace bundle
	{
		FunctionBlockMetainfo::FunctionBlockMetainfo( std::shared_ptr< SharedServiceMetainfo > info ) :
			mImpl( info )
		{
		}

		OutletMetainfo FunctionBlockMetainfo::createOutlet( std::string const& name )
		{
			return OutletMetainfo( mImpl.lock()->createOutlet( name ) );
		}

		InletMetainfo FunctionBlockMetainfo::createInlet( std::string const& name )
		{
			return InletMetainfo( mImpl.lock()->createInlet( name ) );
		}

		ParameterMetainfo FunctionBlockMetainfo::createParameter( std::string const& name )
		{
			return ParameterMetainfo( mImpl.lock()->createParameter( name ) );
		}

		void FunctionBlockMetainfo::setDescription( std::string const& description )
		{
			mImpl.lock()->setDescription( description );
		}

		void FunctionBlockMetainfo::setBlockClassInternal( std::shared_ptr< AbstractSharedServiceFactory > f )
		{
			mImpl.lock()->setFactory( f );
		}

		void FunctionBlockMetainfo::setDependencies( std::vector< std::string > const& dependencies )
		{
			mImpl.lock()->setDependencies( dependencies );
		}

		void FunctionBlockMetainfo::setDefaultUpdatePolicy( InSlotPolicy const& policy  )
		{
			mImpl.lock()->setDefaultUpdatePolicy( static_cast< std::shared_ptr< DefaultPolicy > >( policy ) );
		}

		void FunctionBlockMetainfo::setDefaultSetupPolicy( InSlotPolicy const& policy  )
		{
			mImpl.lock()->setDefaultUpdatePolicy( static_cast< std::shared_ptr< DefaultPolicy > >( policy ) );
		}
	}
}