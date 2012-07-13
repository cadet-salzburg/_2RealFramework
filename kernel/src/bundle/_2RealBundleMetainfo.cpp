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
#include "engine/_2RealMetainfo.h"
#include "helpers/_2RealVersion.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		BundleMetainfo::BundleMetainfo( Metainfo &info ) :
			m_Impl( info )
		{
		}

		void BundleMetainfo::setDescription( std::string const& description )
		{
			m_Impl.setDescription( trim( description ) );
		}

		void BundleMetainfo::setVersion( unsigned int major, unsigned int minor, unsigned int revision )
		{
			m_Impl.setVersion( Version( major, minor, revision ) );
		}

		void BundleMetainfo::setAuthor( std::string const& author )
		{
			m_Impl.setAuthor( trim( author ) );
		}

		void BundleMetainfo::setContact( std::string const& contact )
		{
			m_Impl.setContact( trim( contact ) );
		}

		void BundleMetainfo::setCategory( std::string const& category )
		{
			m_Impl.setCategory( trim( category ) );
		}

		ContextBlockMetainfo & BundleMetainfo::exportContextBlockInternal( AbstractBlockCreator &obj )
		{
			return m_Impl.setContextBlockCreator( obj );
		}

		BlockMetainfo & BundleMetainfo::exportBlockInternal( AbstractBlockCreator &obj, std::string const& blockName )
		{
			return m_Impl.setBlockCreator( toLower( trim( blockName ) ), obj );
		}
	}
}