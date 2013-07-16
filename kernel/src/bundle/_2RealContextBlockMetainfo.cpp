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

#include "bundle/_2RealContextBlockMetainfo.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealParameterMetadata.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		ContextBlockMetainfo::ContextBlockMetainfo( BlockMetadata &data ) :
			m_Impl( data )
		{
		}

		void ContextBlockMetainfo::setDescription( std::string const& description )
		{
			m_Impl.setDescription( description );
		}

		void ContextBlockMetainfo::setThreadingPolicy( ThreadingPolicy const& policy )
		{
			m_Impl.setThreadingPolicy( policy );
		}

		void ContextBlockMetainfo::addOutlet( std::string const& name, std::string const& typeName )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			// check the existence of the type ( pre-defined, as well as self registered types are possible )
			OutletMetadata *data = new OutletMetadata( trimmed, typeName );
			m_Impl.addOutlet( data );
		}
	}
}