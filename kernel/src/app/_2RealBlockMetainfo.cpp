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

#include "app/_2RealBlockMetainfo.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealSharedServiceMetainfo.h"

#include "app/_2RealInletmetainfo.h"
#include "app/_2RealOutletMetainfo.h"
#include "app/_2RealParameterMetainfo.h"

namespace _2Real
{
	namespace app
	{
		BlockMetainfo::BlockMetainfo( std::shared_ptr< const SharedServiceMetainfo > meta ) :
			mImpl( meta )
		{
		}

		bool BlockMetainfo::isValid() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		bool BlockMetainfo::isSingleton() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return meta->isSingleton();
		}

		std::string BlockMetainfo::getName() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return meta->getName();
		}

		std::string BlockMetainfo::getDescription() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return meta->getDescription();
		}

		std::vector< std::string > BlockMetainfo::getDependenciesByName() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return meta->getDependencies();
		}

		std::vector< InletMetainfo > BlockMetainfo::getInlets() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );		
			std::vector< InletMetainfo > result;
			for ( auto it : meta->getInletMetainfos() )
				result.push_back( InletMetainfo( it ) );
			return result;
		}

		std::vector< OutletMetainfo > BlockMetainfo::getOutlets() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			std::vector< OutletMetainfo > result;
			for ( auto it : meta->getOutletMetainfos() )
				result.push_back( OutletMetainfo( it ) );
			return result;
		}

		std::vector< ParameterMetainfo > BlockMetainfo::getParameters() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			std::vector< ParameterMetainfo > result;
			for ( auto it : meta->getParameterMetainfos() )
				result.push_back( ParameterMetainfo( it ) );
			return result;
		}

		InletMetainfo BlockMetainfo::getInlet( std::string const& name )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return InletMetainfo( meta->getInletMetainfo( name ) );
		}

		OutletMetainfo BlockMetainfo::getOutlet( std::string const& name )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return OutletMetainfo( meta->getOutletMetainfo( name ) );
		}

		ParameterMetainfo BlockMetainfo::getParameter( std::string const& name )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			return ParameterMetainfo( meta->getParameterMetainfo( name ) );
		}

		//std::shared_ptr< const UpdatePolicyMetainfo > BlockMetainfo::getDefaultUpdatePolicy() const
		//{
		//	std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
		//	return meta->getDefaultUpdatePolicy();
		//}
	}
}