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
#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealCustomTypeMetainfo.h"
#include "engine/_2RealBundleMetainfoImpl.h"

namespace _2Real
{
	namespace bundle
	{

		BundleMetainfo::BundleMetainfo( std::shared_ptr< BundleMetainfoImpl > metainfo ) :
			mImpl( metainfo )
		{
		}


		void BundleMetainfo::setAuthor( const std::string author )
		{
			mImpl->setAuthor( author );
		}

		void BundleMetainfo::setDescription( const std::string description )
		{
			mImpl->setDescription( description );
		}

		void BundleMetainfo::setContact( const std::string contact )
		{
			mImpl->setContact( contact );
		}

		void BundleMetainfo::setCategory( const std::string category )
		{
			mImpl->setCategory( category );
		}

		void BundleMetainfo::setVersion( const unsigned int major, const unsigned int minor, const unsigned int revision )
		{
			mImpl->setVersion( Version( major, minor, revision ) );
		}
	
		void BundleMetainfo::exportsType( const std::string name, const std::vector< FieldDeclaration > fields )
		{
			mImpl->exportType( name, fields );
		}

		void BundleMetainfo::exportsBlock( const std::string name, const std::vector< InletDeclaration > inlets,  const std::vector< OutletDeclaration > outlets, const std::vector< ParameterDeclaration > parameters )
		{
			mImpl->exportBlock( name, inlets, outlets, parameters );
		}

	}
}