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
		BlockMetainfo::BlockMetainfo() :
			mImpl()
		{
		}

		BlockMetainfo::BlockMetainfo( std::shared_ptr< const SharedServiceMetainfo > meta ) :
			mImpl( meta )
		{
		}

		bool BlockMetainfo::isValid() const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		bool BlockMetainfo::isContext() const
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

		void BlockMetainfo::getDependencies( std::vector< std::string > &dependencies ) const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			dependencies = meta->getDependencies();
		}

		void BlockMetainfo::getInletMetainfos( std::vector< InletMetainfo > &inlets )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			
			auto tmp = meta->getInletMetainfos();

			for ( auto it : tmp )
				inlets.push_back( InletMetainfo( it ) );
		}

		void BlockMetainfo::getOutletMetainfos( std::vector< OutletMetainfo > &outlets )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );

			auto tmp = meta->getOutletMetainfos();

			for ( auto it : tmp )
				outlets.push_back( OutletMetainfo( it ) );
		}

		void BlockMetainfo::getParameterMetainfos( std::vector< ParameterMetainfo > &parameters )
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );

			auto tmp = meta->getParameterMetainfos();

			for ( auto it : tmp )
				parameters.push_back( ParameterMetainfo( it ) );
		}

		void BlockMetainfo::getDefaultUpdatePolicy( std::vector< std::vector< std::string > > &policy ) const
		{
			std::shared_ptr< const SharedServiceMetainfo > meta = checkValidity< const SharedServiceMetainfo >( mImpl, "block metainfo" );
			policy = meta->getDefaultUpdatePolicy();
		}
	}
}