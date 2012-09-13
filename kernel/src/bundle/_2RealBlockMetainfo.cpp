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
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealParameterMetadata.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		BlockMetainfo::BlockMetainfo( BlockMetadata &data ) :
			m_Impl( data )
		{
		}

		void BlockMetainfo::setDescription( std::string const& description )
		{
			m_Impl.setDescription( description );
		}

		void BlockMetainfo::setCategory( std::string const& category )
		{
			m_Impl.setDescription( category );
		}

		void BlockMetainfo::setThreadingPolicy( ThreadingPolicy const& policy )
		{
			m_Impl.setThreadingPolicy( policy );
		}

		void BlockMetainfo::addInletInternal( std::string const& name, TypeDescriptor const *const descriptor, Any const& init, AnyOptionSet const& options, InletPolicy const& defaultPolicy )
		{
			checkChars( toLower( trim( name ) ) );
			InletMetadata *data = new InletMetadata( trim( name ), descriptor, init, options, defaultPolicy, false );
			m_Impl.addInlet( *data );
		}

		void BlockMetainfo::addMultiInletInternal( std::string const& name, TypeDescriptor const *const descriptor, Any const& init, AnyOptionSet const& options, InletPolicy const& defaultPolicy )
		{
			checkChars( toLower( trim( name ) ) );
			InletMetadata *data = new InletMetadata( trim( name ), descriptor, init, options, defaultPolicy, true );
			m_Impl.addInlet( *data );
		}

		void BlockMetainfo::addOutletInternal( std::string const& name, TypeDescriptor const *const descriptor, Any const& init )
		{
			checkChars( toLower( trim( name ) ) );
			OutletMetadata *data = new OutletMetadata( trim( name ), descriptor, init );
			m_Impl.addOutlet( *data );
		}
	}
}