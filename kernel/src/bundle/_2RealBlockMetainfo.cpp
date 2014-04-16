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

#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealIoSlotMetainfo.h"
#include "bundle/_2RealUpdatePolicyMetainfo.h"
#include "engine/_2RealBlockMetainfoImpl.h"
#include "engine/_2RealUpdatePolicyMetainfoImpl.h"

namespace _2Real
{
	namespace bundle
	{
		BlockMetainfo::BlockMetainfo( std::shared_ptr< BlockMetainfoImpl > info ) :
			mImpl( info )
		{
		}

		std::string BlockMetainfo::getName() const
		{
			return mImpl->getName();
		}

		void BlockMetainfo::setBlockClassInternal( std::shared_ptr< BlockFactory_I > factory )
		{
			mImpl->setFactory( factory );
		}

		void BlockMetainfo::setDescription( std::string const& description )
		{
			mImpl->setDescription( description );
		}

		OutletMetainfo BlockMetainfo::getOutletMetainfo( std::string const& name )
		{
			return OutletMetainfo( mImpl->getOutletMetainfo( name ) );
		}

		InletMetainfo BlockMetainfo::getInletMetainfo( std::string const& name )
		{
			return InletMetainfo( mImpl->getInletMetainfo( name ) );
		}

		ParameterMetainfo BlockMetainfo::getParameterMetainfo( std::string const& name )
		{
			return ParameterMetainfo( mImpl->getParameterMetainfo( name ) );
		}

		void BlockMetainfo::setDependencies( std::vector< std::string > const& dependencies )
		{
			mImpl->setDependencies( dependencies );
		}

		UpdatePolicyMetainfo BlockMetainfo::getUpdatePolicyMetainfo()
		{
			return UpdatePolicyMetainfo( mImpl->getDefaultUpdatePolicy() );
		}
	}
}