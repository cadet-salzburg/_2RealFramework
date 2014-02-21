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

#include "bundle/_2RealBundleMetainfo.h"
#include "bundle/_2RealFunctionBlockMetainfo.h"
#include "bundle/_2RealContextBlockMetainfo.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealSharedLibraryMetainfo.h"

namespace _2Real
{
	namespace bundle
	{

		BundleMetainfo::BundleMetainfo( std::shared_ptr< SharedLibraryMetainfo > metainfo ) :
			mImpl( metainfo )
		{
		}

		TypeMetainfo BundleMetainfo::createTypeMetainfo( std::string const& name )
		{
			return TypeMetainfo( mImpl.lock()->createType( name ) );
		}

		FunctionBlockMetainfo BundleMetainfo::createFunctionBlockMetainfo( std::string const& name )
		{
			return FunctionBlockMetainfo( mImpl.lock()->createService( name, false ) );
		}

		ContextBlockMetainfo BundleMetainfo::createContextBlockMetainfo( std::string const& name )
		{
			return ContextBlockMetainfo( mImpl.lock()->createService( name, true ) );
		}

		void BundleMetainfo::setAuthor( std::string const& author )
		{
			mImpl.lock()->setAuthor( author );
		}

		void BundleMetainfo::setDescription( std::string const& description )
		{
			mImpl.lock()->setDescription( description );
		}

		void BundleMetainfo::setContact( std::string const& contact )
		{
			mImpl.lock()->setContact( contact );
		}

		void BundleMetainfo::setCategory( std::string const& category )
		{
			mImpl.lock()->setCategory( category );
		}

		void BundleMetainfo::setVersion( const unsigned int major, const unsigned int minor, const unsigned int revision )
		{
			mImpl.lock()->setVersion( Version( major, minor, revision ) );
		}
	}
}